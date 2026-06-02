#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gpiod.h>
#include <signal.h>
#include <stdbool.h>
#include <poll.h>

// LED Output Configuration - GPIO0_36
static const char *led_chip_path = "/dev/gpiochip2";
static const unsigned int led_line_offset = 36;
static struct gpiod_line_request *led_request = NULL;
static enum gpiod_line_value led_value = GPIOD_LINE_VALUE_INACTIVE;

// Input Configuration - GPIO1_23
static const char *input_chip_path = "/dev/gpiochip3"; 
static const unsigned int input_line_offset = 23;
static struct gpiod_line_request *input_request = NULL;

volatile bool running = true;

/**
 * Request a GPIO line as output.
 */
static struct gpiod_line_request *request_output_line(const char *chip_path, unsigned int offset, enum gpiod_line_value value, const char *consumer) {
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *line_cfg = NULL;
    struct gpiod_chip *chip = NULL;
    int ret;

    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        fprintf(stderr, "Failed to open output GPIO chip: %s\n", strerror(errno));
        return NULL;
    }

    settings = gpiod_line_settings_new();
    if (!settings) goto cleanup;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, value);

    line_cfg = gpiod_line_config_new();
    if (!line_cfg) goto cleanup;

    ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
    if (ret) goto cleanup;

    if (consumer) {
        req_cfg = gpiod_request_config_new();
        if (req_cfg) gpiod_request_config_set_consumer(req_cfg, consumer);
    }

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        fprintf(stderr, "Failed to request output GPIO line: %s\n", strerror(errno));
    }

cleanup:
    if (req_cfg) gpiod_request_config_free(req_cfg);
    if (line_cfg) gpiod_line_config_free(line_cfg);
    if (settings) gpiod_line_settings_free(settings);
    if (chip) gpiod_chip_close(chip);
    return request;
}

/**
 * Request a GPIO line as an input with edge detection (Interrupt).
 */
static struct gpiod_line_request *request_input_line(const char *chip_path, unsigned int offset, const char *consumer) {
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *line_cfg = NULL;
    struct gpiod_chip *chip = NULL;
    int ret;

    chip = gpiod_chip_open(chip_path);
    if (!chip) {
        fprintf(stderr, "Failed to open input GPIO chip: %s\n", strerror(errno));
        return NULL;
    }

    settings = gpiod_line_settings_new();
    if (!settings) goto cleanup;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_edge_detection(settings, GPIOD_LINE_EDGE_RISING);

    line_cfg = gpiod_line_config_new();
    if (!line_cfg) goto cleanup;

    ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
    if (ret) goto cleanup;

    req_cfg = gpiod_request_config_new();
    if (req_cfg) {
        gpiod_request_config_set_consumer(req_cfg, consumer);
        // Important for events: request needs an event buffer size
        gpiod_request_config_set_event_buffer_size(req_cfg, 16);
    }

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        fprintf(stderr, "Failed to request input GPIO line: %s\n", strerror(errno));
    }

cleanup:
    if (req_cfg) gpiod_request_config_free(req_cfg);
    if (line_cfg) gpiod_line_config_free(line_cfg);
    if (settings) gpiod_line_settings_free(settings);
    if (chip) gpiod_chip_close(chip);
    return request;
}

/**
 * Toggle the LED state.
 */
void toggleLED(void) {
    if (!led_request) return;
    
    if (led_value == GPIOD_LINE_VALUE_INACTIVE) {
        gpiod_line_request_set_value(led_request, led_line_offset, GPIOD_LINE_VALUE_ACTIVE);
        led_value = GPIOD_LINE_VALUE_ACTIVE;
        printf("LED turned ON\n");
    } else {
        gpiod_line_request_set_value(led_request, led_line_offset, GPIOD_LINE_VALUE_INACTIVE);
        led_value = GPIOD_LINE_VALUE_INACTIVE;
        printf("LED turned OFF\n");
    }
}

/**
 * Clean up and release GPIO lines.
 */
void cleanup_gpio(void) {
    if (led_request) {
        gpiod_line_request_set_value(led_request, led_line_offset, GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_release(led_request);
        led_request = NULL;
    }
    if (input_request) {
        gpiod_line_request_release(input_request);
        input_request = NULL;
    }
    printf("GPIO lines released\n");
}

void handle_sigint(int sig) {
    printf("\nSIGINT received, cleaning up and exiting...\n");
    running = false;
}

int main() {
    signal(SIGINT, handle_sigint);

    // 1. Initialize the Output (LED)
    led_request = request_output_line(led_chip_path, led_line_offset, GPIOD_LINE_VALUE_INACTIVE, "led-toggle");
    if (!led_request) return EXIT_FAILURE;

    // 2. Initialize the Input (Button/Interrupt source)
    input_request = request_input_line(input_chip_path, input_line_offset, "button-interrupt");
    if (!input_request) {
        cleanup_gpio();
        return EXIT_FAILURE;
    }

    // 3. Get the file descriptor for the input events
    int input_fd = gpiod_line_request_get_fd(input_request);
    if (input_fd < 0) {
        fprintf(stderr, "Failed to get file descriptor for input line\n");
        cleanup_gpio();
        return EXIT_FAILURE;
    }

    struct pollfd pfd;
    pfd.fd = input_fd;
    pfd.events = POLLIN;

    // Create an event buffer to read edge events into
    struct gpiod_edge_event_buffer *event_buffer = gpiod_edge_event_buffer_new(1);

    printf("Waiting for interrupts on %s offset %d...\n", input_chip_path, input_line_offset);

    // 4. Main Event Loop
    while (running) {
        // Poll with a 1-second timeout so the loop can exit smoothly on Ctrl+C
        int ret = poll(&pfd, 1, 1000); 

        if (ret < 0) {
            if (errno == EINTR) continue; // Interrupted by our SIGINT handler
            perror("poll failed");
            break;
        } else if (ret == 0) {
            // Timeout reached, loop continues, checking 'running' flag
            continue;
        }

        // If POLLIN is set, a hardware edge event occurred
        if (pfd.revents & POLLIN) {
            // Read the event to clear the interrupt from the queue
            ret = gpiod_line_request_read_edge_events(input_request, event_buffer, 1);
            if (ret > 0) {
                toggleLED();
            }
        }
    }

    // Cleanup
    gpiod_edge_event_buffer_free(event_buffer);
    cleanup_gpio();
    printf("Exiting program.\n");
    
    return 0;
}

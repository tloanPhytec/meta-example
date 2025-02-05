#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gpiod.h>
#include <signal.h>

static const char *chip_path = "/dev/gpiochip4";
static const unsigned int line_offset = 26;
static struct gpiod_line_request *led_request = NULL;
static enum gpiod_line_value led_value = GPIOD_LINE_VALUE_INACTIVE;

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
        fprintf(stderr, "Failed to open GPIO chip: %s\n", strerror(errno));
        return NULL;
    }

    settings = gpiod_line_settings_new();
    if (!settings) {
        fprintf(stderr, "Failed to create line settings: %s\n", strerror(errno));
        goto cleanup;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, value);

    line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        fprintf(stderr, "Failed to create line config: %s\n", strerror(errno));
        goto cleanup;
    }

    ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);
    if (ret) {
        fprintf(stderr, "Failed to add line settings: %s\n", strerror(errno));
        goto cleanup;
    }

    if (consumer) {
        req_cfg = gpiod_request_config_new();
        if (!req_cfg) {
            fprintf(stderr, "Failed to create request config: %s\n", strerror(errno));
            goto cleanup;
        }
        gpiod_request_config_set_consumer(req_cfg, consumer);
    }

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        fprintf(stderr, "Failed to request GPIO line: %s\n", strerror(errno));
    }

cleanup:
    if (req_cfg) gpiod_request_config_free(req_cfg);
    if (line_cfg) gpiod_line_config_free(line_cfg);
    if (settings) gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);
    return request;
}

/**
 * Turn LED OFF.
 */
void ledOFF(void) {
    if (!led_request) {
        led_request = request_output_line(chip_path, line_offset, GPIOD_LINE_VALUE_INACTIVE, "blink");
        if (!led_request) {
            fprintf(stderr, "Failed to request GPIO line for LED OFF.\n");
            return;
        }
    }
    gpiod_line_request_set_value(led_request, line_offset, GPIOD_LINE_VALUE_INACTIVE);
    led_value = GPIOD_LINE_VALUE_INACTIVE;
    printf("LED turned OFF\n");
}

/**
 * Turn LED ON.
 */
void ledON(void) {
    if (!led_request) {
        led_request = request_output_line(chip_path, line_offset, GPIOD_LINE_VALUE_ACTIVE, "blink");
        if (!led_request) {
            fprintf(stderr, "Failed to request GPIO line for LED ON.\n");
            return;
        }
    }
    gpiod_line_request_set_value(led_request, line_offset, GPIOD_LINE_VALUE_ACTIVE);
    led_value = GPIOD_LINE_VALUE_ACTIVE;
    printf("LED turned ON\n");
}

/**
 * Clean up and release GPIO line.
 */
void cleanup_gpio(void) {
    if (led_request) {
        gpiod_line_request_release(led_request);
        led_request = NULL;
        printf("GPIO line released\n");
    }
}

void handle_sigint(int sig) {
    printf("\nSIGINT received, cleaning up and exiting...\n");
    running = false;  // Stop the main loop
}

int main() {
    // Register the signal handler
    signal(SIGINT, handle_sigint);

    // Infinite loop that stops when SIGINT is received
    while (running) {
        ledON();
        sleep(1);
        ledOFF();
        sleep(1);
    }

    // Cleanup before exiting
    cleanup_gpio();
    printf("GPIO cleaned up. Exiting program.\n");
    return 0;
}

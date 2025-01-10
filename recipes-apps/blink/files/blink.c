#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_PATH "/sys/class/leds/led-1/brightness"
#define DELAY 500000  // 500 milliseconds (half a second)

void set_led_brightness(int value) {
    int fd = open(LED_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open LED brightness control");
        exit(1);
    }

    // Write the brightness value (0 = off, 1 = on)
    if (dprintf(fd, "%d", value) < 0) {
        perror("Failed to set LED brightness");
        close(fd);
        exit(1);
    }

    close(fd);
}

int main() {
    while (1) {
        set_led_brightness(255);  // Turn LED on
        usleep(DELAY);           // Wait
        set_led_brightness(0);   // Turn LED off
        usleep(DELAY);           // Wait
    }
    return 0;
}

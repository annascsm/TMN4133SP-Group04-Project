#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "file_operations.h"

// Create or open a file
void create_or_open_file(const char *filename) {
    int fd = open(filename, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        write(STDERR_FILENO, "Error creating/opening file\n", 28);
    } else {
        write(STDOUT_FILENO, "File created/opened successfully.\n", 34);
        close(fd);
    }
}

// Change file permissions
void change_file_permissions(const char *filename, const char *permissions) {
    mode_t mode = strtol(permissions, NULL, 8);
    if (chmod(filename, mode) == -1) {
        write(STDERR_FILENO, "Error changing file permissions\n", 32);
    } else {
        write(STDOUT_FILENO, "File permissions changed successfully.\n", 40);
    }
}

// Read from a file
void read_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        write(STDERR_FILENO, "Error opening file\n", 20);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1) {
        write(STDERR_FILENO, "Error reading file\n", 20);
    }

    close(fd);
}

// Write to a file
void write_file(const char *filename) {
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        write(STDERR_FILENO, "Error opening file\n", 20);
        return;
    }

    char buffer[1024];
    write(STDOUT_FILENO, "Enter text to write to the file (Ctrl+D to finish):\n", 52);
    ssize_t bytes_written;
    while ((bytes_written = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        if (write(fd, buffer, bytes_written) == -1) {
            write(STDERR_FILENO, "Error writing to file\n", 23);
            break;
        }
    }

    close(fd);
}

// Delete a file
void delete_file(const char *filename) {
    if (unlink(filename) == -1) {
        write(STDERR_FILENO, "Error deleting file\n", 21);
    } else {
        write(STDOUT_FILENO, "File deleted successfully.\n", 27);
    }
}


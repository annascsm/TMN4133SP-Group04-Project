#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "directory_operations.h"

// Create a directory
void create_directory(const char *dirname) {
    if (mkdir(dirname, 0755) == -1) {
        write(STDERR_FILENO, "Error creating directory\n", 26);
    } else {
        write(STDOUT_FILENO, "Directory created successfully.\n", 32);
    }
}

// Delete a directory
void delete_directory(const char *dirname) {
    if (rmdir(dirname) == -1) {
        write(STDERR_FILENO, "Error deleting directory\n", 26);
    } else {
        write(STDOUT_FILENO, "Directory deleted successfully.\n", 32);
    }
}

// Print current working directory
void print_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        write(STDERR_FILENO, "Error getting current directory\n", 32);
    } else {
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    }
}

// List current directory contents
void list_current_directory() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        write(STDERR_FILENO, "Error opening directory\n", 25);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
        write(STDOUT_FILENO, "\n", 1);
    }

    closedir(dir);
}


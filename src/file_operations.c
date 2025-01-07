#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "file_operations.h"

#define BUFFER_SIZE 1024

// Create or open a file
void create_or_open_file(const char *filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        write(STDERR_FILENO, "Error creating/opening file\n", 28);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        write(STDERR_FILENO, "Error retrieving file information\n", 35);
        close(fd);
        return;
    }

    if (st.st_size == 0)
    {
        write(STDOUT_FILENO, "File created successfully. The file is empty.\n", 47);
    }
    else
    {
        write(STDOUT_FILENO, "File exists. Opening the file...\n", 34);
        char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == -1)
        {
            write(STDERR_FILENO, "Error reading file\n", 20);
        }
        else
        {
            buffer[bytes_read] = '\0';
            write(STDOUT_FILENO, "File content:\n", 14);
            write(STDOUT_FILENO, buffer, bytes_read);
        }
    }

    close(fd);
}

// Change file permissions
void change_file_permissions(const char *filename, const char *permissions)
{
    mode_t mode = strtol(permissions, NULL, 8);
    if (chmod(filename, mode) == -1)
    {
        write(STDERR_FILENO, "Error changing file permissions\n", 32);
    }
    else
    {
        write(STDOUT_FILENO, "File permissions changed successfully.\n", 40);
    }
}

// Read from a file
void read_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        write(STDERR_FILENO, "Error opening file\n", 20);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

    if (bytes_read == 0)
    {
        // The file is empty
        const char *empty_message = "The file is empty\n";
        write(STDOUT_FILENO, empty_message, strlen(empty_message));
    }
    else if (bytes_read == -1)
    {
        // Error reading the file
        write(STDERR_FILENO, "Error reading file\n", 20);
    }
    else
    {
        // File has content, print it
        write(STDOUT_FILENO, buffer, bytes_read);

        // Continue reading if there is more content
        while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        // Handle any reading error
        if (bytes_read == -1)
        {
            write(STDERR_FILENO, "Error reading file\n", 20);
        }
    }

    close(fd);
}

// Write to a file using system calls
void write_file(const char *filename)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Open the file in append mode
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1)
    {
        write(STDERR_FILENO, "Error opening file\n", 19);
        return;
    }

    // Write a newline at the beginning of each new run (before writing user input)
    if (lseek(fd, 0, SEEK_END) > 0)
    { // Only add newline if file is not empty
        write(fd, "\n", 1);
    }

    // Prompt user for input
    const char *prompt = "Enter text to write to the file: ";
    write(STDOUT_FILENO, prompt, strlen(prompt));

    // Read input from user
    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1)
    {
        write(STDERR_FILENO, "Error reading input\n", 20);
        close(fd);
        return;
    }

    // Null-terminate the input to avoid issues with strings
    buffer[bytes_read - 1] = '\0'; // Remove the newline character

    // Write the input to the file
    ssize_t bytes_written = write(fd, buffer, strlen(buffer));
    if (bytes_written == -1)
    {
        write(STDERR_FILENO, "Error writing to file\n", 22);
    }
    else
    {
        const char *success_message = "Text written to the file successfully.\n";
        write(STDOUT_FILENO, success_message, strlen(success_message));
    }

    // Close the file
    close(fd);
}

// Delete a file
void delete_file(const char *filename)
{
    if (unlink(filename) == -1)
    {
        write(STDERR_FILENO, "Error deleting file\n", 21);
    }
    else
    {
        write(STDOUT_FILENO, "File deleted successfully.\n", 27);
    }
}

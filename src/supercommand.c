#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "file_operations.h"
#include "directory_operations.h"
#include "keylogger.h"

// Function to validate numeric input
int get_validated_choice(int min, int max)
{
    char input[16];
    int choice = -1;

    while (choice < min || choice > max)
    {
        ssize_t bytes_read = read(STDIN_FILENO, input, sizeof(input) - 1);

        if (bytes_read > 0)
        {
            input[bytes_read - 1] = '\0'; // Replace newline with null terminator
            int is_numeric = 1;

            // Check for empty input (just Enter pressed)
            if (input[0] == '\0')
            {
                return -1; // Return -1 or any other value to indicate a fallback action (e.g., previous menu)
            }

            for (int i = 0; input[i] != '\0'; i++)
            {
                if (!isdigit(input[i]))
                {
                    is_numeric = 0;
                    break;
                }
            }

            if (is_numeric)
            {
                choice = atoi(input);
            }
        }

        if (choice < min || choice > max)
        {
            write(STDOUT_FILENO, "Invalid choice. Try again.\n", 27);
        }
    }

    return choice;
}

// Function to wait for the user to press Enter to continue
void wait_for_enter_to_continue()
{
    write(STDOUT_FILENO, "\nPress Enter to continue...\n", 27);
    char buffer[2];
    read(STDIN_FILENO, buffer, sizeof(buffer)); // Wait for Enter key
}

// Function to execute tasks directly from command line
void execute_command_line(int argc, char *argv[])
{
    if (argc < 4)
    {
        write(STDERR_FILENO, "Error: Insufficient arguments.\nUsage: ./supercommand -m <mode> <operation> [arguments]\n", 83);
        return;
    }

    int mode = atoi(argv[2]);
    int operation = atoi(argv[3]);

    char *filename;
    char *permissions;

    if (mode == 1)
    { // File Operations
        if (operation == 1 && argc >= 5)
        { // Create or Open File
            filename = argv[4];
            create_or_open_file(filename);
        }
        else if (operation == 2 && argc >= 6)
        { // Change File Permissions
            filename = argv[4];
            permissions = argv[5];
            change_file_permissions(filename, permissions);
        }
        else if (operation == 3 && argc >= 5)
        { // Read File
            filename = argv[4];
            read_file(filename);
        }
        else if (operation == 4 && argc >= 5)
        { // Write File
            filename = argv[4];
            write_file(filename);
        }
        else if (operation == 5 && argc >= 5)
        { // Delete File
            filename = argv[4];
            delete_file(filename);
        }
        else
        {
            write(STDERR_FILENO, "Error: Invalid file operation arguments.\n", 41);
        }
    }
    else if (mode == 2)
    { // Directory Operations
        char *dirname;

        if (operation == 1 && argc >= 5)
        { // Create Directory
            dirname = argv[4];
            create_directory(dirname);
        }
        else if (operation == 2 && argc >= 5)
        { // Delete Directory
            dirname = argv[4];
            delete_directory(dirname);
        }
        else if (operation == 3)
        { // Print Current Directory
            print_current_directory();
        }
        else if (operation == 4)
        { // List Directory
            list_current_directory();
        }
        else
        {
            write(STDERR_FILENO, "Error: Invalid directory operation arguments.\n", 46);
        }
    }
    else if (mode == 3)
    { // Keylogger Operations
        if (operation == 1)
        { // Start Keylogger
            initialize_keylogger();
        }
        else if (operation == 2)
        { // Stop Keylogger
            stop_keylogger();
        }
        else if (operation == 3)
        { // Check Keylogger Status
            check_keylogger_status();
        }
        else
        {
            write(STDERR_FILENO, "Error: Invalid keylogger operation arguments.\n", 46);
        }
    }
    else
    {
        write(STDERR_FILENO, "Error: Invalid mode.\n", 22);
    }
}

// Function to display the main menu
void display_main_menu()
{
    const char *menu = "\n--- Main Menu ---\n"
                       "1. File Operations\n"
                       "2. Directory Operations\n"
                       "3. Keylogger Operations\n"
                       "0. Exit\n"
                       "Enter your choice: ";
    write(STDOUT_FILENO, menu, strlen(menu));
}

// Function to display the file operations submenu
void display_file_menu()
{
    system("clear");

    const char *menu = "\n--- File Operations ---\n"
                       "1. Create/Open File\n"
                       "2. Change File Permissions\n"
                       "3. Read File\n"
                       "4. Write File\n"
                       "5. Delete File\n"
                       "0. Back\n"
                       "Enter your choice: ";
    write(STDOUT_FILENO, menu, strlen(menu));
}

// Function to display the directory operations submenu
void display_directory_menu()
{
    system("clear");

    const char *menu = "\n--- Directory Operations ---\n"
                       "1. Create Directory\n"
                       "2. Delete Directory\n"
                       "3. Print Current Directory\n"
                       "4. List Current Directory\n"
                       "0. Back\n"
                       "Enter your choice: ";
    write(STDOUT_FILENO, menu, strlen(menu));
}

// Handle file operations menu
void handle_file_operations()
{
    system("clear");

    int choice;

    do
    {
        display_file_menu();
        fflush(stdout);
        choice = get_validated_choice(0, 5);

        char filename[256];
        char permissions[4];

        switch (choice)
        {
        case 1:
            write(STDOUT_FILENO, "Enter file name with path: ", 27);
            read(STDIN_FILENO, filename, sizeof(filename));
            filename[strcspn(filename, "\n")] = '\0'; // Remove newline
            create_or_open_file(filename);
            break;
        case 2:
            write(STDOUT_FILENO, "Enter file name: ", 18);
            read(STDIN_FILENO, filename, sizeof(filename));
            filename[strcspn(filename, "\n")] = '\0';
            write(STDOUT_FILENO, "Enter new permissions (e.g., 777): ", 36);
            read(STDIN_FILENO, permissions, sizeof(permissions));
            permissions[strcspn(permissions, "\n")] = '\0';
            change_file_permissions(filename, permissions);
            break;
        case 3:
            write(STDOUT_FILENO, "Enter file name: ", 18);
            read(STDIN_FILENO, filename, sizeof(filename));
            filename[strcspn(filename, "\n")] = '\0';
            read_file(filename);
            break;
        case 4:
            write(STDOUT_FILENO, "Enter file name: ", 18);
            read(STDIN_FILENO, filename, sizeof(filename));
            filename[strcspn(filename, "\n")] = '\0';
            write_file(filename);
            break;
        case 5:
            write(STDOUT_FILENO, "Enter file name: ", 18);
            read(STDIN_FILENO, filename, sizeof(filename));
            filename[strcspn(filename, "\n")] = '\0';
            delete_file(filename);
            break;
        case 0:
            write(STDOUT_FILENO, "Returning to Main Menu.\n", 24);
            break;
        default:
            write(STDOUT_FILENO, "Invalid choice. Try again.\n", 27);
        }

        // Ask the user to press Enter to continue
        wait_for_enter_to_continue();

        // Clear screen after user presses Enter to continue
        system("clear");

    } while (choice != 0);
}

// Handle directory operations menu
void handle_directory_operations()
{
    int choice;

    do
    {
        display_directory_menu();
        fflush(stdout);
        choice = get_validated_choice(0, 4);

        char directory[256];

        switch (choice)
        {
        case 1:
            write(STDOUT_FILENO, "Enter directory name with path: ", 33);
            read(STDIN_FILENO, directory, sizeof(directory));
            directory[strcspn(directory, "\n")] = '\0';
            create_directory(directory);
            break;
        case 2:
            write(STDOUT_FILENO, "Enter directory name with path: ", 33);
            read(STDIN_FILENO, directory, sizeof(directory));
            directory[strcspn(directory, "\n")] = '\0';
            delete_directory(directory);
            break;
        case 3:
            print_current_directory();
            break;
        case 4:
            list_current_directory();
            break;
        case 0:
            write(STDOUT_FILENO, "Returning to Main Menu.\n", 24);
            break;
        default:
            write(STDOUT_FILENO, "Invalid choice. Try again.\n", 27);
        }

        // Ask the user to press Enter to continue
        wait_for_enter_to_continue();

        // Clear screen after user presses Enter to continue
        system("clear");

    } while (choice != 0);
}

// Handle keylogger operations menu
void handle_keylogger_operations()
{
    system("clear");

    int choice;
    do
    {
        write(STDOUT_FILENO, "\nKeylogger Control Menu:\n", 25);
        write(STDOUT_FILENO, "1. Start keylogger\n", 19);
        write(STDOUT_FILENO, "2. Stop keylogger\n", 18);
        write(STDOUT_FILENO, "3. Check keylogger status\n", 26);
        write(STDOUT_FILENO, "0. Returning to Main Menu\n", 26);
        write(STDOUT_FILENO, "Enter your choice: ", 19);
        fflush(stdout);
        choice = get_validated_choice(0, 3);

        switch (choice)
        {
        case 1:
            initialize_keylogger();
            break;
        case 2:
            stop_keylogger();
            break;
        case 3:
            check_keylogger_status();
            break;
        case 0:
            return;
        default:
            write(STDOUT_FILENO, "Invalid choice. Please try again.\n", 33);
        }

        // Ask the user to press Enter to continue
        wait_for_enter_to_continue();

        // Clear screen after user presses Enter to continue
        system("clear");

    } while (choice != 0);
}

int main(int argc, char *argv[])
{
    // Check for command-line execution mode
    if (argc > 1 && strcmp(argv[1], "-m") == 0)
    {
        execute_command_line(argc, argv);
        return 0;
    }

    // Menu system
    int choice = -1;
    while (choice != 0)
    {
        // Clear the screen before displaying the main menu
        system("clear");

        display_main_menu();
        fflush(stdout); // Ensure any output is fully flushed before waiting for input

        choice = get_validated_choice(0, 3); // Validate user choice

        switch (choice)
        {
        case 1:
            handle_file_operations();
            break;
        case 2:
            handle_directory_operations();
            break;
        case 3:
            handle_keylogger_operations();
            break;
        case 0:
            write(STDOUT_FILENO, "Exiting program.\n", 18);
            break;
        default:
            write(STDOUT_FILENO, "Invalid choice. Try again.\n", 27);
        }
    }
    return 0;
}
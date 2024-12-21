# TMN4133 System Programming Group Project

## 👨🏻‍💻 Developed By Group 04

- **78855** ANNASTASHA CHANG SEE MAY
- **79260** EE CHEE FAT
- **79027** CHAI CHENG KANG
  
This project implements a **menu-driven system** and **command-line interface** for performing file, directory, and keylogger operations using system calls in C. It supports both interactive menu-based and command-line execution modes. This project was completed as part of our course requirements.

---

## 🛠️ **Build Instructions**

<details>
<summary>Click to expand!</summary><br>

### 1. Clone the repository (if not already done):
```bash
git clone <repository-URL>
cd TMN4133SP--Group04-Project
```

### 2. Compile the program: Use the provided `Makefile` to compile all source files:
```bash
make
```

### 3. Run the program:
```bash
./supercommand
```

### 3. Clean build files:
```bash
make clean
```

</details>

## ⚙️ **Features**

<details>
<summary>Click to expand!</summary><br>
  
### 1. File Operations (Menu Option 1)
- **Create/Open File**: Create or open a file with a specified name and path.
- **Change File Permissions**: Modify the file's permissions (e.g., `777`) based on user input.
- **Read File**: Read content from a file and display it on the standard output.
- **Write File**: Write content from standard input to a file.
- **Delete File**: Remove a file given its name and path.

### 2. Directory Operations (Menu Option 2)
- **Create Directory**: Create a directory with a specified name and path.
- **Delete Directory**: Remove a directory with a specified name and path.
- **Print Current Directory**: Display the current working directory.
- **List Current Directory**: List all files and directories in the current directory.

### 3. Keylogger Operations (Menu Option 3)
- Run a keylogger in the background.
- Record all keystrokes from the keyboard into a file named `keylog.txt`.
- Add a timestamp at the start of the keylogging session.
</details>

---

## ℹ️ **Usage**

<details>
<summary>Click to expand!</summary><br> 
  
The program can be run in two modes:
1. **Menu-Based Mode**: Navigate the menu to select operations interactively.
2. **Command-Line Mode**: Perform specific tasks directly through command-line arguments.

### ☰ Menu-Based Mode
Run the program without additional arguments:
```bash
./supercommand
```

### </> Command-Line Mode
Run the program with the `-m` flag to specify the mode and operation:
```bash
./supercommand -m <mode> <operation> [arguments]
```
Examples:
- Create a New File:
  ```bash
  ./supercommand -m 1 1 newFile.txt
  ```
- Change File Permissions:
  ```bash
  ./supercommand -m 1 2 myFile.txt 777
  ```
- List Current Directory:
  ```bash
  ./supercommand -m 2 4
  ```
</details>

---
  
## 📂 **File Structure**

```txt
TNN4133SP--Group04-Project/
├── include/
│   ├── file_operations.h        # Header file for file operations
│   ├── directory_operations.h   # Header file for directory operations
│   └── keylogger.h             # Header file for keylogger operations
├── src/
│   ├── supercommand.c          # Main program file
│   ├── file_operations.c       # Implementation of file operations
│   ├── directory_operations.c  # Implementation of directory operations
│   └── keylogger.c            # Implementation of keylogger operations
├── Makefile                    # Build configuration file
└── README.md                   # Project documentation (this file)
```

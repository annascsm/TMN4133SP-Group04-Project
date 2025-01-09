# **TMN4133 System Programming Group Project**

## 👨🏻‍💻 **Developed By Group 04**

- **78855** ANNASTASHA CHANG SEE MAY  
- **79260** EE CHEE FAT  
- **79027** CHAI CHENG KANG  
- **81031** RAYNOLD GERRY ANAK ALFRED  

This project implements a **menu-driven system** and **command-line interface** for performing file, directory, and keylogger operations using system calls in C. It supports both interactive menu-based and command-line execution modes. This project was completed as part of our course requirements under the supervision of **Associate Professor Dr. Johari bin Abdullah**, at the **Universiti of Malaysia Sarawak, UNIMAS**.

---

## 🔧 **Build Instructions**

<details>
<summary>Click to expand!</summary><br>

### **Step 1: Install Required Tools**  
Before starting, ensure the following tools are installed:  
- **Make Utility**: For building the project.  
  Install using:  
  ```bash
  sudo apt install make
  ```  
- **GCC Compiler**: For compiling C code.  
  Install using:  
  ```bash
  sudo apt install build-essential
  ```

---

### **Step 2: Clone the Repository**  
Use the following commands to download the project and navigate to the directory:  
```bash
git clone <repository-URL>
cd TMN4133SP--Group04-Project
```

---

### **Step 3: Build the Program**  
Run the `make` command to compile the program:  
```bash
make
```

---

### **Step 4: Run the Program**  
- For **FULL functionality**, use:  
  ```bash
  sudo ./supercommand
  ```  
  Note: `sudo` is required for keylogger operations. 

---

### **Step 5: Clean Build Files (Optional)**  
To remove compiled files and reset the directory:  
```bash
make clean
```

</details>

---

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
- **Start Keylogger**: Run a keylogger in the background. All the keystrokes from the keyboard will be recorded into a file named `keylog.txt`. 
- **Stop Keylogger**: Stop the keylogger process.
- **Check Keylogger Status**: Check the status of the keylogger to see whether it has been started or stopped.

</details>

---

## ℹ️ **Usage Instructions**

<details>
<summary>Click to expand!</summary><br>

The program can be used in two ways:  
1. **Menu-Based Mode**: Navigate through the menu interactively.  
2. **Command-Line Mode**: Run specific tasks directly with commands.

---

### **☞ Menu-Based Mode**  
Use the following command to start the Menu System for full functionality :  
```bash
sudo ./supercommand
```  

---

### **</> Command-Line Mode**  
For running **File Operation** and **Directory Operation** use the following command:  

```bash
./supercommand -m <mode> <operation> [arguments]
```  

Whereby,
- 1st parameter:   `mode` refers to file/directory operation mode
- 2nd parameter: `operation` refers to the provided selection for file/directory operation 
- 3rd parameter: `arguments` name of the file / file permission

**Note:** For **Change File Permission**, an additional argument specifying the file permission code (e.g., `755`, `777`) is required.

---

For running **Keylogger Operation** use the following command:  

```bash
sudo ./supercommand -m <mode> <operation>
```    

Whereby,
- 1st parameter:   `mode` refers to the keylogger operation mode
- 2nd parameter: `operation` refers to refers to the provided selection for keylogger operation 

**Note:** `sudo` is required for keylogger operations as the searching and reading of the keyboard event needs the root privileges.

---

For viewing the manual page for supercommand, use the following command:

```bash
man supercommand
```

---

### Examples command for File Operation and Directory Operation:  
- **Create a New File**:  
  ```bash
  ./supercommand -m 1 1 newFile.txt
  ```

- **Change File Permissions**:  
  ```bash
  ./supercommand -m 1 2 myFile.txt 755
  ```  
  (Provide the permission code, e.g., `755`, `777` or any other combination.)  

- **List Current Directory**:  
  ```bash
  ./supercommand -m 2 4
  ```

---

### Examples command for Keylogger Operation: 
**Requires `sudo` to work:**  
- **Start Keylogger**:  
  ```bash
  sudo ./supercommand -m 3 1
  ```
- **Stop Keylogger**:  
  ```bash
  sudo ./supercommand -m 3 2
  ```
- **Check Keylogger Status**:  
  ```bash
  sudo ./supercommand -m 3 3
  ```

</details>

---

## 📂 **File Structure**

```plaintext
TNN4133SP--Group04-Project/
├── include/
│   ├── file_operations.h        # Header file for file operations
│   ├── directory_operations.h   # Header file for directory operations
│   └── keylogger.h              # Header file for keylogger operations
├── src/
│   ├── supercommand.c          # Main program file
│   ├── file_operations.c       # Implementation of file operations
│   ├── directory_operations.c  # Implementation of directory operations
│   └── keylogger.c             # Implementation of keylogger operations
│   └── manpage                 # Manual page for supercommand
├── Makefile                    # Build configuration file
└── README.md                   # Project documentation (this file)
```

---

## 🙏 **Acknowledgements**

We would like to express our deepest gratitude to **Associate Professor Dr. Johari bin Abdullah** for his valuable guidance and support throughout this project. His expertise and encouragement have been instrumental in helping us complete this work.

We also acknowledge the assistance provided by **ChatGPT**, which helped clarify technical concepts and refine our project documentation. Thank you for your support!

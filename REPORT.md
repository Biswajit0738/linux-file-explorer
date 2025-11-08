# Capstone Project Report — Linux File Explorer (C++17)

Student Name: Biswajit Patnaik 
Registration No: 2241016246
Department / Batch: CSE


---

## 1. Introduction
The Linux operating system is built around the concept of files and directories, where almost everything is represented as a file in the filesystem. Understanding how to navigate, manipulate, and manage these files programmatically is a fundamental skill in system-level programming.

This project implements a console-based File Explorer using C++17 and the <filesystem> library, which provides a high-level API for interacting with the operating system’s file system. The application supports essential file operations such as listing directory contents, copying and moving files, creating and removing directories, modifying file permissions, and performing recursive searches.

The purpose of this project is to gain hands-on experience with Linux internal operations and modern C++ filesystem APIs, while reinforcing practical knowledge of command-line workflows and system behavior.

---

## 2. Objectives
-To understand how Linux organizes and manages files and directories at the filesystem level.

-To implement core file operations (create, copy, move, rename, delete) programmatically using C++17 <filesystem>.

-To simulate shell-like commands in a custom CLI environment for real file manipulation.

-To learn how to handle file permissions and interpret rwx bit representations.

-To perform recursive directory traversal and pattern-based file searching.

-To gain practical experience in building a system-level utility that interacts directly with the operating system.

---

## 3. System Requirements
- Ubuntu / Linux OS
- g++ with C++17 support

---

## 4. How to Compile & Run
g++ -std=gnu++17 -O2 main.cpp -o filex
./filex



---

## 5. Features Demonstration (Screenshots)

### 5.1 List directory
![List Directory Screenshot](screenshots/list.png)

### 5.2 Create directory and file
![Create Directory Screenshot](screenshots/create.png)

### 5.3 Copy and Move files
![Copy and Move Screenshot](screenshots/copy-move.png)

### 5.4 Change permissions
![Change Permissions Screenshot](screenshots/chmod.png)

### 5.5 Search files
![Search File Screenshot](screenshots/search.png)

---

## 6. Conclusion
This project helped me understand how Linux handles file systems at a deeper and more practical level. By building a command-line file explorer in C++17, I learned how directories, files, metadata, and permissions are managed internally through system calls and the <filesystem> library. I also gained hands-on experience with recursion, permission bits, path handling, and basic command parsing.

The final program supports real file operations like listing, copying, moving, deleting, searching, and updating permissions directly from the terminal. These are everyday tasks in system administration, and implementing them manually gave me a clearer picture of how tools like ls, cp, mv, and chmod are structured under the hood.

Overall, this project strengthened my understanding of Linux OS concepts and improved my ability to write structured and maintainable C++ code. It also taught me the importance of handling errors, validating user input, and designing interactive terminal applications. The experience was practical, meaningful, and directly relevant to real software development and systems work.
---

## 7. References
- C++17 `<filesystem>` documentation
- Linux manual (man pages)


<div align="center">

# Linux File Explorer (C++17)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![Made with C++](https://img.shields.io/badge/Made%20with-C%2B%2B17-blue.svg)
![Platform: Linux](https://img.shields.io/badge/Platform-Linux-orange.svg)

A **console-based File Explorer** built using **C++17** and the **<filesystem>** library.  
This application provides essential file management operations through a clean, terminal-friendly command interface — similar to lightweight shell utilities.

---
</div>

## 🚀 Features

| Feature | Description |
|--------|-------------|
| **List directories/files** | Display directory contents with metadata. |
| **Navigate folders** | Move through directories using paths. |
| **Create/Delete** | Make or remove files and directories. |
| **Copy & Move** | Transfer and rename files easily. |
| **Change Permissions** | Modify `rwx` permission bits. |
| **Search Files** | Recursive search by file name pattern. |

---

## 🧠 Skills Demonstrated
- Linux filesystem & directory traversal
- C++17 `<filesystem>` API usage
- Command parsing & interactive CLI design
- File permissions (`chmod`) & path handling
- Recursive searching & error handling

---

## 📸 Feature Demonstration (Screenshots)

### 5.1 List Directory
![List Directory Screenshot](screenshots/list.png)

### 5.2 Create Directory & File
![Create Directory Screenshot](screenshots/create.png)

### 5.3 Copy & Move Files
![Copy & Move Screenshot](screenshots/copy-move.png)

### 5.4 Change Permissions
![Change Permissions Screenshot](screenshots/chmod.png)

### 5.5 Search Files
![Search File Screenshot](screenshots/search.png)

---

## 🛠 How to Compile & Run

```bash
g++ -std=gnu++17 main.cpp -o filex
./filex


## 📂 Project Structure

linux-file-explorer/
├── 🧠 main.cpp # Core C++ source code (file explorer implementation)
├── 📄 REPORT.md # Capstone project report with explanations & screenshots
├── 📘 README.md # Documentation & usage guide (this file)
├── 📜 LICENSE # MIT License for open-source use
├── 🖼️ screenshots/ # Demonstration images used in report & README
│ ├── 🖼️ list.png # Directory listing output
│ ├── 🖼️ create.png # Create directory + file example
│ ├── 🖼️ copy-move.png # Copy and move operations demonstration
│ ├── 🖼️ chmod.png # Permission modification example
│ └── 🖼️ search.png # Recursive file search demonstration
└── 📁 testdir/ # Example folder used during testing
├── 📄 file1.txt
└── 📄 renamed.txt

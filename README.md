+# Linux File Explorer (C++17)

A console-based file explorer built using C++17 and the Linux `<filesystem>` library.

## Features
- List directories with details
- Create, rename, copy, and delete files/directories
- Change file permissions
- Recursive file search
- Simple terminal-based command interface

## Build
To compile the program:
g++ -std=gnu++17 -O2 main.cpp -o filex


If your system gives `<filesystem>` linking errors, use:

g++ -std=gnu++17 -lstdc++fs -O2 main.cpp -o filex


To run the program:

./filex

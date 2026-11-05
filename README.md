# CPE2600_Lab9_System_Calls

### Name: **Matthew Schiemann**
### Course: **CPE2600 - Sys Pro**
### Section: **121**

## Overview
This lab explores the most common POSIX system calls by writing for c programs which focus on different areas

| Program | Category | What it does |
|---|---|---|
|*info*| Information | Prints the curent time (ns), host name, OS name, release, version, hardware type, cpu count and total/free memory |
|*pinfo*| Processes | Prints the priority and scheduling policy for a process |
|*pmod*| Processes | Lowers its own process priority by 10, sleeps 1.84 sec, and exits |
|*finfo*| File Management | Prints a file time, permissions, owner, and last modification time |

## Building
Use Make and Make Clean

## Usage
./info
./pinfo            # info for the current process
./pinfo <pid>      # info for a specific process
./pmod
./finfo <file>
# shell</div>
Task 2 : Linux practice - Operating systems course.</div>

## Project Description:</div>
The task is made of 3 parts: file operations, dynamic libraries, and basic shell.</div>

### **Part A : file operations**</div>

We implemented 2 tools:</div>

1. **CMP -** The tool compare two files, and returns "0" if they are equal, and "1" if not (return INT). Besides returning an int value, the tool prints "equals" or "distinct" with the v flag. It supports the -i flag, meaning "ignore cases," so "AAA" and "aaa" are the same.</div>
</div>

2.  **COPY -** In this tool, it copies a file to a different location and/or name.</div>
The tool returns "0" on success, or "1" on failure (it returns INT).
When a file does not exist, the tool creates a new one. However, if a file already exists, it will not overwrite it.</div>
Using the v flag, the tool will output "*success*" if the file is copied, "*target file exists*" if it is, or "*general failure*" on other problems (in addition to the returned INT value).
In this tool, the f flag (i.e. force) allows overwriting of the target file.</div>
Thats tools implement in </div>

### **Part B : dynamic libraries**</div>
We implemented two coding libraries:</div>
1. `codecA.c:` covers all lowercase characters to uppercase, and all uppercase to lowercase, all other characters will remain unchanged. </div>
2.  `codecB.c:` converts all characters to the next 3rd character (adding a number of 3 to ascii val). </div>
The libraries support `encoding.c` and `decoding.c` methods.</div>

### Part C : basic shell
Shell is a program that provides a user interface to interact with the operating system. It interprets and executes commands entered by the user, and can also run scripts containing a series of commands. </div>  The shell facilitates inter-process communication (IPC) and process management.</div>
This stshell program supports running system commands using `fork`, `exec`, and` wait`, handles stopping a running command with **Ctrl+C** without killing the shell, supports output redirection with **>** and **>>**, allows piping with** | **for up to 2 pipes, and can be stopped using the `exit` command. </div>
</div>

**Description of the order in which these commands/processes are typically used:** </div>

- **fork:** This system call creates a new child process by duplicating the current process.</div> The child process is an exact copy of the parent process, except for a few values, such as the process ID. </div>

- **pipe:** A pipe is a form of IPC that allows two processes to communicate with each other. It creates a unidirectional data channel. One process writes data to the pipe, and the other process reads from it. </div>

- **File descriptor:** A file descriptor is a non-negative integer that serves as an index to access an open file or pipe. When a pipe is created, it returns two file descriptors: one for reading (usually 0) and one for writing (usually 1). </div>

- **dup2:** This system call duplicates a file descriptor, making the new file descriptor point to the same file or pipe as the old one. It is often used to redirect standard input, output, or error streams of a process. </div>

- **execvp:** This function replaces the current process image with a new process image specified by the file path. It also takes an array of arguments to pass to the new program.</div>

- **wait:** This system call suspends the current process until one of its child processes terminates. It allows the parent process to collect the exit status of the child. </div>
</div>

## Building</div>
1. Cloning the repo to local machine: ` git clone` https://github.com/DorHarizi/task2_opsys.git </div>
2. Building all the necessary files & the main programs:  `make all` </div>
3. Export shared libraries : `export LD_LIBRARY_PATH="."` </div>
</div>

## How to run? </div>
- Run the compering files program. : `./cmp<file1> <file2> [-iv]` </div>
- Run the copying files program : `./copy<src> <dest> [-fv]` </div>
- Run the encoder program with codecA : `./encode codecA "text"` </div>
- Run the encoder program with codecB :` ./encode codecB "text"`  </div>
- Run the decoder program with codecA :` ./decode codecA "text"` </div>
- Run the decoder program with codecB : `./decode codecB "text"` </div>
- Run the students shell: `./stshell` </div>
</div>

## Author: </div>
[Batel Cohen Yerushalmi](https://github.com/BatelCohen7 "Batel Cohen Yerushalmi") **&** [Dor Harizi](https://github.com/DorHarizi "Dor Harizi")

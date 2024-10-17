Simple Shell (msh)
This is a simple shell implementation in C, called msh. The shell accepts user commands from the terminal or a file, executes them, and supports basic I/O redirection.

Features
  Command parsing and execution
    Built-in commands:
      cd: Changes the current working directory
      exit: Exits the shell
      quit: Exits the shell
  Command redirection using > (output redirection to a file)
  Execution of commands in a child process
  Error handling for various edge cases
  
Requirements
  C compiler (e.g., gcc)
  Linux/Unix environment
  
How to Build and Run
Compile
To compile the program, use the following command:

gcc -o msh msh.c

Usage
You can run the shell interactively by executing the binary:

./msh

Alternatively, you can run commands from a file by passing the filename as an argument:

./msh commands.txt

Supported Commands
  The shell can execute any valid system command available in the environment, such as ls, echo, etc.
  Supports output redirection using > to redirect the output of a command to a file.
Examples
Running commands interactively:

  msh> ls
  msh> cd /home/user
  msh> ls > output.txt
  msh> exit
  
Running commands from a file:

  ./msh script.txt

Error Handling
  If an error occurs (such as command syntax errors or invalid commands), the shell prints:

  An error has occurred

Exit Conditions
  The shell exits when:
    The user types exit or quit
  End of file (EOF) is reached when reading from a file or standard input
Limitations  
  Only supports single output redirection (>).
  Doesn't handle background processes or advanced piping.
  Commands with more than 32 arguments are not supported.
  
License
  This project is open-source and can be freely used and modified.

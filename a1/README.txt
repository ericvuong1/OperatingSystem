The current mysh runs on mimi.cs.mcgill.ca.
To build the shell, run the following commands:

gcc -c shell.c interpreter.c shellmemory.c
gcc -o mysh shell.o interpreter.o shellmemory.o

Included files are:

interpreter.c
interpreter.h
interpreter.o
shell.c
shell.h
shell.o
shellmemory.c
shellmemory.h
shellmemory.o
testfile.txt
README.txt

Assumptions: when user entering a command, when user inputs more than required arguments, they are ignored and no error is thrown.


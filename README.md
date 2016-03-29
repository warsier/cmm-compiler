# C Minus Minus Compiler
This is a lesson project of Principles of Compilers, which implements a c-- compiler.

## 0. Features
Till now, this compiler has the function of:

* Lexical and syntax analysis for all ```.cmm``` files.
* Output a syntax tree for ```.cmm``` files without lexical or syntax error.
* Output the lexical and syntax errors for the files if they have some.
* The lexical analysis part can also deal with comments without nests, octal and hexademical numbers, and scientific notation.
* Some differences (mistakes?) in the implement of error output and error recovery.

## 1. Compilation and Usage
For the convenience of presentation, I use ```$(DIR)``` to express the root directory of the folder in the following descriptions.

This project uses

* GNU Flex, *version 2.5.35*
* GNU Bison, *version 3.0.2*

to generate lexical and syntax analysis program.

So, as a prerequisite, you should install Flex and Bison.
```Bash
apt-get install flex
apt-get install bison
```

To compile all files, use command
```Bash
make
```

To perform tests on all ```.cmm``` files in folder ```$(DIR)/test```, use command
```Bash
make test
```
The result will be output both to the shell and ```$(DIR)/log.txt```.
You can comment out codes in ```$(DIR)/src/lexical.l``` to test the lexical analysis part.

To debug specific files in ```$(DIR)/test``` with gdb, modify the file directory below command  ```gdb``` in ```Makefile```. Then use command
```Bash
make gdb
```
The result will be output to ```$(DIR)/log.txt```.

To clean up all temporary files generated, use command
```Bash
make clean
```
 	
You can control the indent of the output syntax tree or always enable output regardless of the errors by modify values in file ```$(DIR)/include/syntax_tree.h```.


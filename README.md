# Notation-Converter
Notation Converter Command Line Interface (CLI)
# Overview
A Command-Line Interface (CLI) tool for converting between Infix, Prefix, and Postfix notations using expression trees and stack-based algorithms. This tool accepts a mathematical expression in one of the three notations (Infix, Prefix, or Postfix) and converts it to another. It parses the expression, validates its structure, builds an expression tree if necessary, and performs the conversion based on the selected input and output formats.
Member
•	Lovely Neri Cena
Description
 
How to Build / Compile the Project
Prerequisites
•	GCC or any standard C compiler
•	Unix/Linux terminal or Windows Command Prompt
Build Instructions
1.	Open your terminal and navigate to the file’s directory.
2.	Run the following command to compile the program:
gcc -o not-conv not-conv.c
This will generate an executable named not-conv.
How to Run the Utility
Running the Program
To launch the utility, run the executable in your Linux/Ubuntu terminal:
./not-conv

For windows command prompt, run the following:
not-conv
As you run the executable, it will display help like this:
Usage:
    not-conv --in <NOTATION> --out <NOTATION> "EXPRESSION"

  Options:
    --in <NOTATION>        Input notation: infix | prefix | postfix
    --out <NOTATION>       Output notation: infix | prefix | postfix
    "EXPRESSION"           The expression to convert
                           (enclosed in quotes, with tokens separated by single spaces)
    -h, --help             Show this help message
    -g, --guide            Show detailed usage guide

  Examples:
    not-conv --in infix --out postfix "1 + 2"
    not-conv --in prefix --out infix "+ 1 2"
    not-conv --in postfix --out prefix "1 2 +"
  For a more detailed explanation and examples, use:
    not-conv --guide
From that short overview, You can also access help by running either of the following:
Linux Ubuntu
./not-conv --help

./not-conv --h
Windows
not-conv --help

not-conv --h

For a more detailed explanation, access guide by running either of the following:
Linux Ubuntu
./not-conv --guide

./not-conv --g
Windows
not-conv --guide

not-conv --g
Usage Examples
./not-conv --in infix --out postfix "3 + 5"
  3 5 +

./not-conv --in infix --out prefix "3 + 5 * 2"
  + 3 * 5 2

./not-conv --in prefix --out infix "* + 3 5 2"
  ( 3 + 5 ) * 2

./not-conv --in prefix --out postfix "/ * + 8 2 - 5 3 2"
  8 2 + 5 3 - * 2 /

./not-conv --in postfix --out prefix "8 2 + 5 3 - * 2 /"
  / * + 8 2 - 5 3 2

./not-conv --in postfix --out infix "8 2 + 5 3 - * 2 /"
  ( ( 8 + 2 ) * ( 5 - 3 ) ) / 2
Design Choices & Limitations
Design Choices
•	The program made use of command line arguments to get the users input for conversion. Manual memory management with an allocation of 256 was chosen for function and result string storage and dynamic memory management for constant strings [input, output, and notationCheck] and expression. The program is made up of library and user-defined functions. User-defined functions handle the section of options [help, guide] and the conversion [buildTreeFromPostfix, buildTreeFromInfix, shuntingYard]. 
•	Prefix to Infix and Postfix
o	In converting prefix notations to Infix, the spaces between the operators and operands are removed first, then an expression tree is built from prefix, which is then traversed in inorder. In terms of prefix to postfix, the similar expression tree is traversed postorder.
•	Infix to Prefix 
o	In converting infix to postfix, a shunting yard algorithm was used. The goal of the shunting yard was to make use of stacks to convert an infix expression into postfix. In converting to prefix, a shunting yard algorithm was also used, then the postfix expression formed has its spaces removed for making an expression tree. The expression tree is traversed in preorder to obtain the prefix notation. 
•	Postfix to Prefix 
o	For postfix to prefix and infix, an expression tree based on the postfix expression was built after removing the spaces in between. And it was traversed in preorder to obtain the prefix notation or inorder for infix notation.
Known Limitations
The notation converter is limited from the following:
•	Only handles single digit operands (0 - 9 ) and MDAS operators (+ - * /)
•	
•	Generalized error handling on malformed expressions and commands with less than 5 arguments 
Language & Dependencies
Programming Language
•	C (C99 Standard)
Dependencies
•	String.h
•	Stdlib.h
•	Ctype.h

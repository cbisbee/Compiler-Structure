# Compiler-Structure
Repo for CSCI 450 - Compiler Structure, Colorado Mesa University Fall 2017

# Assignment 1: UTF-8 converting program
- This assignment is to make a program which reads a UTF-8 encoded file and returns the resulting code points for all of the characters in the file
- The code and test files for this branch are in the URL https://github.com/cbisbee/Compiler-Structure/tree/Assignment1/UTF8_Decoder/UTF8_Decoder 
- There is a collection of test input and output files at the URL https://github.com/cbisbee/Compiler-Structure/tree/Assignment1/UTF8_Decoder/Debug/TestFiles

# Assignment 2: Build a lexer/parser/ast for a simple language
- In this assignment we were to pick out a simple language and create a lexical scanner and parser for the language. I chose to do a very simple subset of Keyhole Markup Language (KML) which allows users to define placemarkers and give those placemarkers names, descriptions, and coordinates.
- The branch can be found at https://github.com/cbisbee/Compiler-Structure/tree/Assignment2
- The source files for this project can be found at https://github.com/cbisbee/Compiler-Structure/tree/Assignment2/Assignment2KML/SourceFiles
- The test files for this project can be found at https://github.com/cbisbee/Compiler-Structure/tree/Assignment2/Assignment2KML/TestFiles

# Assignment 3: Build a lexer/parser/ast for a simple language using Peg/Leg
- In this assignment I implemented the same scanner and parser that I did for assignment 2 but instead of using Flex and Bison I used Leg as my tool to define tokens and grammars.
- The branch can be found at https://github.com/cbisbee/Compiler-Structure/tree/PegLegKML
- The source files for this project can be found at https://github.com/cbisbee/Compiler-Structure/tree/PegLegKML/SourceFiles
- The test files for this project can be found at https://github.com/cbisbee/Compiler-Structure/tree/PegLegKML/TestFiles

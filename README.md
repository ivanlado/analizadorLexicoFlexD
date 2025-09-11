# Flex Lexical Analyzer

This repository contains a simple **lexical analyzer** (also known as a lexer or scanner) for programming language D, built as part of a compiler construction project for the subject Compilers and Interpreters. 

## What is a compiler?

A **compiler** is a program that translates code written in one programming language (like C, Python, Java or D) into another language, usually machine code that a computer can execute.

### 🛠️ Stages of a Compiler

Compilers work in several stages:

1. **Lexical Analysis (Lexer/Scanner):**
	- Breaks the input code into tokens (words, numbers, symbols).
	- Removes whitespace and comments.
	- Example: Turns `int x = 5;` into tokens: `int`, `x`, `=`, `5`, `;`.

2. **Syntax Analysis (Parser):**
	- Checks if the sequence of tokens follows the grammar rules of the language.
	- Builds a tree structure (parse tree) representing the code.

3. **Semantic Analysis:**
	- Ensures the code makes sense (e.g., variables are declared before use).

4. **Intermediate Code Generation:**
	- Converts the parse tree into an intermediate representation.

5. **Optimization:**
	- Improves the intermediate code for efficiency.

6. **Code Generation:**
	- Produces the final machine code or bytecode.

This project focuses on the **first stage: Lexical Analysis**.

## 🎯 Purpose of this Project

The goal is to implement a **lexical analyzer** using Flex (Fast Lexical Analyzer Generator). The analyzer reads source code and breaks it into tokens, which are the building blocks for further compilation stages. This is a fundamental step in building a compiler or interpreter for any programming language.

## 🚀 How to Compile and Run


### Steps
1. **Generate the C code from the Flex file:**
	```sh
	flex lex.l
	```
2. **Compile the generated C code:**
	```sh
	gcc lex.yy.c -o ejecutable.out
	```
3. **Run the analyzer:**
	```sh
	./ejecutable.out < input_file.txt
	```
	Replace `input_file.txt` with your source code file.

## 📝 Project Structure

- `lex.l` — The Flex specification file (main lexer definition)
- `lex.yy.c` — Generated C code from Flex
- `main.c` — Main program (if present)
- `makefile` — Automates the build process
- `errores.c`, `lista.c`, `tabla_hash_encadenamiento.c`, etc. — Helper modules
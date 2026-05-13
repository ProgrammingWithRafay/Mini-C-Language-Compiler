# MiniC Compiler

A Mini C Compiler built in **C** using **Flex** (lexer) and **Bison** (parser) that takes a subset of C source code and processes it through a full 7-phase compilation pipeline — from raw tokens all the way to pseudo-assembly output and interpreted execution.

---

## Features

- **7-Phase Compilation Pipeline** — each phase is clearly separated and produces visible output
- **Lexical Analysis** — tokenizes the source file and prints a formatted token table
- **Syntax Analysis** — builds and prints an ASCII Abstract Syntax Tree (AST)
- **Semantic Analysis** — validates types, checks declarations, and outputs a symbol table
- **Intermediate Code Generation** — generates Three-Address Code (TAC)
- **Code Optimization** — applies Constant Folding and Dead Code Elimination to the TAC
- **Target Code Generation** — translates optimized TAC into x86 pseudo-assembly instructions
- **TAC Interpreter** — executes the optimized TAC and prints the program's actual output

---

## Project Structure

```
MiniC_Compiler/
├── src/
│   ├── main.c           # Entry point; orchestrates all 7 phases
│   ├── lexer.l          # Flex lexer definition
│   ├── parser.y         # Bison grammar and AST construction
│   ├── ast.h            # AST node definitions
│   ├── semantic.c/.h    # Semantic analysis and symbol table
│   ├── codegen.c/.h     # TAC intermediate code generation
│   ├── tac.h            # TAC instruction definitions
│   ├── optimizer.c/.h   # Constant folding & dead code elimination
│   ├── target.c/.h      # x86 pseudo-assembly code generation
│   └── interpreter.c/.h # TAC interpreter
├── tests/
│   ├── test1.c          # Basic arithmetic & optimizations
│   ├── test2.c          # Basic if-else & variables
│   ├── test3.c          # For loops & basic functions
│   ├── test4.c          # Edge case: nested if-else
│   ├── test5.c          # Edge case: function calls in expressions
│   └── test6.c          # Edge case: missing semicolons (syntax error)
└── Makefile
```

---

## Prerequisites

- **Windows 11**
- **MinGW** with GCC installed and added to `PATH`
- **Flex & Bison** installed and added to `PATH`
- **Make** installed and added to `PATH`

---

## Building

1. Open a terminal or Command Prompt in the `MiniC_Compiler/` directory.
2. Add MinGW to your session's PATH (if not already set globally):
   ```powershell
   $env:PATH += ";C:\MinGW\bin;C:\MinGW\msys\1.0\bin"
   ```
3. Run:
   ```bash
   make
   ```
   This will produce `compiler.exe`.

To clean all generated files:
```bash
make clean
```

---

## Running

Pass a `.c` source file as an argument to the compiler:

```bash
.\compiler.exe tests/test1.c
```

### Available Test Cases

| Command | Description |
|---|---|
| `.\compiler.exe tests/test1.c` | Basic arithmetic & optimizations |
| `.\compiler.exe tests/test2.c` | Basic if-else & variables |
| `.\compiler.exe tests/test3.c` | For loops & basic functions |
| `.\compiler.exe tests/test4.c` | Edge case: nested if-else |
| `.\compiler.exe tests/test5.c` | Edge case: function calls in expressions |
| `.\compiler.exe tests/test6.c` | Edge case: missing semicolons (syntax error) |

---

## Expected Output

For every run, the compiler prints output for all 7 phases:

```
===== PHASE 1: LEXICAL ANALYSIS =====
Token Value     | Token Type      | Line No.
--------------------------------------------------
...

===== PHASE 2: SYNTAX ANALYSIS =====
...AST tree...

===== PHASE 3: SEMANTIC ANALYSIS =====
...symbol table...

===== PHASE 4: INTERMEDIATE CODE GENERATION =====
...TAC instructions...

===== PHASE 5: CODE OPTIMIZATION =====
...TAC before and after optimization...

===== PHASE 6: TARGET CODE GENERATION =====
...x86 pseudo-assembly...

===== PHASE 7: PROGRAM OUTPUT =====
...interpreted result...
```

---

## Technologies Used

- **C** — compiler implementation language
- **Flex** — lexical analyzer generator
- **Bison** — parser generator (LALR)
- **GCC / MinGW** — compilation toolchain
- **Make** — build automation

---

## License

This project was developed as an academic compiler construction project. Feel free to use it for learning purposes.

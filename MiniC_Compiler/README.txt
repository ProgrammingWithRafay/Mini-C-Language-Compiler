Mini C Compiler Project.

Prerequisites:
- Windows 11
- MinGW with GCC installed and added to PATH
- Flex & Bison installed and added to PATH
- Make installed and added to PATH

How to Compile:
1. Open terminal/cmd in the MiniC_Compiler directory.
2. Run the command:
$env:PATH += ";C:\MinGW\bin;C:\MinGW\msys\1.0\bin"
   make
3. This will generate `compiler.exe`.

How to Run Tests:
Run the compiler by passing the test file as an argument:
   .\compiler.exe tests/test1.c  (Basic arithmetic & optimizations)
   .\compiler.exe tests/test2.c  (Basic if-else & variables)
   .\compiler.exe tests/test3.c  (For loops & basic functions)
   .\compiler.exe tests/test4.c  (Edge case: nested if-else)
   .\compiler.exe tests/test5.c  (Edge case: function calls in expressions)
   .\compiler.exe tests/test6.c  (Edge case: missing semicolons syntax error)

How to Clean:
Run `make clean` to remove all generated object files and executables.

What Output to Expect:
For each run, the compiler will display output for 7 phases:
1. Lexical Analysis: A table of identified tokens.
2. Syntax Analysis: An ASCII tree representation of the AST.
3. Semantic Analysis: A symbol table and validation status.
4. Intermediate Code Generation: Three-Address Code (TAC).
5. Code Optimization: TAC before and after Constant Folding and Dead Code Elimination.
6. Target Code Generation: x86 pseudo-assembly instructions.
7. Program Output: The actual executed result using the TAC interpreter.

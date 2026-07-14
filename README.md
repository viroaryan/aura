# Aura Language v2.0 🚀

Aura is a modern, blazing fast, 100% self-hosted systems programming language. It natively compiles directly to pure `x86-64` assembly without any C/C++ wrappers or virtual machines. 

## Features
- **Zero C++ Dependencies:** Fully bootstrapped and self-hosted. 
- **Native AOT Compilation:** Compiles directly to GAS Intel syntax assembly.
- **Native MS ABI Interop:** Effortlessly link against C libraries (`msvcrt`, `ws2_32`, `kernel32`) with zero-overhead FFI.
- **Object-Oriented:** Supports native `class` declarations, dynamic heap allocation, and native method dispatch via Global Property Offsets.
- **Dynamic Arrays & Strings:** First-class support for dynamically allocated arrays (`[]`) and native strings (`.data` segment injection).

## Standard Libraries (`std/`)
Aura includes powerful standard libraries natively built-in:
- **`std/ml.aura`**: A high-performance Object-Oriented Tensor library for Neural Networks.
- **`std/web.aura`**: A raw WinSock (`ws2_32.dll`) HTTP web server library.
- **`std/io.aura`**: Native Windows File I/O (`kernel32.dll`).

## Quick Start
```bash
# Compile a script
aurac.exe myscript.aura

# Assemble and run
gcc main.s -o myapp.exe
./myapp.exe
```

## Architecture
Aura is designed for uncompromising performance. It achieves dynamic language flexibility (like Python) with systems language speeds (like C) by mapping OOP Hash Maps into dense memory structures and compiling dynamic array indexing into raw `x86-64` pointer arithmetic.

## License
MIT License

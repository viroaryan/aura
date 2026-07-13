# 🚀 The Aura Programming Language
[![Language](https://img.shields.github.io/badge/Language-Aura-blueviolet)](https://github.com/)
[![License](https://img.shields.github.io/badge/License-MIT-blue)](LICENSE)
[![Compiler](https://img.shields.github.io/badge/Compiler-C%2B%2B17-orange)](aurac.cpp)
[![Platform](https://img.shields.github.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20Android-green)](#)

Aura is a high-performance, developer-friendly, compiled programming language designed to combine **Python's ease of readability** with **C++'s execution speed**. 

Aura compiles directly to optimized, intermediate `C++20` source code, which is then statically compiled using **LLVM-MinGW (Clang/LLVM 22.1.8)** into standalone, zero-dependency machine code binaries.

---

## ✨ Core Features

* **⚡ Ultra-Fast Native Compilation**: Compiles down to native machine assembly instructions with 0% interpreter or JVM overhead.
* **🐍 Simple & Readable Syntax**: Clean, boilerplate-free design (no namespaces, std prefix, or header imports).
* **📦 Dynamic Arrays & Lists**: Supports array declarations, element mutation, and length querying out of the box.
* **🏢 Structs (Object-Oriented Programming)**: Implements C++17 Class Template Argument Deduction (CTAD) templates under the hood for clean object models at zero runtime cost.
* **💾 Built-in System I/O**: Access the host filesystem natively (`readFile`, `writeFile`, `appendFile`) and read inputs via `input()`.
* **🌐 Native Web Serving**: Launch dynamic web servers directly from Aura scripts using cross-platform socket bindings.
* **📱 Android NDK & JNI Ready**: Easily compile Aura scripts into `.so` shared libraries to link directly with Kotlin/Java in Android Studio.

---

## 🚀 Quickstart Guide

### 1. Installation

Aura compiles using any local C++ compiler (Clang++ or G++).

1. Download **`aura-sdk.zip`** from the [Releases](https://github.com/) page and extract it.
2. Add the absolute path to the `bin/` folder (e.g. `C:\aura-sdk\bin`) to your System's environment **PATH** variable.
3. Verify installation:
   ```bash
   aurac
   ```

### 2. VS Code Extension
To enable syntax coloring:
1. Copy the folder `extension/aura-lang-0.1.0` from the SDK.
2. Paste it in your VS Code extensions folder: `C:\Users\<Username>\.vscode\extensions`.
3. Restart VS Code.

---

## 💻 Code Examples

### 1. Variables & Concatenation
Aura features implicit type coercion, making it easy to concatenate text, integers, and decimals:
```aura
fn main() {
    var name = "Aura Engine"
    var version = 1.0
    print("Welcome to " + name + " v" + version)
}
```

### 2. Arrays & Loops
```aura
fn main() {
    var numbers = [10, 20, 30, 40]
    numbers[2] = 99
    
    var i = 0
    var len = size(numbers)
    while i < len {
        print("Index " + i + ": " + numbers[i])
        i = i + 1
    }
}
```

### 3. OOP Structs
```aura
struct User {
    name
    age
    isDeveloper
}

fn main() {
    var dev = User("Aryan", 25, true)
    print("Name: " + dev.name)
    dev.age = 26
    print("New Age: " + dev.age)
}
```

### 4. Dynamic Web Server
```aura
fn main() {
    var title = "My Aura Site"
    var html = "<!DOCTYPE html><html><head><title>" + title + "</title></head><body><h1>Hello World from Aura!</h1></body></html>"
    
    // Serves dynamically generated HTML on port 8080
    startServer(8080, html)
}
```

---

## ⚙️ Compilation & Build Flow
To compile any `.aura` file:
```bash
aurac main.aura
```
This generates:
1. `main.cpp` - Optimized, clean intermediate C++20 code.
2. `main.exe` - Highly optimized, standalone, static native binary.

To run:
```bash
.\main.exe
```

---

## 📜 Open Source License

Aura is distributed under the **MIT License**. This means you are completely free to use, modify, distribute, and sell applications built with Aura, with zero licensing costs. See [LICENSE](LICENSE) for full legal text.

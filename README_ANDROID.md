# 📱 Aura Android Integration Guide (JNI & NDK)

Since Aura compiles directly to optimized C++ with `extern "C"` support, you can compile Aura code into native shared libraries (`.so` files) and load them directly into your Android Studio projects using the **Android NDK (Native Development Kit)**.

---

## 🛠️ Step-by-Step Guide

### Step 1: Write JNI-Compatible Aura Code
Android JNI requires native functions to follow a specific naming pattern: `Java_PackageName_ActivityName_FunctionName`. 

Create a file named `native-lib.aura`:
```aura
/* 
   native-lib.aura
   This function can be called directly from Kotlin in your MainActivity!
*/

extern fn Java_com_example_auraapp_MainActivity_calculateSpeed(distance, time) {
    if time == 0 {
        return 0
    }
    return distance / time
}
```

### Step 2: Compile to C++
Compile your Aura code to C++ using `aurac.exe`:
```bash
.\aurac.exe native-lib.aura
```
This generates `native-lib.cpp` containing:
```cpp
extern "C" double Java_com_example_auraapp_MainActivity_calculateSpeed(double distance, double time) {
    if ((time == 0)) {
        return 0;
    }
    return (distance / time);
}
```

### Step 3: Compile for Android (NDK)
To compile `native-lib.cpp` into a shared library for Android devices (ARM64 architecture), run the Clang++ compiler from your Android NDK installation directory:

```bash
# Locate your NDK toolchain Clang compiler
# (Standard Windows Path: C:\Users\<Username>\AppData\Local\Android\Sdk\ndk\<version>\toolchains\llvm\prebuilt\windows-x86_64\bin)

aarch64-linux-android30-clang++ -O3 -shared -fPIC -std=c++20 native-lib.cpp -o libnative-lib.so
```
*(This generates `libnative-lib.so` which is the optimized native binary library for Android).*

### Step 4: Import into Android Studio
1. Open your Android Studio project.
2. Place the generated `libnative-lib.so` file in the folder:
   `app/src/main/jniLibs/arm64-v8a/libnative-lib.so`

### Step 5: Call Aura from Kotlin
In your Kotlin `MainActivity.kt` file, load the library and declare the external function:

```kotlin
package com.example.auraapp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    // Load the native library compiled from Aura
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    // Declare the external Aura function
    private external fun calculateSpeed(distance: Double, time: Double): Double

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Call your Aura function directly!
        val speed = calculateSpeed(100.0, 4.0) // Returns 25.0
        
        findViewById<TextView>(R.id.resultText).text = "Speed from Aura: $speed km/h"
    }
}
```

---

## ⚡ Benefits of using Aura for Android
1. **Ultra-Fast Code**: Runs directly on the CPU (native speed), bypassing the JVM garbage collector and runtime interpreter.
2. **Battery & Memory Efficient**: Perfect for heavy tasks like physics math, game mechanics, image processing, or database filters.
3. **No C++ Complexity**: Write clean Aura code instead of manual C++ pointers and configurations.

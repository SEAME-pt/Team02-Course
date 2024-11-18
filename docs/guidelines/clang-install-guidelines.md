### **Step 1: Identify Your System**
1. **Operating System**:
   - Windows: Use `LLVM-18.1.8-win64.exe` (or `win32` for 32-bit systems).
   - Linux: Choose the pre-built binary matching your distribution, e.g.:
     - `clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04.tar.xz` for Ubuntu 18.04.
   - macOS: Use `clang+llvm-18.1.8-arm64-apple-macos11.tar.xz` for Apple Silicon.
2. **Architecture**:
   - Ensure the file matches your hardware (e.g., x86_64, arm64, etc.).

---

### **Step 2: Download the Pre-Built Binary**
1. Go to the [LLVM GitHub Releases Page](https://github.com/llvm/llvm-project/releases).
2. Find version **18.1.8**.
3. Download the relevant pre-built binary for your OS.

---

### **Step 3: Install LLVM**
#### **On Windows**:
1. Run the installer (`LLVM-18.1.8-win64.exe`) and follow the prompts.
2. During installation, enable the option to add LLVM to your system’s `PATH`.
3. Verify installation:
   ```cmd
   clang-format --version
   ```

#### **On Linux**:
1. Extract the downloaded archive:
   ```bash
   tar -xf clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04.tar.xz
   ```
2. Move the binaries to a directory in your `PATH`:
   ```bash
   sudo mv clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04 /usr/local/llvm-18.1.8
   export PATH="/usr/local/llvm-18.1.8/bin:$PATH"
   ```
3. Verify installation:
   ```bash
   clang-format --version
   ```

#### **On macOS**:
1. Extract the archive:
   ```bash
   tar -xf clang+llvm-18.1.8-arm64-apple-macos11.tar.xz
   ```
2. Add the extracted folder to your `PATH`:
   ```bash
   export PATH="/path/to/llvm/bin:$PATH"
   ```
3. Verify installation:
   ```bash
   clang-format --version
   ```

---

### **Step 4: Install clang-format Xavier extension**


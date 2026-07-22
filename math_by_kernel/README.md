# Math by Kernel - Linux Kernel Module Inter-Communication

This project demonstrates **inter-module communication** in the Linux kernel using exported symbols. It consists of two kernel modules: an **exporter** that provides a calculation function, and an **importer** that uses that function from another module.

## 📁 Project Structure

```
math_by_kernel/
├── calc_exporter.c    # Module that exports the calculation function
├── calc_importer.c    # Module that imports and uses the exported function
├── Makefile           # Build configuration for both modules
└── README.md          # This documentation
```

## 🔍 Overview

### What This Project Demonstrates

This project showcases how Linux kernel modules can communicate with each other through **symbol exporting**. The `calc_exporter` module exports a function called `perform_calculation()`, which the `calc_importer` module can call even though it resides in a different kernel module.

### Key Concepts

1. **Symbol Exporting**: Using `EXPORT_SYMBOL()` to make functions available to other modules
2. **External Function Declaration**: Using `extern` to declare functions from other modules
3. **Module Parameters**: Passing runtime arguments to kernel modules
4. **Module Dependencies**: Understanding how modules depend on each other
5. **Kernel Logging**: Using `printk()` for debugging and output

---

## 🛠️ Code Execution

### Step 1: Build the Modules

```bash
cd math_by_kernel
make
```

This compiles both `calc_exporter.ko` and `calc_importer.ko` kernel modules.

### Step 2: Load the Exporter Module

```bash
sudo insmod calc_exporter.ko a=24 b=20 op_str="-"
```

**What happens:**
- The exporter module loads with parameters `a=24`, `b=20`, and `op_str="-"`
- It performs an internal test calculation: `24 - 20 = 4`
- It exports the symbol `perform_calculation` for other modules to use

**Check kernel logs:**
```bash
sudo dmesg | tail -n 3
```

**Output:**
```
[24348.119952] [EXPORTER] Module unloaded. Symbol removed.
[24354.566582] [EXPORTER] Loaded. Internal test: 24 - 20 = 4
[24354.566588] [EXPORTER] Module loaded. Symbol 'perform_calculation' exported.
```

### Step 3: Load the Importer Module

```bash
sudo insmod calc_importer.ko x=50 y=12 op_str="/"
```

**What happens:**
- The importer module loads with parameters `x=50`, `y=12`, and `op_str="/"`
- It calls the `perform_calculation()` function from the exporter module
- The exporter calculates `50 / 12 = 4` (integer division)

**Check kernel logs:**
```bash
sudo dmesg | tail -n 6
```

**Output:**
```
[24348.119952] [EXPORTER] Module unloaded. Symbol removed.
[24354.566582] [EXPORTER] Loaded. Internal test: 24 - 20 = 4
[24354.566588] [EXPORTER] Module loaded. Symbol 'perform_calculation' exported.
[24378.736469] [IMPORTER] Module loaded.
[24378.736474] [IMPORTER] Requesting Calculation from Exporter: 50 / 12 = ?
[24378.736477] [IMPORTER] Success! Exporter returned: 4
```

### Step 4: Unload Modules (Order Matters!)

```bash
sudo rmmod calc_importer
sudo rmmod calc_exporter
```

**Important:** You must unload the importer **before** the exporter because the importer depends on the exporter's symbol.

**If you try to remove the exporter first:**
```bash
sudo rmmod calc_exporter
# Output: rmmod: ERROR: Module calc_exporter is in use by: calc_importer
```

---

## 🧠 Why and How It Works

### The Problem: Module Isolation

By default, Linux kernel modules are isolated. Functions defined in one module are not visible to other modules. This is a safety feature to prevent naming conflicts and ensure module independence.

### The Solution: Symbol Exporting

Linux provides a mechanism for modules to explicitly share functions through **symbol tables**.

### How It Works - Step by Step

#### 1. **Exporter Side (`calc_exporter.c`)**

**Function Definition:**
```c
int perform_calculation(int num1, int num2, const char *op) {
    // ... calculation logic ...
    return result;
}
EXPORT_SYMBOL(perform_calculation);
```

**Key Points:**
- `EXPORT_SYMBOL(perform_calculation)` adds this function to the kernel's symbol table
- Other modules can now resolve references to this symbol
- The function becomes part of the kernel's global namespace (while the module is loaded)

**Module Initialization:**
```c
static int __init exporter_init(void) {
    int result = perform_calculation(a, b, op_str);
    printk(KERN_INFO "[EXPORTER] Loaded. Internal test: %d %c %d = %d\n", a, op_str[0], b, result);
    printk(KERN_INFO "[EXPORTER] Module loaded. Symbol 'perform_calculation' exported.\n");
    return 0;
}
```

#### 2. **Importer Side (`calc_importer.c`)**

**External Declaration:**
```c
extern int perform_calculation(int num1, int num2, const char *op);
```

**Key Points:**
- `extern` tells the compiler that this function exists elsewhere
- The actual implementation will be resolved at module load time
- No need to include header files from the exporter module

**Using the Imported Function:**
```c
static int __init importer_init(void) {
    printk(KERN_INFO "[IMPORTER] Module loaded.\n");
    printk(KERN_INFO "[IMPORTER] Requesting Calculation from Exporter: %d %c %d = ?\n", x, op_str[0], y);
    
    // Call the function from the other module!
    result = perform_calculation(x, y, op_str);
    
    printk(KERN_INFO "[IMPORTER] Success! Exporter returned: %d\n", result);
    return 0;
}
```

#### 3. **Kernel's Role**

When `calc_importer.ko` is loaded:

1. **Symbol Resolution**: The kernel checks if `perform_calculation` is available in its symbol table
2. **Dependency Check**: The kernel sees that `calc_importer` needs a symbol from `calc_exporter`
3. **Reference Counting**: The kernel increments the reference count of `calc_exporter`
4. **Linking**: The kernel patches the importer's code to point to the exporter's function address

---

## 📚 Key Insights and Definitions

### 1. **EXPORT_SYMBOL()**

**Definition:** A macro that makes a kernel symbol (function or variable) available to other modules.

**How it works:**
- Adds the symbol to the kernel's symbol table
- The symbol remains exported until the module is unloaded
- Multiple modules can use the same exported symbol

**Syntax:**
```c
EXPORT_SYMBOL(symbol_name);
```

### 2. **extern Keyword**

**Definition:** A storage class specifier that declares a symbol without defining it.

**How it works:**
- Tells the compiler "this function exists somewhere"
- Defers symbol resolution to link/load time
- Essential for cross-module function calls

**Syntax:**
```c
extern return_type function_name(parameters);
```

### 3. **Module Parameters**

**Definition:** Runtime-configurable variables passed to kernel modules during insertion.

**How it works:**
- Defined using `module_param(name, type, permissions)`
- Types: `int`, `charp` (char pointer), `bool`, etc.
- Permissions: `0644` allows reading/writing via `/sys/module/`

**Example:**
```c
static int a = 0;
module_param(a, int, 0644);
MODULE_PARM_DESC(a, "Description of parameter a");
```

**Usage:**
```bash
insmod module.ko a=24 b=20 op_str="-"
```

### 4. **printk() - Kernel Logging**

**Definition:** The kernel-space equivalent of `printf()` for logging messages.

**Log Levels:**
- `KERN_EMERG`: System is unusable
- `KERN_ALERT`: Action required immediately
- `KERN_CRIT`: Critical conditions
- `KERN_ERR`: Error conditions
- `KERN_WARNING`: Warning messages
- `KERN_NOTICE`: Normal but significant
- `KERN_INFO`: Informational messages
- `KERN_DEBUG`: Debug-level messages

**Example:**
```c
printk(KERN_INFO "[MODULE] Informational message\n");
printk(KERN_ERR "[MODULE] Error message\n");
```

### 5. **Module Reference Counting**

**Definition:** A mechanism that tracks how many modules or processes are using a module.

**Why it matters:**
- Prevents unloading modules that are in use
- Ensures system stability
- Creates implicit dependencies between modules

**In this project:**
- When `calc_importer` loads, it increases `calc_exporter`'s reference count
- `calc_exporter` cannot be unloaded until `calc_importer` is removed first

### 6. **Integer Division in C**

**Definition:** Division operation where the result is truncated to an integer.

**In this project:**
```c
50 / 12 = 4  // Not 4.166... but 4 (truncated)
```

**Important:** The calculator performs integer arithmetic, so fractional results are lost.

---

## ⚠️ Important Considerations

### 1. **Load Order Matters**

Always load the exporter before the importer:
```bash
# ✅ Correct
sudo insmod calc_exporter.ko a=24 b=20 op_str="-"
sudo insmod calc_importer.ko x=50 y=12 op_str="/"

# ❌ Wrong (importer will fail to load)
sudo insmod calc_importer.ko x=50 y=12 op_str="/"
sudo insmod calc_exporter.ko a=24 b=20 op_str="-"
```

### 2. **Unload Order Matters**

Always unload the importer before the exporter:
```bash
# ✅ Correct
sudo rmmod calc_importer
sudo rmmod calc_exporter

# ❌ Wrong (will get "module in use" error)
sudo rmmod calc_exporter
```

### 3. **Symbol Conflicts**

If multiple modules export the same symbol name, conflicts can occur. Use unique names or namespaces in production code.

### 4. **Division by Zero**

The exporter handles division by zero gracefully:
```c
case '/': 
    if (num2 == 0) {
        printk(KERN_ERR "[EXPORTER] Error: Division by zero!\n");
        return -1;
    }
    result = num1 / num2;
    break;
```

---

## 🔧 Building and Cleaning

### Build Both Modules
```bash
make
```

### Clean Build Artifacts
```bash
make clean
```

### View Module Information
```bash
modinfo calc_exporter.ko
modinfo calc_importer.ko
```

---

## 🎯 Learning Outcomes

After studying this project, you should understand:

1. ✅ How to export symbols from a kernel module
2. ✅ How to import and use symbols from another module
3. ✅ The role of `EXPORT_SYMBOL()` and `extern`
4. ✅ How kernel module dependencies work
5. ✅ How to pass parameters to kernel modules
6. ✅ Proper module loading and unloading order
7. ✅ Kernel logging with `printk()`
8. ✅ Module reference counting and its implications

---

## 📝 License

GPL (as specified in the module source files)

## 👨‍💻 Author

Asif Ahamed

---

## 📖 Further Reading

- [Linux Kernel Module Programming Guide](https://tldp.org/LDP/lkmpg/html/x121.html)
- [Kernel Documentation - Exported Symbols](https://www.kernel.org/doc/html/latest/driver-api/basics.html)
- [Understanding EXPORT_SYMBOL and EXPORT_SYMBOL_GPL](https://lwn.net/Articles/26688/)

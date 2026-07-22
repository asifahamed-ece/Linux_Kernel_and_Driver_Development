# Exporting Symbols - Inter-Module Communication

## 🎯 Overview
Demonstrates how kernel modules can share functions with each other using `EXPORT_SYMBOL()` and `extern` declarations. One module exports a function, and another imports and uses it.

## 📁 Files
- `math_exporter.c` - Exports the `add_numbers()` function
- `math_importer.c` - Imports and uses `add_numbers()`
- `Makefile` - Build configuration

## 🔑 Key Concepts

### 1. Exporting a Function (Exporter Module)
```c
// Define the function
int add_numbers(int a, int b) {
    printk(KERN_INFO "Adding %d and %d\n", a, b);
    return a + b;
}

// Export to kernel's global symbol table
EXPORT_SYMBOL(add_numbers);
```
- **`EXPORT_SYMBOL()`**: Makes function available to other modules
- Function becomes part of kernel's global symbol table

### 2. Importing a Function (Importer Module)
```c
// Declare external function
extern int add_numbers(int a, int b);

// Use it like a local function
int result = add_numbers(20, 12);
printk(KERN_INFO "Result: %d\n", result);
```
- **`extern`**: Tells compiler function exists in another module
- No special syntax needed to call it

## 🛠️ Build & Run

### Build Both Modules
```bash
make
```

### Load Order Matters!
**1. Load Exporter First:**
```bash
sudo insmod math_exporter.ko
# Output: [EXPORTER] Module Loaded! Symbol 'add_numbers' Exported.
```

**2. Load Importer Second:**
```bash
sudo insmod math_importer.ko
# Output: [IMPORTER] Result from Exporter Module: 32
```

### View Output
```bash
sudo dmesg | tail -n 5
```

### Unload Order (Reverse!)
**1. Remove Importer First:**
```bash
sudo rmmod math_importer
```

**2. Then Remove Exporter:**
```bash
sudo rmmod math_exporter
```

⚠️ **If you try to remove exporter first:**
```bash
sudo rmmod math_exporter
# Error: Module math_exporter is in use by: math_importer
```

## 📋 Quick Reference

| Macro/Keyword | Purpose |
|---------------|---------|
| `EXPORT_SYMBOL(func)` | Export function to kernel symbol table |
| `extern` | Declare external function from another module |
| `insmod exporter.ko` | Load exporter module FIRST |
| `insmod importer.ko` | Load importer module SECOND |
| `rmmod importer` | Unload importer FIRST |
| `rmmod exporter` | Unload exporter LAST |
| `lsmod` | Check module dependencies |

## ⚠️ Important Notes
- **Load order matters**: Exporter must load before importer
- **Unload order matters**: Importer must unload before exporter
- Kernel tracks dependencies automatically
- If exporter is removed while importer is using it, system crashes
- Use `lsmod` to check which modules depend on others

## ✅ Learning Checklist
- [ ] Understand `EXPORT_SYMBOL()` purpose
- [ ] Know how to use `extern` keyword
- [ ] Understand load/unload order requirements
- [ ] Can check module dependencies with `lsmod`
- [ ] Know why symbol exporting is needed

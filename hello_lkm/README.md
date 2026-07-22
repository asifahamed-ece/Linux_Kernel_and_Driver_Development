# Hello LKM - First Loadable Kernel Module

## 🎯 Overview
A simple "Hello World" kernel module that demonstrates the basic structure of Loadable Kernel Modules (LKM). It prints messages to the kernel log when loaded and unloaded.

## 📁 Files
- `hello_lkm.c` - Main module source code
- `Makefile` - Build configuration

## 🔑 Key Functions & Macros

### 1. Required Headers
```c
#include <linux/module.h>   // All kernel modules need this
#include <linux/init.h>     // For __init, __exit macros
#include <linux/kernel.h>   // For printk() and log levels
```

### 2. Module Metadata (Required)
```c
MODULE_LICENSE("GPL");           // License type (required for kernel symbols)
MODULE_AUTHOR("Your Name");      // Author info
MODULE_DESCRIPTION("Description"); // What the module does
MODULE_VERSION("1.0");           // Version string
```

### 3. Init Function (Constructor)
```c
static int __init hello_lkm_init(void) {
    printk(KERN_INFO "[HELLO_LKM] Module Loaded!\n");
    return 0;  // 0 = success, negative = error
}
```
- **`__init`**: Memory freed after module loads (saves RAM)
- **`printk()`**: Kernel's printf - writes to kernel ring buffer (view with `dmesg`)
- **`KERN_INFO`**: Log level (others: `KERN_ERR`, `KERN_WARNING`, `KERN_DEBUG`)
- **Returns 0**: Success. Negative values indicate errors

### 4. Exit Function (Destructor)
```c
static void __exit hello_lkm_exit(void) {
    printk(KERN_INFO "[HELLO_LKM] Goodbye!\n");
}
```
- **`__exit`**: Cleanup code called when module is removed

### 5. Register Entry/Exit Points
```c
module_init(hello_lkm_init);    // Called on insmod
module_exit(hello_lkm_exit);    // Called on rmmod
```

## 🛠️ Build & Run

### Build
```bash
make
```

### Load Module
```bash
sudo insmod hello_lkm.ko
```

### View Output
```bash
sudo dmesg | tail -n 5
# Output: [HELLO_LKM] Module Loaded!
```

### Unload Module
```bash
sudo rmmod hello_lkm
```

## 📋 Quick Reference

| Macro/Function | Purpose |
|---------------|---------|
| `printk()` | Kernel logging (use instead of printf) |
| `KERN_INFO` | Info log level |
| `__init` | Marks init function (memory freed after load) |
| `__exit` | Marks exit function (cleanup code) |
| `module_init()` | Registers init function |
| `module_exit()` | Registers exit function |
| `insmod` | Command to load module |
| `rmmod` | Command to unload module |
| `dmesg` | View kernel messages |
| `lsmod` | List loaded modules |

## ⚠️ Important Notes
- Kernel modules run with **full system privileges** - bugs can crash the system
- Always test in a VM first
- Use `sudo` for insmod/rmmod commands
- Messages go to kernel buffer, not terminal (use `dmesg` to view)

## ✅ Learning Checklist
- [ ] Understand module structure (init/exit)
- [ ] Know difference between `printk()` and `printf()`
- [ ] Can build and load a module
- [ ] Understand `__init` and `__exit` macros

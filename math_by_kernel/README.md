# Math by Kernel - Advanced Inter-Module Communication

## 🎯 Overview
Demonstrates advanced inter-module communication where an importer module requests calculations from an exporter module with dynamic parameters. Shows how modules can work together to perform operations.

## 📁 Files
- `calc_exporter.c` - Exports `perform_calculation()` function with operation support
- `calc_importer.c` - Imports and uses calculation function with custom parameters
- `Makefile` - Build configuration

## 🔑 Key Concepts

### 1. Exporter Module (Provides Calculation Service)
```c
// Function that performs calculations based on operation string
long perform_calculation(long a, long b, const char *op) {
    if (strcmp(op, "+") == 0) return a + b;
    if (strcmp(op, "-") == 0) return a - b;
    if (strcmp(op, "*") == 0) return a * b;
    if (strcmp(op, "/") == 0) return b != 0 ? a / b : 0;
    return 0;
}

// Export the function
EXPORT_SYMBOL(perform_calculation);
```

### 2. Importer Module (Uses Calculation Service)
```c
// Declare external function
extern long perform_calculation(long a, long b, const char *op);

// Module parameters for dynamic input
static long x = 50, y = 12;
static char *op_str = "/";
module_param(x, long, 0644);
module_param(y, long, 0644);
module_param(op_str, charp, 0644);

// Call the exported function
long result = perform_calculation(x, y, op_str);
printk(KERN_INFO "Result: %ld\n", result);
```

## 🛠️ Build & Run

### Build Both Modules
```bash
make
```

### Load Order Matters!

**1. Load Exporter First:**
```bash
sudo insmod calc_exporter.ko a=24 b=20 op_str="-"
# Output: [EXPORTER] Loaded. Internal test: 24 - 20 = 4
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
[24354.566582] [EXPORTER] Loaded. Internal test: 24 - 20 = 4
[24354.566588] [EXPORTER] Module loaded. Symbol 'perform_calculation' exported.
```

### Step 3: Load the Importer Module

```bash
sudo insmod calc_importer.ko x=50 y=12 op_str="/"
# Output: [IMPORTER] Success! Exporter returned: 4
```

### View Output
```bash
sudo dmesg | tail -n 6
```

**Expected Output:**
```
[24354.566582] [EXPORTER] Loaded. Internal test: 24 - 20 = 4
[24354.566588] [EXPORTER] Module loaded. Symbol 'perform_calculation' exported.
[24378.736469] [IMPORTER] Module loaded.
[24378.736474] [IMPORTER] Requesting Calculation from Exporter: 50 / 12 = ?
[24378.736477] [IMPORTER] Success! Exporter returned: 4
```

### Unload Order (Reverse!)
```bash
sudo rmmod calc_importer
sudo rmmod calc_exporter
```

⚠️ **Error if wrong order:**
```bash
sudo rmmod calc_exporter
# Error: Module calc_exporter is in use by: calc_importer
```

## 📋 Quick Reference

| Concept | Purpose |
|---------|---------|
| `EXPORT_SYMBOL()` | Make function available to other modules |
| `extern` | Import function from another module |
| `module_param()` | Pass parameters at load time |
| Load order | Exporter FIRST, then Importer |
| Unload order | Importer FIRST, then Exporter |
| Integer division | 50/12 = 4 (truncates decimal) |

## ⚠️ Important Notes
- **Load exporter before importer** - kernel needs symbol to be available
- **Unload importer before exporter** - prevents use-after-free errors
- Kernel tracks module dependencies automatically
- Division by zero returns 0 (no error handling in this example)
- Integer division truncates decimals (50/12 = 4, not 4.166)

## ✅ Learning Checklist
- [ ] Understand EXPORT_SYMBOL for sharing functions
- [ ] Know how to use extern for importing
- [ ] Can pass parameters to modules at load time
- [ ] Understand load/unload order requirements
- [ ] Know how kernel tracks module dependencies

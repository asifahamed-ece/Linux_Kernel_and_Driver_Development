# Module Parameters - Dynamic Configuration for Kernel Modules

## 🎯 Overview
Demonstrates how to pass dynamic parameters to kernel modules at load time, allowing configuration without recompilation. Parameters can also be modified at runtime via sysfs.

## 📁 Files
- `module_parameters.c` - Main module with parameter handling
- `Makefile` - Build configuration

## 🔑 Key Functions & Macros

### 1. Declare Variables with Defaults
```c
static char *name = "World!";
static int count = 2;
```
- Variables become parameters after registration
- Defaults used if no parameters passed

### 2. Register Parameters
```c
module_param(name, charp, 0644);    // String parameter
module_param(count, int, 0644);     // Integer parameter
```
**Syntax:** `module_param(variable, type, permissions)`

**Common Types:**
| Type | Description | Variable Type |
|------|-------------|---------------|
| `int` | Integer | `int` |
| `charp` | String | `char *` |
| `bool` | Boolean | `bool` |
| `long` | Long integer | `long` |

**Permissions:**
- `0644`: User RW, Others Read (most common)
- `0444`: Read-only (cannot change at runtime)
- `0`: No sysfs entry (load-time only)

### 3. Add Descriptions
```c
MODULE_PARM_DESC(name, "A String parameter to greet");
MODULE_PARM_DESC(count, "Number of lines to print");
```
- Shows up in `modinfo` output
- Helps users understand parameters

### 4. Using Parameters
```c
for(i=0; i<count; i++) {
    printk(KERN_INFO "Hello, %s (Iteration %d)\n", name, i+1);
}
```
- Use parameters like regular variables
- No special syntax needed

## 🛠️ Build & Run

### Build
```bash
make
```

### Load with Defaults
```bash
sudo insmod module_parameters.ko
# Uses: name="World!", count=2
```

### Load with Custom Values
```bash
sudo insmod module_parameters.ko name="Linux" count=3
# Uses: name="Linux", count=3
```

### View Output
```bash
sudo dmesg | tail -n 5
# Output: Hello, Linux (Iteration 1)
#         Hello, Linux (Iteration 2)
#         Hello, Linux (Iteration 3)
```

### Runtime Modification (via sysfs)
```bash
# View current values
cat /sys/module/module_parameters/parameters/name
cat /sys/module/module_parameters/parameters/count

# Change values (if writable)
echo "NewName" | sudo tee /sys/module/module_parameters/parameters/name
echo 5 | sudo tee /sys/module/module_parameters/parameters/count
```

### Unload
```bash
sudo rmmod module_parameters
```

## 📋 Quick Reference

| Macro/Command | Purpose |
|---------------|---------|
| `module_param()` | Register a module parameter |
| `MODULE_PARM_DESC()` | Add parameter description |
| `insmod module.ko param=value` | Load with custom parameter |
| `modinfo module.ko` | View parameter info |
| `/sys/module/<name>/parameters/` | Runtime parameter access |

## ⚠️ Important Notes
- **Validate all inputs** - kernel doesn't validate automatically
- Check string lengths and integer ranges
- Use restrictive permissions (0600) for sensitive parameters
- Never use world-writable permissions (0666, 0777)
- Test in VM first - bad parameters can crash system

## ✅ Learning Checklist
- [ ] Understand `module_param()` syntax
- [ ] Know parameter types (int, charp, bool)
- [ ] Can load modules with custom parameters
- [ ] Understand sysfs interface for runtime changes
- [ ] Know importance of input validation

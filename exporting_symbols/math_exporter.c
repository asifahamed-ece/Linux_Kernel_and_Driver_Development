//  Two Modules Talking to Each Other
// We will create two modules: math_exporter (provides a function) and math_importer (uses it).

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

int add_numbers(int, int);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ASIF AHAMED S");
MODULE_DESCRIPTION("Module 1: Exports a Math Function.");

// 1. Define the Fucntion to Export
int add_numbers(int a, int b){
    printk(KERN_INFO "[EXPORTER] Adding %d and %d\n", a, b);
    return a+b;
}

// 2. Exporting Symbol to Kernel's global reg table
EXPORT_SYMBOL(add_numbers);

// 3. Normally Load and Unload Instructions (Next) 
static int __init exporter_init(void){
    printk(KERN_INFO "[EXPORTER] Module Loaded! Symbol 'add_numbers' Exported.\n");
    return 0;
}

static void __exit exporter_exit(void){
    printk(KERN_INFO "[EXPORTER] Module Unloaded.Symbol removed.\nExitting Kernel Space!\n");
}

module_init(exporter_init);
module_exit(exporter_exit);


#include <linux/init.h>     // Needed for the macro functions (__init, __exit)
#include <linux/module.h>    // Needed for all kernel modules
#include <linux/kernel.h>    // Needed for KERN_INFO log level

// 1. Module Metadata (Crucial for kernel licensing and identification)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Asif Ahamed < asifahamed670@gmail.com");
MODULE_DESCRIPTION("First Loadable Kernel Module in Linux");
MODULE_VERSION("1.0");

// 2. Initiation Function (The Constructor)
// __init macros: Frees this memory (RAM) after module Loads
static int __init hello_lkm_init(void){
    // printk() writes to Kernel's Ring Buffer, Not STDOUT.
    printk(KERN_INFO "[HELLO_LKM] Module Loaded Successfully!\n");
    printk(KERN_INFO "[HELLO_LKM] Welcome to Kernel Space.\n");

    // Return 0 indicates success. Negative values indicate specific errors (e.g., -ENOMEM)
    return 0;
}

// 3. Exit Function (Destructor)
// __exit macros: discards this code if built-in, keeps if its modular
static void __exit hello_lkm_exit(void){
    printk(KERN_INFO "[HELLO_LKM] Module Loaded. Cleaning Up Resources...\n");
    printk(KERN_INFO "[HELLO_LKM] GoodBye from Kernel Space!\n");
}

// 4. Register the Entry and Exit points
module_init(hello_lkm_init);
module_exit(hello_lkm_exit);
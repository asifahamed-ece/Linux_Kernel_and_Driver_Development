#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

// 1. define variables with default values
static char *name = "World!";
static int  count = 2;

// 2. Register parameters with kernel.
// 0644: User with RW, Others with Read only
module_param(name, charp, 0644);            // For name Variable
module_param(count, int,  0644);               // For count variable

// Optional Description for Parameters used in Module.
MODULE_PARM_DESC(name, "A String parameter to greet");
MODULE_PARM_DESC(count, "Number of Lines to Print in greeting.");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ASIF AHAMED S");
MODULE_DESCRIPTION("An LKM for Module Parameters Implementation.");
MODULE_VERSION("1.0");

static int __init param_init(void){
    int i;
    printk(KERN_INFO "[PARAM] Module Loaded!\n");
    
    for(i=0; i<count; i++){
        printk(KERN_INFO"[PARAM] Hello, %s (Iteration %d)\n", name, i+1);
    }
    return 0;
}

static void __exit param_exit(void){
    printk(KERN_INFO "[PARAM] Execution Completed...\n");
    printk(KERN_INFO "GoodBye, %s\n", name);
}

// Registering Modules
module_init(param_init);
module_exit(param_exit);
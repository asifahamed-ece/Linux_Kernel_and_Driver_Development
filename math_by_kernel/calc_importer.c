#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// 1. Importer's own parameters (double quotes for string!)
static int x = 10;
static int y = 5;
static char *op_str = "+"; 

module_param(x, int, 0644);
module_param(y, int, 0644);
module_param(op_str, charp, 0644);

MODULE_PARM_DESC(x, "Importer's first number");
MODULE_PARM_DESC(y, "Importer's second number");
MODULE_PARM_DESC(op_str, "Importer's operation string: +, -, *, /");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Asif Ahamed");
MODULE_DESCRIPTION("Module 2: Imports and uses the exported calculation function");

// 2. Declare the external function
extern int perform_calculation(int num1, int num2, const char *op);

static int __init importer_init(void) {
    int result;
    printk(KERN_INFO "[IMPORTER] Module loaded.\n");
    
    // FIXED: Kept the string on a single line to prevent syntax errors
    printk(KERN_INFO "[IMPORTER] Requesting Calculation from Exporter: %d %c %d = ?\n", x, op_str[0], y);
    
    // 3. Call the function from the other module!
    result = perform_calculation(x, y, op_str);
    
    if (result >= 0) {
        printk(KERN_INFO "[IMPORTER] Success! Exporter returned: %d\n", result);
    } else {
        printk(KERN_ERR "[IMPORTER] Calculation failed (check exporter logs).\n");
    }
    
    return 0;
}

static void __exit importer_exit(void) {
    printk(KERN_INFO "[IMPORTER] Module unloaded.\n");
}

module_init(importer_init);
module_exit(importer_exit);
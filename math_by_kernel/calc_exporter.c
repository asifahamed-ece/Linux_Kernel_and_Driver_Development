#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// 1. Define parameters (use char* for the operation, initialized with double quotes)
static int a = 0;
static int b = 0;
static char *op_str = "+"; 

// Function prototype
int perform_calculation(int , int, const char*);

// 2. Register parameters
module_param(a, int, 0644);
module_param(b, int, 0644);
module_param(op_str, charp, 0644);

// 3. Describe parameters
MODULE_PARM_DESC(a, "First number for calculation");
MODULE_PARM_DESC(b, "Second number for calculation");
MODULE_PARM_DESC(op_str, "Operation string: +, -, *, /");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Asif Ahamed");
MODULE_DESCRIPTION("Module 1: Exports a dynamic calculation function");

// 4. The exported calculation function
int perform_calculation(int num1, int num2, const char *op) {
    int result = 0;
    char operation = op ? op[0] : '+'; // Safely get the first character
    
    switch(operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
            if (num2 == 0) {
                printk(KERN_ERR "[EXPORTER] Error: Division by zero!\n");
                return -1;
            }
            result = num1 / num2;
            break;
        default:
            printk(KERN_ERR "[EXPORTER] Error: Unknown operation '%c'!\n", operation);
            return -1;
    }
    
    return result;
}
EXPORT_SYMBOL(perform_calculation);

static int __init exporter_init(void) {
    int result = perform_calculation(a, b, op_str);
    
    if (result >= 0) {
        printk(KERN_INFO "[EXPORTER] Loaded. Internal test: %d %c %d = %d\n", a, op_str[0], b, result);
    }
    printk(KERN_INFO "[EXPORTER] Module loaded. Symbol 'perform_calculation' exported.\n");
    return 0;
}

static void __exit exporter_exit(void) {
    printk(KERN_INFO "[EXPORTER] Module unloaded. Symbol removed.\n");
}

module_init(exporter_init);
module_exit(exporter_exit);
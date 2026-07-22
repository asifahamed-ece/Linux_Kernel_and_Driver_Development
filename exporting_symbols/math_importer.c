# include <linux/init.h>
# include <linux/kernel.h>
# include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ASIF AHAMED S");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Module 2: Imports a Math Function.");

// Declaring the Global Symbol (External Function)
extern int add_numbers(int a, int b);

static int __init importer_init(void){
    printk(KERN_INFO "[IMPORTER] Module Loaded!\n");

    int res;
    res = add_numbers(20 , 12);
    printk(KERN_INFO "[IMPORTER] Result from Exporter Module : %d\n", res);
    return 0;
}

static void __exit importer_exit(void){
    printk(KERN_INFO "[IMPORTER] Module Unloaded! Exiting...\n");
}

module_init(importer_init);
module_exit(importer_exit);

# 🐧 Linux Kernel & Device Driver Development

[![GitHub repo](https://img.shields.io/badge/GitHub-Repository-blue?style=flat-square)](https://github.com/asifahamed-ece/system_programming)
[![License](https://img.shields.io/badge/License-GPL--2.0-green?style=flat-square)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active%20Development-brightgreen?style=flat-square)](https://github.com/asifahamed-ece/system_programming)
[![Kernel](https://img.shields.io/badge/Kernel-5.x%20%7C%206.x-blue?style=flat-square)](https://kernel.org)

A hands-on journey into Linux Kernel internals and Device Driver Development. This repository documents the transition from User-Space System Programming to Kernel-Space Driver Development — covering Loadable Kernel Modules, Character Device Drivers, Kernel Memory, Concurrency, Interrupt Handling, and Platform Drivers.

## 👨‍💻 About the Author
  Asif Ahamed  
🎓 Final-Year Electronics and Communication Engineering (ECE) Student  
   Rajalakshmi Engineering College, Chennai  
🚀 Embedded Systems Enthusiast | STM32 | ESP32 | FreeRTOS | Linux Kernel | Device Drivers  

Building disciplined, modular, and well-documented kernel code. Every module here is a lesson in how the kernel actually works under the hood.


# 🧠 Core Concepts Explained

## Loadable Kernel Modules (LKM)
A Loadable Kernel Module is a compiled object file (.ko) that can be dynamically inserted into a running kernel without rebooting. It extends kernel functionality on demand.

## Key building blocks:
module_init() — Registers the entry point called when the module is loaded via insmod.
module_exit() — Registers the cleanup function called on rmmod.
init / exit — Macros that tell the kernel to free initialization memory after load, and discard exit code if the module is built-in.
printk() — The kernel's logging function. Unlike printf, it writes to the kernel ring buffer (readable via dmesg) and supports log levels like KERNINFO, KERNERR, KERN_DEBUG.

Module Parameters
Modules can expose tunable variables to user-space at load time using module_param(name, type, perm). When permissions are non-zero, the kernel automatically creates a sysfs entry at /sys/module//parameters/, allowing runtime reads and writes without reloading the module.

## Character Device Drivers
A character device handles data as a stream of bytes (like a serial port or a sensor). The driver registers a set of callbacks through the file_operations structure:
c
struct file_operations {
    struct module *owner;
    int (open)(struct inode , struct file *);
    int (release)(struct inode , struct file *);
    ssizet (*read)(struct file *, char user *, sizet, loff_t *);
    ssizet (*write)(struct file *, const char user *, sizet, loff_t *);
    long (unlocked_ioctl)(struct file , unsigned int, unsigned long);
};

Registration is done via registerchrdev() (legacy) or allocchrdevregion() + cdevadd() (modern). Device nodes appear in /dev/ either manually via mknod or automatically through device_create() and udev.

## User-Kernel Data Transfer
Kernel code cannot directly dereference user-space pointers. Safe transfer requires:
copytouser(dst, src, n) — Kernel → User
copyfromuser(dst, src, n) — User → Kernel

Both return the number of bytes that failed to copy. Always check the return value.

ioctl — Custom Device Commands
ioctl provides a channel for device-specific commands that don't fit the read/write model. Commands are encoded using macros like IO, IOR, IOW, IOWR which pack a magic number, command number, and data size into a single unsigned int.

## Kernel Memory Allocation
kmalloc(size, GFP_KERNEL) — Allocates physically contiguous memory. Fast, limited to small sizes.
vmalloc(size) — Allocates virtually contiguous memory. Slower, suitable for large buffers.
Slab allocator (kmemcachecreate) — Custom caches for frequently allocated same-sized objects.

## Concurrency in the Kernel
The kernel is heavily concurrent — interrupts, softirqs, multiple processes, and SMP all compete for the same data.

Mutex — Sleepable lock. Used when the critical section may sleep or take long.
Spinlock — Busy-wait lock. Used for very short critical sections, especially in interrupt context where sleeping is forbidden.
Atomic operations — Lock-free primitives for simple counters and flags (atomict, atomicinc, atomic_read).

## Interrupt Handling
Hardware interrupts are handled in two phases:
Top-half (request_irq handler) — Runs in interrupt context, must be fast, cannot sleep. Acknowledges the hardware.

## Bottom-half — Defers heavy work to a safer context:
  - Tasklets — Run in softirq context, serialized per tasklet.
  - Workqueues — Run in process context, can sleep, scheduled on kernel worker threads.

## Platform Drivers & Device Tree
Modern Linux separates hardware description from driver code. The Device Tree (.dts files) describes hardware topology. Platform drivers bind to devices described in the tree via compatible strings, receiving resources (memory regions, IRQs) through the kernel's resource management APIs.

# ✅ Progress Tracker

## LKM Basics
[x] Hello World module with printk, init, exit  
[x] Module parameters with sysfs integration  

## Character Device Drivers
[ ] file_operations skeleton & device registration  
[ ] ioctl command handling  
[ ] Automatic /dev/ node creation via device_create  

## Kernel Memory
[ ] kmalloc vs vmalloc comparison  
[ ] Custom slab caches  
[ ] mmap implementation  

## Concurrency
[ ] Mutex & semaphore usage  
[ ] Spinlocks & IRQ-safe variants  
[ ] Atomic operations  

## Interrupts
[ ] request_irq top-half handler  
[ ] Tasklets for deferred work  
[ ] Workqueues for sleepable bottom-halves  

## Platform Drivers
[ ] GPIO driver using subsystem APIs  
[ ] I2C client driver  
[ ] Platform device/driver binding  
[ ] Device Tree integration  

## 🛠️ Build & Run Guide

## Prerequisitesbash
sudo apt update  
sudo apt install linux-headers-$(uname -r) build-essential kmod  

## Building a Modulebash
cd LKMBasics/hellomodule/  
make  

The Makefile invokes the kernel build system:makefile  
obj-m += hello_module.o  

all:  
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules  

clean:  
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean  
  
## Loading & Testingbash
sudo insmod hello_module.ko          # Load  
lsmod | grep hello_module            # Verify  
dmesg | tail -n 10                   # Check kernel logs  
sudo rmmod hello_module              # Unload  
make clean                           # Cleanup  

## Module with Parametersbash  
sudo insmod module_parameters.ko name="Asif" count=3  
cat /sys/module/module_parameters/parameters/name    
echo "NewValue" | sudo tee /sys/module/module_parameters/parameters/name  

## 🔬 Kernel Debugging Toolkit  

| Tool | Purpose |
|------|---------|
| dmesg | View kernel ring buffer |
| dmesg -w | Follow kernel logs in real-time |
| lsmod | List loaded modules |
| modinfo  | Show module metadata |
| cat /proc/modules | Alternative module list |
| cat /sys/module//parameters/* | Inspect runtime parameters |
| cat /proc/sys/kernel/tainted | Check kernel taint status |
| journalctl -k | Kernel logs via systemd journal |

## 💡 Key Takeaways

Kernel space has no safety net. A null pointer dereference causes a Kernel Panic, not a segfault.  
printk is not printf. It logs to the kernel ring buffer, not stdout.  
Never dereference user pointers directly. Always use copytouser / copyfromuser.  
Locking choice matters. Mutex for sleepable contexts, spinlock for atomic/interrupt contexts.  
Top-half must be fast. Defer work to tasklets or workqueues.    
Clean up everything. Unregister devices, free memory, release IRQs in the module exit function.  

## 📚 Recommended Reading  
Linux Device Drivers, 3rd Edition — Corbet, Rubini, Kroah-Hartman  
Linux Kernel Development — Robert Love  
Professional Linux Kernel Architecture — Wolfgang Mauerer  
Kernel.org Documentation  
Bootlin Kernel Training Materials  

## 📬 Connect
GitHub: github.com/asifahamed-ece  
LinkedIn: www.linkedin.com/in/asif-ahamed-s-ece  
Email: asifahamed670@gmail.com  

Built one module at a time. No shortcuts, no magic — just kernel code that works.

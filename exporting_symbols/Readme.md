
# Module Export Symbols — Explanation

## What are Exported Symbols?

In the Linux kernel, **exported symbols** are functions or global variables that a module makes available for other modules to use. The kernel maintains a global symbol table that allows modules to resolve external references at load time.

By default, symbols defined in a module are **private** and cannot be used outside that module. To share them, you must explicitly export them using one of the export macros.

---

## Why Export Symbols?

- To enable **reusable kernel code**: multiple drivers can share common helper functions.   
- To support **layered architectures** (e.g., a core module providing a service to peripheral modules).   
- To create **plugin‑like modules** where functionality can be added or removed dynamically.  

---

## CODE's KERNAL OUTPUT:  

❯ sudo dmesg | tail -n 7    
[ 8646.552929] [EXPORTER] Module Loaded! Symbol 'add_numbers' Exported.  
[ 8695.470032] [IMPORTER] Module Loaded!  
[ 8695.470038] [EXPORTER] Adding 20 and 12  
[ 8695.470041] [IMPORTER] Result from Exporter Module : 32  
[ 8744.937768] [IMPORTER] Module Unloaded! Exiting...  
[ 8754.681835] [EXPORTER] Module Unloaded.Symbol removed.  
               Exitting Kernel Space!  
  
---

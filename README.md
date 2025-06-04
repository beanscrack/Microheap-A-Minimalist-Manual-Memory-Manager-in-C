🧬 microheap: A Minimalist Manual Memory Manager in C

This project implements a **manual memory management system** in C — a simplified "heap allocator" that emulates how low-level allocators work underneath `malloc` and `free`. It manages a contiguous memory block with explicit tracking of allocated and free regions using linked metadata structures.

 💡 Features

- ✅ Manual allocation (`cmalloc`) and deallocation (`cfree`) inside a user-controlled memory region  
- ✅ Doubly-linked metadata structure (`struct cnode`) to track memory chunks  
- ✅ Gap-aware allocator: finds and fills gaps between allocations  
- ✅ Debug visualization via `print_debug` to inspect memory layout  
- ✅ Built-in integrity checks via `assert` to ensure safe access and management

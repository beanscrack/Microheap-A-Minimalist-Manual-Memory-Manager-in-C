microheap: A Minimalist Manual Memory Manager in C

This project implements a **manual memory management system** in C — a simplified "heap allocator" that emulates how low-level allocators work underneath `malloc` and `free`. It manages a contiguous memory block with explicit tracking of allocated and free regions using linked metadata structures.

Features

- Manual allocation (`cmalloc`) and deallocation (`cfree`) inside a user-controlled memory region  
- Doubly-linked metadata structure (`struct cnode`) to track memory chunks  
- Gap-aware allocator: finds and fills gaps between allocations  
- Debug visualization via `print_debug` to inspect memory layout  
- Built-in integrity checks via `assert` to ensure safe access and management

How It Works

1. **Memory Region Setup**
   - `make_contiguous(size)` allocates a single contiguous memory block and prepares it for manual use.

2. **Allocation**
   - `cmalloc(block, size)` scans for available space (either at the beginning, between chunks, or at the end).
   - It writes a `struct cnode` before every allocation to track its size and neighbors.

3. **Deallocation**
   - `cfree(ptr)` detaches the node from the doubly-linked list and frees the chunk.

4. **Visualization**
   - `print_debug(block)` prints the entire memory layout, showing gaps, chunk sizes, and content.

Why This Project?

This system was built to **understand and simulate how malloc/free work** under the hood. It’s a learning-focused assignment project to:
- Learn pointer arithmetic
- Practice systems-level C programming
- Understand memory layout and fragmentation


#include <stddef.h>

struct contiguous;

// We need to make these available solely for testing.
// Do not use these values anywhere in your solution!
extern const int SIZEOF_CONTIGUOUS;
extern const int SIZEOF_CNODE;


// make_contiguous(size) Create a block including a buffer of size.
// This function does call malloc.
// effects: allocates memory.  Caller must call destroy_contiguous.
struct contiguous *make_contiguous(size_t size);

//destroy_contiguous(block) Cleans up block.
// effects: calls free.
void destroy_contiguous(struct contiguous *block);

// cmalloc(block, size) Inside block, make a region of size bytes, and
// return a pointer to it.  If there is not enough space,
// return NULL.
// Note: this function must not call malloc.
void *cmalloc(struct contiguous *block, int size);

// cfree(p) Remove the node for which p points to its data.
// Note: this function must not call free.
void cfree(void *p);


// print_debug(block) print a long message showing the content of block.
void print_debug(struct contiguous *block);

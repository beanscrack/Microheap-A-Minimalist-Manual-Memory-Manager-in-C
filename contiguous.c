/////////////////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments > Assignment Information & Policies > Academic Integrity Policy)
/////////////////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// 
//
// Name: Xiutong Wu
// login ID: j692wu
//////////////////////////////////////////////////////////////////////////////////////////

// See contiguous.h for more documentation

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "contiguous.h"


struct contiguous {
  struct cnode *first;
  void *upper_limit;
};

struct cnode {
  size_t nsize;
  struct cnode *prev;
  struct cnode *next;
  struct contiguous *block;
};

const int SIZEOF_CONTIGUOUS = sizeof(struct contiguous);
const int SIZEOF_CNODE = sizeof(struct cnode);



static const char STAR_STR[] = "*";
static const char NULL_STR[] = "NULL";

// maybe_null(void *p) return a pointer to "NULL" or "*",
//   indicating if p is NULL or not.
static const char *maybe_null(void *p) {
  return p ? STAR_STR : NULL_STR;
}

// gapsize(n0, n1) determine the size (in bytes) of the gap between n0 and n1.
static size_t gapsize(struct cnode *n0, struct cnode *n1) {
  assert(n0);
  assert(n1);
  void *v0 = n0;
  void *v1 = n1;
  return (v1 - v0) - n0->nsize - sizeof(struct cnode);
}

// print_gapsize(n0, n1) print the size of the gap between n0 and n1,
//     if it's non-zero.
static void print_gapsize(struct cnode *n0, struct cnode *n1) {
  assert(n0);
  assert(n1);
  size_t gap = gapsize(n0, n1);
  
  if (gap != 0) { 
    printf("%zd byte gap\n", gap);
  }
}


// pretty_print_block(chs, size) Print size bytes, starting at chs,
//    in a human-readable format: printable characters other than backslash
//    are printed directly; other characters are escaped as \xXX
static void pretty_print_block(unsigned char *chs, int size) {
  assert(chs);
  for (int i = 0; i < size; i++) {
    printf(0x20 <= chs[i] && chs[i] < 0x80 && chs[i] != '\\'
           ? "%c" : "\\x%02X", chs[i]);
  }
  printf("\n");
}

// print_node(node) Print the contents of node and all nodes that
//    follow it.  Return a pointer to the last node.
static struct cnode *print_node(struct cnode *node) {
  while (node != NULL) {
    void *raw = node + 1;     // point at raw data that follows.
    printf("struct cnode\n");
    printf("    nsize: %ld\n", node->nsize);
    printf("    prev: %s\n", maybe_null(node->prev));
    printf("    next: %s\n",  maybe_null(node->next));

    printf("%zd byte chunk: ", node->nsize);
    
    pretty_print_block(raw, node->nsize);
    
    if (node->next == NULL) {
      return node;
    } else {
      print_gapsize(node, node->next);
      node = node->next;
    }
  }
  return NULL;
}



static void print_hr(void) {
    printf("----------------------------------------------------------------\n");
}

// print_debug(block) print a long message showing the content of block.
void print_debug(struct contiguous *block) {
  assert(block);
  void *raw = block;

  print_hr();
  printf("struct contiguous\n");
  printf("    first: %s\n", maybe_null(block->first));

  if (block->first == NULL) {
    size_t gap = block->upper_limit - raw - sizeof(struct contiguous);
    printf("%zd byte gap\n", gap);           
  } else {
    void *block_first = block->first;
    size_t gap = block_first - raw - sizeof(struct contiguous);
    if (gap) {
      printf("%zd byte gap\n", gap);
    }
  }
 
  struct cnode *lastnode = print_node(block->first);
  
  if (lastnode != NULL) {
    print_gapsize(lastnode, block->upper_limit);
  }

  print_hr();
}



  struct contiguous *make_contiguous(size_t size) {
    if (size < sizeof(struct contiguous)) {
      return NULL;
    }
    void *block = malloc(size);
    struct contiguous *conti = block;
    conti->first = NULL;
    char *byte_ptr = block;
    conti->upper_limit = byte_ptr + size;
    for (int i = sizeof(struct contiguous); i < size; i++) {
      byte_ptr[i] = '$';
    }
    return conti;
  }


void destroy_contiguous(struct contiguous *block) {
  assert(block);
  if (block->first) {
    printf("Destroying non-empty block!\n");
  }
  free(block);
}


void cfree(void *p) {
  assert(p);
  if (p == NULL) {
    return;
  }
  void *vp_chunk = p;
  char *p_chunk = vp_chunk;
  char *p_node_start = p_chunk - sizeof(struct cnode);
  void *vp_node = p_node_start;
  struct cnode *node = vp_node;
  struct contiguous *block = node->block;
  if (node->prev != NULL) {
    node->prev->next = node->next;
  } else {
    block->first = node->next;
  }
  if (node->next != NULL) {
    node->next->prev = node->prev;
  }
}


void *cmalloc(struct contiguous *block, int size) {
  assert(block);
  assert(size >= 0);
  void *vp_block = block;
  void *vp_upper = block->upper_limit;
  char *p_block = vp_block;
  char *p_upper = vp_upper;
  if (block->first == NULL) {
    char *p_gap_start = p_block + sizeof(struct contiguous);
    int available = p_upper - p_gap_start;
    if (available < size + sizeof(struct cnode)) {
      return NULL;
    }
    void *vp_new_node = p_gap_start;
    struct cnode *new_node = vp_new_node;
    new_node->nsize = size;
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->block = block;
    block->first = new_node;
    char *p_chunk_ptr = p_gap_start + sizeof(struct cnode);
    void *vp_chunk_ptr = p_chunk_ptr;
    return vp_chunk_ptr;
  } else {
      struct cnode *first_node = block->first;
      char *p_gap_start = p_block + sizeof(struct contiguous);
      void *vp_first_node = first_node;
      char *p_first_node = vp_first_node;
      int available = p_first_node - p_gap_start;
      if (available >= size + sizeof(struct cnode)) {
        void *vp_new_node = p_gap_start;
        struct cnode *new_node = vp_new_node;
        new_node->nsize = size;
        new_node->prev = NULL;
        new_node->next = first_node;
        new_node->block = block;
        first_node->prev = new_node;
        block->first = new_node;
        char *p_chunk_ptr = p_gap_start + sizeof(struct cnode);
        void *vp_chunk_ptr = p_chunk_ptr;
        return vp_chunk_ptr;
      }
      struct cnode *current = block->first;
      while (current->next != NULL) {
        void *vp_current = current;
        char *p_current = vp_current;
        char *p_current_end = p_current + sizeof(struct cnode)
        + current->nsize;
        void *vp_next = current->next;
        char *p_next = vp_next;
        available = p_next - p_current_end;
        if (available >= size + sizeof(struct cnode)) {
          void *vp_new_node = p_current_end;
          struct cnode *new_node = vp_new_node;
          new_node->nsize = size;
          new_node->prev = current;
          new_node->next = current->next;
          new_node->block = block;
          current->next->prev = new_node;
          current->next = new_node;
          char *p_chunk_ptr = p_current_end + sizeof(struct cnode);
          void *vp_chunk_ptr = p_chunk_ptr;
          return vp_chunk_ptr;
        }
        current = current->next;
      }
      void *vp_current = current;
      char *p_current = vp_current;
      char *p_current_end = p_current + sizeof(struct cnode)
      + current->nsize;
      available = p_upper - p_current_end;
      if (available < size + sizeof(struct cnode)) {
        return NULL;
      }
      void *vp_new_node = p_current_end;
      struct cnode *new_node = vp_new_node;
      new_node->nsize = size;
      new_node->prev = current;
      new_node->next = NULL;
      new_node->block = block;
      current->next = new_node;
      char *p_chunk_ptr = p_current_end + sizeof(struct cnode);
      void *vp_chunk_ptr = p_chunk_ptr;
      return vp_chunk_ptr;
    }
  return NULL;
}



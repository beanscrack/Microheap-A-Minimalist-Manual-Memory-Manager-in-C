#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "contiguous.h"

void public_test(void) {
  struct contiguous *c = make_contiguous(150);
  print_debug(c);

  char *p0 = cmalloc(c, 12 * sizeof(char));
  strcpy(p0, "Hello World");
  print_debug(c);
  // Test: is it offset by exactly enough space for the two
  //     structures?
  assert(p0 == SIZEOF_CONTIGUOUS + SIZEOF_CNODE + (void *) c);
  // Test: we copied the date in correctly.
  assert(!strcmp(p0, "Hello World"));  
  
  char *p1 = cmalloc(c, 20);
  p1[0] = '!';
  print_debug(c);
  // Test: is it offset by exactly enough space for the chunk and the
  //     new node?
  assert(p1 == 12 * sizeof(char) + SIZEOF_CNODE + (void *) p0);
  // Test: data is set properly.
  assert(p1[0] == '!');
  for (int i=1; i < 20; i++) {
    assert(p1[i] == '$');
  }
  
  cfree(p0);
  p0 = NULL;
  print_debug(c);

  int *p2 = cmalloc(c, sizeof(int));
  *p2 = 0x00EEFFC0;
  print_debug(c);

  // Test: is it in the first slot?
  assert(p2 == SIZEOF_CONTIGUOUS + SIZEOF_CNODE + (void *) c);
  // Test: data is set properly.
  assert(*p2 == 0x00EEFFC0);
  

  void *p3 = cmalloc(c, 0);
  print_debug(c);

  // Test: immediately after slot p1).
  assert(p3 == 20 * sizeof(char) + SIZEOF_CNODE + (void *) p1); 

  void *p4 = cmalloc(c, 0);
  print_debug(c);

  // Test: returned NULL when there is not enough space.
  assert(p4 == NULL);
  
  cfree(p2);
  p2 = NULL;
  cfree(p1);
  p1 = NULL;
  print_debug(c);

  cfree(p3);
  p3 = NULL;
  print_debug(c);

  printf("Now we call destroy_contiguous. "
    "It calls free.\n");
  
  destroy_contiguous(c);
}



int main(void) {
  public_test();
}

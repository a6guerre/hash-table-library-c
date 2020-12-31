#include <stdio.h>
#include "hashmap.h"

int main(void)
{
  Hashmap *map = Hashmap_create(NULL, NULL);
  char *str = "Max";
  int val = 5;

  Hashmap_set(map, str, &val);
 
  return 0;
}

#ifndef _hashmap_h
#define _hashmap_h

#include <stdint.h>
#include "darray.h"

#define DEFAULT_NUMBER_OF_BUCKETS 5

typedef int (*Hashmap_compare) (void *a, void *b);
typedef uint32_t(*Hashmap_hash) (void *key);

typedef struct Hashmap {
  DArray *buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;
} Hashmap;

typedef struct HashmapNode
{
  void *key;
  void *data;
  uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb) (HashmapNode *node);

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash);
void Hashmap_destroy(Hashmap * map);

int Hashmap_set(Hashmap * map, char *key, void *data);
void *Hashmap_get(Hashmap * map, char *key);

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap * map, void *key);

#endif

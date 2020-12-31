#include <stdio.h>
#include <string.h>
#include "hashmap.h"

static uint32_t default_hash(void *a);
static int default_compare(void *a, void *b);
static DArray *Hashmap_get_bucket(Hashmap * map, int hash_val);

static int default_compare(void *a, void *b)
{
  return strcmp((char *) a, (char *) b);
}

static DArray *Hashmap_get_bucket(Hashmap * map, int hash_val)
{
  DArray *bucket = &map->buckets[hash_val];
  return bucket;
}

static uint32_t default_hash(void *a)
{
  size_t len = strlen((char *)a);
  char *key = (char *) a;

  printf("len: %ld\n", len);
  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i)
  {
    hash += key[i];
    hash += (hash << 10 );
    hash ^= (hash >> 6 );
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  printf("hash value %d\n", hash);
  return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
  Hashmap *map = (Hashmap *)malloc(sizeof(Hashmap));
  check_mem(map);
  map->compare = (compare == NULL) ? default_compare : compare;
  map->hash = (hash == NULL) ? default_hash : hash; 
  map->buckets = DArray_create(sizeof(DArray *), DEFAULT_NUMBER_OF_BUCKETS);
  map->buckets->count =  DEFAULT_NUMBER_OF_BUCKETS; //fake out expanding it

  return map;

error:
  return NULL;
}
void Hashmap_destroy(Hashmap * map);

int Hashmap_set(Hashmap * map, void *key, void *data)
{
  uint32_t hash_val = map->hash(key);
  int elem = hash_val % DEFAULT_NUMBER_OF_BUCKETS;

  DArray *bucket = Hashmap_get_bucket(map, elem); 
  HashmapNode *node = malloc(sizeof(HashmapNode));
  node->key = key;
  node->data = data;
  node->hash = hash_val; 
  printf("key: %s \n", (char *)node->key);

  if(!bucket)
  {
    printf("bucket is not null\n");
    DArray_push(bucket, node);
  }
  else  // Current bucket has not been initialized
  {
    printf("Bucket is null, let's create an array for it\n");
    bucket = DArray_create(sizeof(HashmapNode *), 1);
    DArray_set(bucket, 0, node);
    DArray_set(map->buckets, elem, bucket);   
  }
  
  bucket = DArray_get(map->buckets, elem);
  node = DArray_get(bucket, 0);
  printf("key: %s \n", (char *)node->key);

  return 1;
}

void *Hashmap_get(Hashmap * map, void *key);

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb);

void *Hashmap_delete(Hashmap * map, void *key);


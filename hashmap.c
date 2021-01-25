/*
 * Author: Max Guerrero
 * Notes: This code is an exercise from "Learn C The Hard Way".
 * I only refernced the solution code when I absolutely could not find my own bugs.
 * Otherwise, this is my solutin to that exercise.
 * Please feel free to use in your project and modify if neccessary. 
 */

#include <stdio.h>
#include <string.h>
#include "hashmap.h"

static uint32_t default_hash(void *a);
static int default_compare(void *a, void *b);
static void *Hashmap_get_bucket(Hashmap * map, int hash_val);
static HashmapNode *Hashmap_search_node(Hashmap *map, DArray *bucket, void *key);
static int Hashmap_search_node_index(Hashmap *map, DArray *bucket, void *key);

static int default_compare(void *a, void *b)
{
  return strcmp((char *) a, (char *) b);
}

static void *Hashmap_get_bucket(Hashmap * map, int hash_val)
{
  return DArray_get(map->buckets, hash_val);
}

static uint32_t default_hash(void *a)
{
  size_t len = strlen((char *)a);
  char *key = (char *) a;

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
 
  return hash;
}

static HashmapNode *Hashmap_search_node(Hashmap *map, DArray *bucket, void *key)
{
  int i;
  size_t len = DArray_count(bucket);
  HashmapNode *iter;

  for ( i = 0; i < len; ++i)
  {
    iter = DArray_get(bucket, i);
    if (iter == NULL)
    {
      break;
    }

    if(map->compare(key, iter->key) == 0)
    {
      return iter;
    }
  }
  printf("Could not find node\n");

  return NULL;
}

static int Hashmap_search_node_index(Hashmap *map, DArray *bucket, void *key)
{
  int i;
  size_t len = DArray_count(bucket);
  HashmapNode *iter;

  for ( i = 0; i < len; ++i)
  {
    iter = DArray_get(bucket, i);
    if (iter == NULL)
    {
      break;
    }

    if(map->compare(key, iter->key) == 0)
    { 
      return i;
    }
  }
  printf("Could not find node\n");

  return -1;
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

void Hashmap_destroy(Hashmap * map)
{
  if(map)
  {
    int idx, idx2;
    if(map->buckets)
    {
      for (idx = 0; idx < DEFAULT_NUMBER_OF_BUCKETS; ++idx)
      {
        DArray *bucket = DArray_get(map->buckets, idx);
        if(bucket)
        {
          for (idx2 = 0; idx2 < bucket->count; ++idx2)
          {
            HashmapNode *node = DArray_get(bucket, idx2);
	    free(node); 
          }
          DArray_destroy(bucket);
        }
      }
       DArray_destroy(map->buckets);
    }
    free(map);
  } 
}

int Hashmap_set(Hashmap * map, void *key, void *data)
{
  uint32_t hash_val = map->hash(key);
  int elem = hash_val % DEFAULT_NUMBER_OF_BUCKETS;

  DArray *bucket = Hashmap_get_bucket(map, elem);
  //Todo: make the node creation a seperate function 
  HashmapNode *node = malloc(sizeof(HashmapNode));
  node->key = key;
  node->data = data;
  node->hash = hash_val; 
  
  if(bucket != NULL)
  {
    DArray_push(bucket, node);
  }
  else  // Current bucket has not been initialized
  {
    bucket = DArray_create(sizeof(HashmapNode *), DEFAULT_NUMBER_OF_BUCKETS);
    DArray_set(bucket, 0, node);
    DArray_set(map->buckets, elem, bucket);   
  }
  
  bucket = DArray_get(map->buckets, elem);
  node = DArray_get(bucket, 0);
 
  return 1;
}

void *Hashmap_get(Hashmap * map, void *key)
{
  uint32_t hash_val = map->hash(key);
  int elem = hash_val % DEFAULT_NUMBER_OF_BUCKETS;

  DArray *bucket = Hashmap_get_bucket(map, elem);
  check(bucket != NULL, "Array cannot be NULL");

  // make function search node
  
  HashmapNode *res = Hashmap_search_node(map, bucket, key);
  check(res != NULL, "Result not found");
  return res->data;
error:
  return NULL;

}

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb)
{
  int i,j, rc = 0;
  DArray *buckets = map->buckets;
  DArray *bucket;
  size_t len = map->buckets->count;
  printf("map->buckets->count: %ld\n", len);
  for (i = 0; i < len; ++i)
  {
    bucket = DArray_get(buckets, i);
    if(bucket != NULL)
    {
      for (j = 0; j < bucket->count; ++j)
      {
        printf("bucket: %d ", i);
        rc = traverse_cb(DArray_get(bucket, j));
	if( rc != 0)
	{
          return rc;
	}
      }
    }
  }

  return 0;
}

void *Hashmap_delete(Hashmap * map, void *key)
{
  uint32_t hash_val = map->hash(key);
  int elem = hash_val % DEFAULT_NUMBER_OF_BUCKETS;

  DArray *bucket = Hashmap_get_bucket(map, elem);
  check(bucket != NULL, "Array cannot be NULL");

  // make function search node
  
  int idx = Hashmap_search_node_index(map, bucket, key);
  check(idx != -1, "Coulnd't delete: result not found\n");
  
  HashmapNode *node = DArray_get(bucket, idx);
  void *data = node->data;
  free(node);
  DArray_set(bucket, idx, NULL);
  
  // If the node we just deleted is not last, then replace end node to be at 'idx'
  HashmapNode *last = DArray_pop(bucket);
  if(node != last)
  {
    DArray_set(bucket, idx, last); 
  }

  return data;

error:
  return NULL;
}

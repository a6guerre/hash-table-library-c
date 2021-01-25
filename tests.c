#include "minunit.h"
#include "hashmap.h"
#include "dbg.h"

typedef struct info
{
  char *name;
  int data;  
}info;

info info_arr[] = {
	           {"max", 545},
		   {"rex", 500},
		   {"phil", 3},
		   {"monica", 4},
		   {"susan", 8},
		   {"john", 55}, 
		   {"mike", 44},
		   {"taylor", 45},
		   {"tomas", 47},
		   {"ana", 48},
		   {"paul", 88},
                   {"luke", 545},
		   {"jessica", 500},
		   {"jarvis", 3},
		   {"david", 4},
		   {"jeff", 8},
		   {"alan", 55}, 
		   {"raul", 44},
		   {"antonio", 45},
		   {"jake", 47},
		   {"george", 48},
		   {"lisa", 88},
                  };

Hashmap *map;

static int traverse_cb(HashmapNode *node);

static int traverse_cb(HashmapNode *node)
{
  printf("(key: %s, value: %d)\n", (char *)node->key , *(int *)node->data);
  return 0;
}

char *test_create_map()
{
  map = Hashmap_create(NULL, NULL);
  mu_assert(map, "Hash map creation failed");
  check(map != NULL, "Error creating map\n");

  ++tests_run; 

  return NULL;

error:
  return "Error creating map\n";
}

char *test_append_entry()
{
  int idx;

  for (idx = 0; idx < sizeof(info_arr)/sizeof(info_arr[0]); ++idx)
  {
    int rc = Hashmap_set(map, info_arr[idx].name, &info_arr[idx].data);
    mu_assert(rc !=  0, "Hashmap setting failed\n");
  }
 
  for (idx = 0; idx < sizeof(info_arr)/sizeof(info_arr[0]); ++idx)
  {
    int *data = (int *)Hashmap_get(map, info_arr[idx].name); 
    mu_assert(*data == info_arr[idx].data, "Node that was set was not set properly");
    check(*data == info_arr[idx].data, "Error either setting or retreiving to/from Hash table %s", info_arr[idx].name);
  }
  ++tests_run;

  return NULL;

error:
  return "Test Append Entry Failed\n";
}

char *test_traverse()
{
  int rc = Hashmap_traverse(map, traverse_cb);
  mu_assert(rc == 0, "Hashmap Traverse failed\n");
  check(rc == 0, "Error");
  ++tests_run; 

  return NULL;

error:
  return "Hashmap Traverse Failed\n";
}

char *test_delete_node()
{  
  int *data = Hashmap_delete(map, info_arr[0].name);
  mu_assert(*data == info_arr[0].data, "Data member differs that of deleted node!\n");
  mu_assert(data != NULL, "Could not find node to delete\n");
  data =  Hashmap_delete(map, info_arr[0].name);
  mu_assert(data == NULL, "Error: Deleting node that should've alredy been deleted\n"); // Trigger error on purpose

  data = Hashmap_delete(map, info_arr[1].name);
  mu_assert(*data == info_arr[1].data, "Data member differs that of deleted node!\n");
  mu_assert(data != NULL, "Could not find node to delete\n");
  data =  Hashmap_delete(map, info_arr[1].name);
  mu_assert(data == NULL, "Error: Deleting node that should've alredy been deleted\n"); // Trigger error on purpose
  ++tests_run;

  return NULL;
}

char *test_destroy()
{
  Hashmap_destroy(map);
  ++tests_run;

  return NULL;
}

char *all_tests()
{
  char *msg;
  mu_suite_start();
  check((msg = test_create_map()) == NULL, "Test Create Map Failed\n");
  check((msg = test_append_entry()) == NULL, "Test Append Entry Failed\n");
  check((msg = test_traverse()) == NULL, "");
  check((msg = test_delete_node()) == NULL, "");
  check((msg = test_destroy()) == NULL, "");

  return NULL;

error:
  return msg;
}

RUN_TESTS(all_tests);

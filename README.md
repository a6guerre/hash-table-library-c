# hash-table-library-c
Hash Table Library in C

git clone https://github.com/a6guerre/hash-table-library-c

cd hash-table-library-c

# Build
Note: Working on getting the makefile, in the meantime run:

gcc darray.c hashmap.c tests.c -o test

./test

Feel free to modify tests, or link this library to your code if you need a hashmap data structures.

# Nice to haves to work on
- Each bucket/bin can be such that the elemets are always sorted. This would increase insertiom time because you will do 
  a binary search to find each node. But now its looping through all nodes in a bucket just to find each one.
- Dynamically size the number of buckets, i.e let the caller specify grep for (DEFAULT_NUMBER_OF_BUCKETS).
- Delete buckets that are empty of nodes to save space, or put empty nuckets into a cache so that you can save time on lost creating and destroying.
- Use a better default_hash. 

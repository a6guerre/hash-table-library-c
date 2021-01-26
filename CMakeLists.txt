cmake_minimum_required(VERSION 3.0)

project(hast-table-library-c)

add_library(hashtable_a STATIC darray.c hashmap.c)
add_library(hashtable SHARED darray.c hashmap.c)

add_executable(unittest tests.c)
target_link_libraries(unittest hashtable_a)

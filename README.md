# My C List

This is a University project done in the course of "Object-Oriented Programming and Big Data". The subject was actually taugth in Italian, so most comments in the source code are in Italian.

I implemented the List object in C through the use of structs and function pointers.

Files are structured in the following way:

* file.c contains the actual source code

* file.h contains functions and enums that the end user needs to utilize the list

* file.hidden contains the internal functions and declarations that the end user doesn't need to trouble with

# How it works

The List is internally a struct:

```
typedef struct _my_list {
  void*      plist;
  type_list  tlist;
} my_list;
```

with a pointer to an internal data structure and a number that determines the type of the list, chosen at
initialization from the parameters of *malloc_list*.

Then, for each provided function, an array of function pointers as long as the number of types of list is allocated.
Everytime a function is called on a list, internally *function[type_list]* is actually used.

# Types of list

Two types of List are provided:

* *list_dynamic*: the classical type of list, made of various nodes pointing each one to the next. 
This type of list occupies memory proportional to the number of elements contained (plus a small overhead).

* *list_table*: a less common type of list, where all instantiated list are located in a big table (able to expand when full).
This type of list is generally faster, but slows down when it needs to expand and occupies more memory in average.

Then, each node of the List can contain three types of data:

* *single elements*: there is a element (for example a float) in each node

* *array of length N*: there are N elements (for example an array of N floats) in each node.
This is faster when you expect to use it with a fixed number of items at a time

* *generic elements*: you can put anything you want in the node, but you need to specify the maximum size of the items
you want to put in when initializing the list

Internally data types are implemented as three group of list types:

* *generic*: it's the simplest. Each node has the size chosen at initialization. Technically the other types
can be implemented as special case of generic, however this would case a overhead to keep track of the actual type contained

* *BASETYPE*: each node has the dimension of a BASETYPE. A BASETYPE is any type of variable in C (int, float, char, ...). At compile time, a bash script duplicates this type of
list for all the types of C, so that you can instantiate a List specifically for ints, floats and so on

* *array of BASETYPE*: each node has the size of an array of N BASETYPE. Also this type is duplicated multiple times, so that you can create lists for array of ints, array of floats and so on

# Member functions

Here is a list of the provided functions:

* malloc_list

* malloc_list_specify_table

* change_resize_table

* resize_table

* get_info_table

* free_list

* get_type_list

* insert_first

* insert_last

* insert_nth

* extract_first

* extract_last

* extract_nth

* get_n_elem

* search_first

* print_list

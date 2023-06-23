#include <stdio.h>
#include <stdlib.h>
#include "../list/list.h"

int print_float(ALL_TYPE value, unsi size){
    printf("%f", (float) value);
    return 1;
  }

int main(int argc, char *argv[])
{
    pvoid plist;
    float f = 3.0;

    if((plist = malloc_list(type_list_dynamic, "FLOAT", 1)) == NULL) printf("Non creata.\n");
    insert_first(plist, (ALL_TYPE) 2.4, 29);
    insert_first(plist, (ALL_TYPE) 4.4, 29);
    print_list(plist, print_float);
    extract_first(plist, (ALL_TYPE*) &f, NULL);
    print_list(plist, print_float);
    extract_first(plist, (ALL_TYPE*) &f, NULL);
    print_list(plist, print_float);

    printf("Estratto %f\n", f);
    return 0;
  }

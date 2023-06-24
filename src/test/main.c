#include <stdio.h>
#include <stdlib.h>
#include "../list/list.h"


int print_float(ALL_TYPE value, unsi size){
    printf("%f", (float) value);
    return 1;
  }

int print_generic(ALL_TYPE value, unsi size){
    printf("%f",*(pfloat)TO_PVOID(value));
    return 1;
  }

void test_generic(){
    pvoid plist;
    float f = 3.0;
    pfloat pextract;
    unsi size_extract;

    if((plist = malloc_list(type_list_dynamic, "GENERIC", 1)) == NULL) printf("Non creata.\n");
    insert_first(plist, TO_ALLTYPE(&f), sizeof(float));
    printf("yes");
    print_list(plist, print_generic);
    extract_first(plist, TO_ALLTYPE(&pextract), &size_extract);
    printf("Estratto %f con size %u\n", *pextract, size_extract);

  }

void test_float(){
    pvoid plist;
    float f = 3.0;

    if((plist = malloc_list(type_list_dynamic, "FLOAT", 1)) == NULL) printf("Non creata.\n");
    insert_first(plist, (ALL_TYPE) 2.4, 29);
    insert_first(plist, (ALL_TYPE) 4.4, 29);
    print_list(plist, print_float);
    extract_first(plist, TO_ALLTYPE(&f), NULL);
    print_list(plist, print_float);
    extract_first(plist,TO_ALLTYPE(&f), NULL);
    print_list(plist, print_float);
    printf("Estratto %f\n", f);
  }

int main(int argc, char *argv[])
{
    test_generic();

    return 0;
  }



#include <stdio.h>
#include <stdlib.h>
#include "../list/list.h"


int print_float(ALL_TYPE value, unsi size){
  printf("%f", (float) value);
  return 1;
 }

int print_array_float(ALL_TYPE value, unsi size){
  pfloat parray = TO_PVOID(value);
  int i;

  printf("Array: ");
  for (i = 0; i < size; i++) {
    printf("%f,", parray[i]);
   }
  printf("\n");
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
  free_list(plist);
  for(;;){
    plist = malloc_list(type_list_dynamic, "GENERIC", 1);
    insert_first(plist, TO_ALLTYPE(&f), sizeof(float));
    free_list(plist);
   }
 }

void test_float(){
  pvoid plist;
  float f = 3.0;

  if((plist = malloc_list(type_list_dynamic, "FLOAT", 1)) == NULL) printf("Non creata.\n");
  insert_first(plist, TO_ALLTYPE(2.4), 29);
  insert_first(plist, TO_ALLTYPE(4.4), 29);
  print_list(plist, print_float);
  extract_first(plist, TO_ALLTYPE(&f), NULL);
  print_list(plist, print_float);
  extract_first(plist,TO_ALLTYPE(&f), NULL);
  print_list(plist, print_float);
  printf("Estratto %f\n", f);
  for(;;){
    plist = malloc_list(type_list_dynamic, "FLOAT", 1);
    insert_first(plist, TO_ALLTYPE(2.4), 0);
    free_list(plist);
   }
 }

void test_array_float(){
  pvoid plist;
  float parray[3] = {1, 2, 23};
  pfloat poutput;

  if((plist = malloc_list(type_list_dynamic, "FLOAT", 3)) == NULL) printf("Non creata.\n");
  insert_first(plist, TO_ALLTYPE(parray), 0);
  /* printf("Float %f\n", GET_FLOAT(parray)); */
  print_list(plist, print_array_float);
  extract_first(plist, TO_ALLTYPE(&poutput), NULL);
  print_list(plist, print_array_float);
  printf("Estratto {%f, %f, %f}\n", poutput[0], poutput[1], poutput[2]);
  free_list(plist);
  for(;;){
    plist = malloc_list(type_list_dynamic, "FLOAT", 3);
    insert_first(plist, TO_ALLTYPE(parray), 0);
    free_list(plist);
   }
 }

void test_table_generic(){
  pvoid plist;

  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  if((plist = malloc_list_specify_table(type_list_table, "GENERIC", 3, type_resize_default, 0)) == NULL) printf("Non creata.\n");
 }

int main(int argc, char *argv[]){
  test_table_generic();

  return 0;
 }



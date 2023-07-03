#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../list/list.h"


int print_float(all_type value, unsi size){
  printf("%f", value.f);
  return 1;
 }

int print_char(all_type value, unsi size){
  printf("%c", value.c);
  return 1;
 }

int print_array_float(all_type value, unsi size){
  pfloat parray = value.pv;
  int i;

  printf("Array di dim %u: ", size);
  for (i = 0; i < size; i++) {
    printf("%f,", parray[i]);
   }
  printf("\n");
  return 1;
 }

int print_generic(all_type value, unsi size){
  printf("%f",*(pfloat)value.pv);
  return 1;
 }

int compare_float(all_type value1, unsi size1,
                  all_type value2, unsi size2){
  printf("Comparo %f e %f\n", value1.f, value2.f);
  if(value1.f == value2.f) return 0;
  if(value1.f > value2.f)  return 1;
  return -1;
 }

void test_generic(){
  pvoid plist;
  float f = 3.0;
  pfloat pextract;
  unsi size_extract;

  if((plist = malloc_list(type_list_dynamic, "GENERIC", 1)) == NULL) printf("Non creata.\n");
  f=5;
  insert_last(plist, (all_type)((pvoid)&f), sizeof(float));
  printf("Estratto %f con size %u\n", *pextract, size_extract);
  f=18;
  insert_first(plist, (all_type)((pvoid)&f), sizeof(float));
  extract_last(plist, (all_type)((pvoid)&pextract), &size_extract);
  printf("Estratto %f con size %u\n", *pextract, size_extract);
  print_list(plist, print_generic);
  extract_last(plist, (all_type)((pvoid)&pextract), &size_extract);
  extract_last(plist, (all_type)((pvoid)&pextract), &size_extract);
  print_list(plist, print_generic);
  extract_first(plist, (all_type)((pvoid)&pextract), &size_extract);
  printf("Estratto %f con size %u\n", *pextract, size_extract);
  free_list(plist);
  for(;;){
    plist = malloc_list(type_list_dynamic, "GENERIC", 1);
    insert_first(plist, (all_type)((pvoid)&f), sizeof(float));
    free_list(plist);
   }
 }

void test_float(){
  pvoid plist;
  float f = 3.0;

  if((plist = malloc_list(type_list_dynamic, "FLOAT", 1)) == NULL) printf("Non creata.\n");
  insert_last(plist, (all_type)((float)8.4), 29);
  insert_first(plist, (all_type)((float)2.4), 29);
  extract_last(plist, (all_type)((pvoid)&f), NULL);
  printf("Estratto %f\n", f);
  extract_last(plist, (all_type)((pvoid)&f), NULL);
  print_list(plist, print_float);
  extract_last(plist, (all_type)((pvoid)&f), NULL);
  printf("Estratto %f\n", f);
  print_list(plist, print_float);
  extract_first(plist, (all_type)((pvoid)&f), NULL);
  print_list(plist, print_float);
  extract_first(plist,(all_type)((pvoid)&f), NULL);
  print_list(plist, print_float);
  printf("Estratto %f\n", f);
  for(;;){
    insert_first(plist, (all_type)((float)2.4), 0);
    extract_first(plist, (all_type)((pvoid)&f), NULL);
   }
 }

void test_search_float(){
  pvoid plist;
  float f = 3.0;

  if((plist = malloc_list(type_list_dynamic, "FLOAT", 1)) == NULL) printf("Non creata.\n");
  insert_first(plist, (all_type)((float)2.4), 29);
  insert_first(plist, (all_type)((float)4.4), 29);
  insert_first(plist, (all_type)((float)5), 29);
  if(!search_first(plist, (all_type)(float)4.4, 0, (all_type)(pvoid) &f, 0, compare_float)) printf("Non trovato");
  else printf("Trovato %f\n", f);
 }

void test_array_float(){
  pvoid plist;
  float parray[3] = {1, 2, 23};
  pfloat poutput;

  if((plist = malloc_list(type_list_dynamic, "FLOAT", 3)) == NULL) printf("Non creata.\n");
  insert_first(plist, (all_type)((pvoid)parray), 0);
  parray[0] = 10;
  insert_last(plist, (all_type)((pvoid)parray), 0);
  parray[0] = 100;
  insert_last(plist, (all_type)((pvoid)parray), 0);
  extract_last(plist, (all_type)((pvoid)&poutput), NULL);
  printf("Estratto {%f, %f, %f}\n", poutput[0], poutput[1], poutput[2]);
  parray[0] = 1000;
  insert_last(plist, (all_type)((pvoid)parray), 0);
  extract_last(plist, (all_type)((pvoid)&poutput), NULL);
  printf("Estratto {%f, %f, %f}\n", poutput[0], poutput[1], poutput[2]);
  extract_last(plist, (all_type)((pvoid)&poutput), NULL);
  extract_last(plist, (all_type)((pvoid)&poutput), NULL);
  extract_last(plist, (all_type)((pvoid)&poutput), NULL);
  /* printf("Float %f\n", GET_FLOAT(parray)); */
  print_list(plist, print_array_float);
  extract_first(plist, (all_type)((pvoid)&poutput), NULL);
  print_list(plist, print_array_float);
  printf("Estratto {%f, %f, %f}\n", poutput[0], poutput[1], poutput[2]);
  free_list(plist);
  for(;;){
    plist = malloc_list(type_list_dynamic, "FLOAT", 3);
    insert_first(plist, (all_type)((pvoid)parray), 0);
    free_list(plist);
   }
 }

void test_table_generic(){
  pvoid plist, plistdue;
  float f=4;
  pfloat pf;
  unsi size, tot, occupied;

  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  if((plist = malloc_list_specify_table("GENERIC", 3, type_resize_default, 5)) == NULL) printf("Non creata.\n");
  if((insert_first(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("Non inserito\n");
  if((insert_first(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("Non inserito\n");
  if((insert_first(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("Non inserito\n");
  if((insert_first(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("Non inserito\n");
  if((insert_first(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("Non inserito\n");
  free_list(plist);
  if((plist = malloc_list_specify_table("GENERIC", 3, type_resize_default, 1)) == NULL) printf("Non creata.\n");
  /* if((plistdue = malloc_list_specify_table("GENERIC", 3, type_resize_default, 100)) == NULL) printf("Non creata.\n"); */
  /* free_list(plistdue); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* insert_first(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* insert_first(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* insert_first(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* f=5; */
  /* /1* insert_last(plist, (all_type)((pvoid)&f), sizeof(float)); *1/ */
  /* extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* print_list(plist, print_generic); */
  /* insert_last(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /* extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /* extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /* printf("Estratto %f con size %u\n", *pf, size); */
  /* f=5; */
  /* insert_last(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* f=10; */
  /* insert_first(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /* print_list(plist, print_generic); */
  /* extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /* printf("Estratto %f con size %u\n", *pf, size); */
  /* print_list(plist, print_generic); */
  /* for(;;){ */
  /*   insert_last(plist, (all_type)((pvoid)&f), sizeof(float)); */
  /*   extract_last(plist, (all_type)((pvoid)&pf), &size); */
  /*   free(pf); */
  /*  } */
 }

void test_table_char(){
  pvoid plist;
  char  c='a', ex;
  unsi tot, occupied;

  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  free_list(plist);
  if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  insert_first(plist, (all_type)c, sizeof(float));
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  /* print_list(plist, print_char); */
  extract_last(plist, (all_type)((pvoid)&ex), NULL);
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  /* extract_last(plist, (all_type)((pvoid)&ex), NULL); */
  /* printf("Estratto %c\n", ex); */
  /* print_list(plist, print_char); */
  /* c='b'; */
  /* insert_first(plist, (all_type)c, sizeof(float)); */
  /* c='c'; */
  /* insert_first(plist, (all_type)c, sizeof(float)); */
  /* print_list(plist, print_char); */
  /* extract_last(plist, (all_type)((pvoid)&ex), NULL); */
  /* printf("Estratto %c\n", ex); */
  /* print_list(plist, print_char); */
  /* /1* free_list(plist); *1/ */
  /* for(;;){ */
  /*   i++; */
  /*   if((insert_last(plist, (all_type)c, sizeof(float))) == 0) break; */
  /*   extract_last(plist, (all_type)((pvoid)&ex), NULL); */
  /*  } */
  /* printf("iter %d\n", i); */
 }

void test_table_array_float(){
  pvoid plist;
  float array[5]={1,2,3,4,5};
  unsi tot, occupied;
  pfloat pextracted;

  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  if((plist = malloc_list(type_list_table, "FLOAT", 5)) == NULL) printf("Non creata.\n");
  free(plist);
  if((plist = malloc_list(type_list_table, "FLOAT", 5)) == NULL) printf("Non creata.\n");
  insert_last(plist, (all_type)(pvoid)array, 0);
  insert_last(plist, (all_type)(pvoid)array, 0);
  insert_last(plist, (all_type)(pvoid)array, 0);
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  extract_first(plist, (all_type)(pvoid)&pextracted, 0);
  extract_first(plist, (all_type)(pvoid)&pextracted, 0);
  extract_first(plist, (all_type)(pvoid)&pextracted, 0);
  extract_first(plist, (all_type)(pvoid)&pextracted, 0);
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);

  extract_last(plist, (all_type)(pvoid)&pextracted, 0);
  printf("Estratto: ");
  print_array_float((all_type)(pvoid)pextracted, 5);
  printf("\n");

  array[0] = 100;
  insert_last(plist, (all_type)(pvoid)array, 0);
  extract_last(plist, (all_type)(pvoid)&pextracted, 0);
  array[0] = 1000;
  insert_first(plist, (all_type)(pvoid)array, 0);
  array[0] = 100000;
  insert_last(plist, (all_type)(pvoid)array, 0);
  print_list(plist, print_array_float);
  /* c='b'; */
  /* insert_first(plist, (all_type)c, sizeof(float)); */
  /* print_list(plist, print_char); */
  /* extract_first(plist, (all_type)((pvoid)&ex), NULL); */
  /* printf("Estratto %c\n", ex); */
  /* print_list(plist, print_char); */
  for(;;){
    insert_last(plist, (all_type)(pvoid)array, 0);
    extract_last(plist, (all_type)(pvoid)&pextracted, 0);
    free(pextracted);
   }
 }

void compare_generic(){
  pvoid plist_dynamic, plist_table;
  float f=4;
  pfloat pf;
  unsi size;
  clock_t time;
  int i;

  plist_table = malloc_list_specify_table("GENERIC", 1, type_resize_manual, 100010);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_table, (all_type)((pvoid)&f), sizeof(float));
    extract_first(plist_table, (all_type)((pvoid)&pf), &size);
   }
  printf("Impiegato tempo table %ld\n", clock() - time);

  plist_dynamic = malloc_list(type_list_dynamic, "GENERIC", 1);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_dynamic, (all_type)((pvoid)&f), sizeof(float));
    extract_first(plist_dynamic, (all_type)((pvoid)&pf), &size);
   }
  printf("Impiegato tempo dynamic %ld\n", clock() - time);
 }

void compare_array_float(){
  pvoid plist_dynamic, plist_table;
  float f[5]={1,2,3,4,5};
  pfloat pf;
  clock_t time;
  int i;

  plist_table = malloc_list_specify_table("FLOAT", 5, type_resize_manual, 10000);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_table, (all_type)((pvoid)f), 0);
    extract_first(plist_table, (all_type)((pvoid)&pf), 0);
   }
  printf("Impiegato tempo table %ld\n", clock() - time);

  plist_dynamic = malloc_list(type_list_dynamic, "FLOAT", 5);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_dynamic, (all_type)((pvoid)f), 0);
    extract_first(plist_dynamic, (all_type)((pvoid)&pf), 0);
   }
  printf("Impiegato tempo dynamic %ld\n", clock() - time);

 }

int main(int argc, char *argv[]){
  test_table_generic();

  return 0;
 }



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
  printf("%f",*((pfloat)value.pv));
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
  insert_first(plist, (all_type)((float)1), 29);
  insert_first(plist, (all_type)((float)2), 29);
  insert_nth(plist, (all_type)((float) 6.6), 0, 2);
  insert_first(plist, (all_type)((float)3), 29);
  print_list(plist, print_float);
  extract_nth(plist, (all_type)((pvoid)&f), NULL, 3);
  printf("Estratto %f\n", f);
  insert_first(plist, (all_type)((float)3), 29);
  insert_last(plist, (all_type)((float)4), 29);
  insert_nth(plist, (all_type)((float) 6.6), 0, 4);
  print_list(plist, print_float);
  insert_last(plist, (all_type)((float)5), 29);
  insert_last(plist, (all_type)((float)6), 29);
  print_list(plist, print_float);
  extract_last(plist, (all_type)((pvoid)&f), NULL);
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
  parray[0] = 5;
  insert_nth(plist, (all_type)((pvoid)parray), 0, 2);
  parray[0] = 100;
  insert_last(plist, (all_type)((pvoid)parray), 0);
  print_list(plist, print_array_float);
  extract_nth(plist, (all_type)((pvoid)&poutput), NULL, 2);
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
  int i;
  unsi size, tot, occupied;

  if((plist = malloc_list_specify_table("GENERIC", 3, type_resize_default, 10)) == NULL) printf("Non creata.\n");
  if((plistdue = malloc_list_specify_table("GENERIC", 3, type_resize_default, 3)) == NULL) printf("Non creata.\n");
  f=9;
  if((insert_last(plistdue, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  f=99;
  if((insert_last(plistdue, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  change_resize_table(plist, type_resize_manual);
  resize_table(plist, 4);
  print_list(plistdue, print_generic);
  if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  change_resize_table(plist, type_resize_default);
  if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
  extract_last(plist, (all_type)((pvoid)&pf), &size);
  printf("Estratto %f con size %u\n", *pf, size);
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  for (i = 0;; i++) {
    if((insert_last(plist, (all_type)((pvoid)&f), sizeof(float))) == 0) printf("non aggiunto\n");
   }
 }

void test_table_char(){
  pvoid plist, plistdue;
  char  c='a', ex;
  unsi tot, occupied;
  int i;

  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  if((plistdue = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  c='l';
  if((insert_first(plistdue, (all_type)c, 0)) == 0) printf("Non inserito\n");
  c='k';
  if((insert_first(plistdue, (all_type)c, 0)) == 0) printf("Non inserito\n");
  resize_table(plist, 10);
  get_info_table(plist, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  print_list(plistdue, print_char);
  c='b';
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)c, 0)) == 0) printf("Non inserito\n");
   }
  if((insert_last(plist, (all_type)c, 0)) == 0) printf("Non inserito\n");
  c='f';
  if((insert_last(plist, (all_type)c, 0)) == 0) printf("Non inserito\n");
  c='e';
  if((insert_nth(plist, (all_type)c, 0, 3)) == 0) printf("Non inserito\n");
  c='c';
  print_list(plist, print_char);
  extract_nth(plist, (all_type)((pvoid)&ex), NULL, 2);
  print_list(plist, print_char);
  printf("Estratto %c\n", ex);
  free_list(plist);
  if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n");
  c='d';
  if((insert_first(plist, (all_type)c, sizeof(float))) == 0) printf("Non inserito\n");
  if((plistdue = malloc_list_specify_table("CHAR", 1, type_resize_default, 10)) == NULL) printf("Non creata.\n");
  c='e';
  if((insert_last(plistdue, (all_type)c, sizeof(float))) == 0) printf("Non inserito\n");
  c='f';
  if((insert_first(plistdue, (all_type)c, sizeof(float))) == 0) printf("Non inserito ora\n");
  print_list(plist, print_char);
  printf("Ciao\n");
  print_list(plistdue, print_char);
  if((resize_table(plist, 50)) == 0) printf("non resizata\n");
  print_list(plist, print_char);
  printf("Ciao\n");
  print_list(plistdue, print_char);
  get_info_table(plistdue, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  /* if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n"); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* free_list(plist); */
  /* if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n"); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* if((plist = malloc_list_specify_table("CHAR", 1, type_resize_default, 100)) == NULL) printf("Non creata.\n"); */
  /* insert_first(plist, (all_type)c, sizeof(float)); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* /1* print_list(plist, print_char); *1/ */
  /* extract_last(plist, (all_type)((pvoid)&ex), NULL); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
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
  pvoid plist, plistdue;
  float array[5]={1,2,3,4,5};
  unsi tot, occupied;
  int i;
  pfloat pextracted;

  if((plist = malloc_list_specify_table("FLOAT", 5, type_resize_default, 70)) == NULL) printf("Non creata.\n");
  resize_table(plist, 30);
  printf("Resizata\n");
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)((pvoid) array), 0)) == 0) printf("Non inserito\n");
   }
  if((insert_first(plist, (all_type)((pvoid) array), 0)) == 0) printf("Non inserito\n");
  array[0] = 5;
  if((insert_last(plist, (all_type)((pvoid) array), 0)) == 0) printf("Non inserito\n");
  array[0] = 15;
  if((insert_last(plist, (all_type)((pvoid) array), 0)) == 0) printf("Non inserito\n");
  array[0] = 55;
  if((insert_nth(plist, (all_type)((pvoid) array), 0, 2)) == 0) printf("Non inserito\n");
  print_list(plist, print_array_float);
  extract_nth(plist, (all_type)(pvoid)&pextracted, 0, 3);
  printf("Estratto: ");
  print_array_float((all_type)(pvoid)pextracted, 5);
  print_list(plist, print_array_float);
  array[0] = 10;
  if((insert_last(plist, (all_type)((pvoid) array), sizeof(float))) == 0) printf("Non inserito\n");
  if((plistdue = malloc_list(type_list_table, "FLOAT", 5)) == NULL) printf("Non creata.\n");
  array[0] = 100;
  if((insert_last(plistdue, (all_type)((pvoid) array), sizeof(float))) == 0) printf("Non inserito\n");
  array[0] = 1000;
  if((insert_last(plistdue, (all_type)((pvoid)array), sizeof(float))) == 0) printf("Non inserito ora\n");
  print_list(plist, print_array_float);
  printf("Ciao\n");
  print_list(plistdue, print_array_float);
  if((resize_table(plist, 90)) == 0) printf("non resizata\n");
  print_list(plist, print_array_float);
  printf("Ciao\n");
  print_list(plistdue, print_array_float);
  get_info_table(plistdue, &tot, &occupied);
  printf("Totali: %u, occupati: %u\n", tot, occupied);
  for (i = 0;; i++) {
    if((insert_last(plistdue, (all_type)((pvoid)array), sizeof(float))) == 0) break;
   }
  printf("%d\n", i);
  /* if((plist = malloc_list(type_list_table, "GENERIC", 3)) == NULL) printf("Non creata.\n"); */
  /* if((plist = malloc_list(type_list_table, "FLOAT", 5)) == NULL) printf("Non creata.\n"); */
  /* free(plist); */
  /* if((plist = malloc_list(type_list_table, "FLOAT", 5)) == NULL) printf("Non creata.\n"); */
  /* insert_last(plist, (all_type)(pvoid)array, 0); */
  /* insert_last(plist, (all_type)(pvoid)array, 0); */
  /* insert_last(plist, (all_type)(pvoid)array, 0); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */
  /* extract_first(plist, (all_type)(pvoid)&pextracted, 0); */
  /* extract_first(plist, (all_type)(pvoid)&pextracted, 0); */
  /* extract_first(plist, (all_type)(pvoid)&pextracted, 0); */
  /* extract_first(plist, (all_type)(pvoid)&pextracted, 0); */
  /* get_info_table(plist, &tot, &occupied); */
  /* printf("Totali: %u, occupati: %u\n", tot, occupied); */

  /* extract_last(plist, (all_type)(pvoid)&pextracted, 0); */
  /* printf("Estratto: "); */
  /* print_array_float((all_type)(pvoid)pextracted, 5); */
  /* printf("\n"); */

  /* array[0] = 100; */
  /* insert_last(plist, (all_type)(pvoid)array, 0); */
  /* extract_last(plist, (all_type)(pvoid)&pextracted, 0); */
  /* array[0] = 1000; */
  /* insert_first(plist, (all_type)(pvoid)array, 0); */
  /* array[0] = 100000; */
  /* insert_last(plist, (all_type)(pvoid)array, 0); */
  /* print_list(plist, print_array_float); */
  /* c='b'; */
  /* insert_first(plist, (all_type)c, sizeof(float)); */
  /* print_list(plist, print_char); */
  /* extract_first(plist, (all_type)((pvoid)&ex), NULL); */
  /* printf("Estratto %c\n", ex); */
  /* print_list(plist, print_char); */
  /* for(;;){ */
  /*   insert_last(plist, (all_type)(pvoid)array, 0); */
  /*   extract_last(plist, (all_type)(pvoid)&pextracted, 0); */
  /*   free(pextracted); */
  /*  } */
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

void test_table_resize(){
  pvoid plist;
  int i;
  float array[3] = {1,2,3};
  unsi size;
  pfloat pf;

  plist = malloc_list_specify_table("FLOAT", 3, type_resize_manual, 30);
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);
  if(((plist = malloc_list_specify_table("FLOAT", 3, type_resize_manual, 30)) == NULL)) return;
  resize_table(plist, 10);
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);
  if((plist = malloc_list_specify_table("FLOAT", 3, type_resize_manual, 30)) == NULL){
    printf("Non allocata\n");
    return;
   }
  resize_table(plist, 100);
  for (i = 0;; i++) {
    array[0] = i;
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
    extract_first(plist, (all_type)((pvoid)&pf), &size);
    printf("%f\n", *pf);
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);

  plist = malloc_list_specify_table("FLOAT", 5, type_resize_manual, 10);
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);
  if(((plist = malloc_list_specify_table("FLOAT", 3, type_resize_manual, 30)) == NULL)) return;
  resize_table(plist, 100);
  for (i = 0;; i++) {
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);
  if((plist = malloc_list_specify_table("FLOAT", 3, type_resize_manual, 30)) == NULL){
    printf("Non allocata\n");
    return;
   }
  resize_table(plist, 5);
  for (i = 0;; i++) {
    array[0] = -i;
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
    extract_first(plist, (all_type)((pvoid)&pf), &size);
    printf("%f\n", *pf);
    if((insert_first(plist, (all_type)((pvoid)array), sizeof(array))) == 0) break;
   }
  printf("Inserimenti: %d\n", i);
  free_list(plist);
 }

int print_lli(all_type val, unsi size){
  printf("%lli", val.lli);
  return 1;
 }
void test_long_long_int(){
  pvoid plist;
  long long int val = 3;

  plist = malloc_list(type_list_dynamic, "LONG LONG INT", 1);
  insert_first(plist, (all_type) val, 0);
  print_list(plist, print_lli);
  return;
 }

void compare_perf_float(){
  pvoid plist_dynamic, plist_table;
  float f=4;
  clock_t time;
  int i;

  plist_table = malloc_list_specify_table("FLOAT", 1, type_resize_manual, 1010);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_table, (all_type)(f), 0);
    extract_first(plist_table, (all_type)((pvoid)&f), NULL);
   }
  printf("Impiegato tempo table %ld\n", clock() - time);

  plist_dynamic = malloc_list(type_list_dynamic, "FLOAT", 1);
  time = clock();
  for (i = 0; i < 1000000; i++) {
    insert_first(plist_dynamic, (all_type)(f), 0);
    extract_first(plist_dynamic, (all_type)((pvoid)&f), NULL);
   }
  printf("Impiegato tempo dynamic %ld\n", clock() - time);
 }

int main(int argc, char *argv[]){
  test_table_array_float();

  return 0;
 }

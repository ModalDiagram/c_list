#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../list/list.h"

/* #define TIPO_LISTA type_list_dynamic */
#define TIPO_LISTA type_list_table

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
  if(value1.f == value2.f) return 0;
  if(value1.f > value2.f)  return 1;
  return -1;
 }


void test_search_float(){
  pvoid plist;
  float f = 3.0;

  if((plist = malloc_list(TIPO_LISTA, "FLOAT", 1)) == NULL) printf("Non creata.\n");
  insert_first(plist, (all_type)((float)2.4), 29);
  insert_first(plist, (all_type)((float)4.4), 29);
  insert_first(plist, (all_type)((float)5), 29);
  if(!search_first(plist, (all_type)(float)5, 0, (all_type)(pvoid) &f, 0, compare_float)) printf("Non trovato");
  else printf("Trovato %f\n", f);
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

void test_table_resize(){
  pvoid plist, plistdue;
  int   i;
  float f=5;
  unsi  tot, occupied;

  plist = malloc_list_specify_table("FLOAT", 1, type_resize_manual, 1000);
  plistdue = malloc_list_specify_table("FLOAT", 1, type_resize_manual, 1000);
  insert_first(plistdue, (all_type) f, 0);
  get_info_table(plist, &tot, &occupied);
  printf("Dimensione iniziale %u, occupati %u\n", tot, occupied);
  for (i = 0; i < 1100; i++) {
    insert_first(plist, (all_type) f, 0);
  }
  get_info_table(plist, &tot, &occupied);
  printf("Dimensione dopo il for %u, occupati %u\n", tot, occupied);
  free_list(plist);
  printf("Libero la lista più grande\n");
  get_info_table(plistdue, &tot, &occupied);
  printf("Dimensione dopo lista liberata: %u, occupati %u\n", tot, occupied);
  resize_table(plistdue, 5);
  get_info_table(plistdue, &tot, &occupied);
  printf("Dimensione dopo resize: %u, occupati %u\n", tot, occupied);
  printf("Verifichiamo che l'altra lista sia ancora stampabile\n");
  print_list(plistdue, print_float);
 }

void test_inserimenti_ed_estrazioni_generic(){
  pvoid plist;
  float f = 70;
  pfloat pf;

  plist = malloc_list(TIPO_LISTA, "GENERIC", 0);
  printf("Inserisco %f in cima\n", f = 90);
  insert_first(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco %f in coda\n", f = 100);
  insert_last(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco %f in coda\n", f = 110);
  insert_last(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco %f dopo il primo\n", f = 120);
  insert_nth(plist, (all_type)((pvoid) &f), sizeof(float), 2);
  printf("Stampo la lista\n");
  print_list(plist, print_generic);
  f=10000;
  printf("Estraggo il terzo elemento\n");
  extract_nth(plist, (all_type)((pvoid) &pf), NULL, 3);
  printf("Estratto: %f\n", *pf);
  printf("Estraggo il primo elemento\n");
  extract_first(plist, (all_type)((pvoid) &pf), NULL);
  printf("Estratto: %f\n", *pf);
  printf("Stampo la lista\n");
  print_list(plist, print_generic);
 }

void test_inserimenti_ed_estrazioni_float(){
  pvoid plist;
  float f = 70;

  plist = malloc_list(TIPO_LISTA, "FLOAT", 1);
  printf("Inserisco %f in cima\n", f = 90);
  insert_first(plist, (all_type)(f), sizeof(float));
  printf("Inserisco %f in coda\n", f = 100);
  insert_last(plist, (all_type)(f), sizeof(float));
  printf("Inserisco %f in coda\n", f = 110);
  insert_last(plist, (all_type)(f), sizeof(float));
  printf("Inserisco %f dopo il primo\n", f = 120);
  insert_nth(plist, (all_type)(f), sizeof(float), 2);
  printf("Stampo la lista\n");
  print_list(plist, print_float);
  f=10000;
  printf("Estraggo il terzo elemento\n");
  extract_nth(plist, (all_type)((pvoid) &f), NULL, 3);
  printf("Estratto: %f\n", f);
  printf("Estraggo il primo elemento\n");
  extract_first(plist, (all_type)((pvoid) &f), NULL);
  printf("Estratto: %f\n", f);
  printf("Stampo la lista\n");
  print_list(plist, print_float);
 }

void test_inserimenti_ed_estrazioni_array(){
  pvoid plist;
  float f[3] = {1,2,3};
  pfloat pf;

  plist = malloc_list(TIPO_LISTA, "FLOAT", 3);
  printf("Inserisco array con f[0] = %f in cima\n", f[0] = 50);
  insert_first(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco array con f[0] = %f in coda\n", f[0] = 250);
  insert_last(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco array con f[0] = %f in coda\n", f[0] = 450);
  insert_last(plist, (all_type)((pvoid) &f), sizeof(float));
  printf("Inserisco array con f[0] = %f dopo il primo\n", f[0] = 650);
  insert_nth(plist, (all_type)((pvoid) &f), sizeof(float), 2);
  printf("Stampo la lista\n");
  print_list(plist, print_array_float);
  printf("Estraggo il terzo elemento\n");
  extract_nth(plist, (all_type)((pvoid) &pf), NULL, 3);
  printf("Estratto array con f[0]: %f\n", pf[0]);
  printf("Estraggo il primo elemento\n");
  extract_first(plist, (all_type)((pvoid) &pf), NULL);
  printf("Estratto array con f[0]: %f\n", pf[0]);
  printf("Stampo la lista\n");
  print_list(plist, print_generic);
 }

int main(int argc, char *argv[])
{
  test_inserimenti_ed_estrazioni_array();
  return 0;
}

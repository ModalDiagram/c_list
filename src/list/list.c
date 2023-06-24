#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "list.hidden"
#include "../util/defines_typedef.h"
#include "../all_type/define_all_type.h"
#include "./list_table/list_table_generic/list_table_generic.h"
#include "./list_table/list_table_array_char/list_table_array_char.h"
#include "./list_table/list_table_array_int/list_table_array_int.h"
#include "./list_table/list_table_array_float/list_table_array_float.h"
#include "./list_table/list_table_array_double/list_table_array_double.h"
#include "./list_table/list_table_array_long/list_table_array_long.h"
#include "./list_table/list_table_char/list_table_char.h"
#include "./list_table/list_table_int/list_table_int.h"
#include "./list_table/list_table_float/list_table_float.h"
#include "./list_table/list_table_double/list_table_double.h"
#include "./list_table/list_table_long/list_table_long.h"

#include "./list_dynamic/list_dynamic_generic/list_dynamic_generic.h"
#include "./list_dynamic/list_dynamic_array_char/list_dynamic_array_char.h"
#include "./list_dynamic/list_dynamic_array_int/list_dynamic_array_int.h"
#include "./list_dynamic/list_dynamic_array_float/list_dynamic_array_float.h"
#include "./list_dynamic/list_dynamic_array_double/list_dynamic_array_double.h"
#include "./list_dynamic/list_dynamic_array_long/list_dynamic_array_long.h"
#include "./list_dynamic/list_dynamic_char/list_dynamic_char.h"
#include "./list_dynamic/list_dynamic_int/list_dynamic_int.h"
#include "./list_dynamic/list_dynamic_float/list_dynamic_float.h"
#include "./list_dynamic/list_dynamic_double/list_dynamic_double.h"
#include "./list_dynamic/list_dynamic_long/list_dynamic_long.h"

CREA_ARRAY(malloc_list)
CREA_ARRAY(malloc_list_with_resize)
CREA_ARRAY(change_resize_table)
CREA_ARRAY(free_list)
CREA_ARRAY(insert_first)
CREA_ARRAY(extract_first)
CREA_ARRAY(search_first)
CREA_ARRAY(get_max)
CREA_ARRAY(sort_list)
CREA_ARRAY(print_list)

/* Sono fornite le seguenti funzioni membro: */

/* malloc_list: istanzia una nuova lista che puo' contenere dati dei tipi:
 *              "CHAR", "INT", "FLOAT", "DOUBLE", "ADDRESS", "GENERIC",
 *              o loro array.
 * type_list:   tipo di lista da instanziare, tra:
 *              - type_list_dynamic
 *              - type_list_table
 * type_string: stringa che descrive il tipo di dato che la lista contiene. Deve
 *              essere una delle stringhe riportate sopra.
 * dim_array:   quanti valori deve contenere ciascun elemento della lista.
 *              Si inserisca ad esempio 1 per avere una lista con tanti elementi
 *              quanto sono i valori inseriti.
 *              Le liste con vettori sono piu' veloci a parita' di dati, ma
 *              i vettori devono avere tutti la stessa dimensione
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list(type_list type_list, pchar type_string, unsi dim_array){
    type_list_data type_new_list;
    pmy_list pnew_list = (pmy_list) malloc(sizeof(my_list));

    if(type_list == type_list_dynamic){
        if(!(strcmp(type_string, "INT"))){
            if(dim_array == 1){type_new_list = type_dynamic_int;}
            else {type_new_list = type_dynamic_array_int;}
          }
        else if(!(strcmp(type_string, "CHAR"))){
            if(dim_array == 1){type_new_list = type_dynamic_char;}
            else {type_new_list = type_dynamic_array_char;}
          }
        else if(!(strcmp(type_string, "FLOAT"))){
            if(dim_array == 1){type_new_list = type_dynamic_float;}
            else {type_new_list = type_dynamic_array_float;}
          }
        else if(!(strcmp(type_string, "DOUBLE"))){
            if(dim_array == 1){type_new_list = type_dynamic_double;}
            else {type_new_list = type_dynamic_array_double;}
          }
        else if(!(strcmp(type_string, "PVOID"))){
            if(dim_array == 1){type_new_list = type_dynamic_long;}
            else {type_new_list = type_dynamic_array_long;}
          }
        else if(!(strcmp(type_string, "GENERIC"))){
            type_new_list = type_dynamic_generic;
          }
        else return NULL;
      }
    else if(type_list == type_list_table){
        if(!(strcmp(type_string, "INT"))){
            if(dim_array == 1){type_new_list = type_table_int;}
            else {type_new_list = type_table_array_int;}
          }
        else if(!(strcmp(type_string, "CHAR"))){
            if(dim_array == 1){type_new_list = type_table_char;}
            else {type_new_list = type_table_array_char;}
          }
        else if(!(strcmp(type_string, "FLOAT"))){
            if(dim_array == 1){type_new_list = type_table_float;}
            else {type_new_list = type_table_array_float;}
          }
        else if(!(strcmp(type_string, "DOUBLE"))){
            if(dim_array == 1){type_new_list = type_table_double;}
            else {type_new_list = type_table_array_double;}
          }
        else if(!(strcmp(type_string, "PVOID"))){
            if(dim_array == 1){type_new_list = type_table_long;}
            else {type_new_list = type_table_array_long;}
          }
        else if(!(strcmp(type_string, "GENERIC"))){
            type_new_list = type_table_generic;
          }
        else return NULL;
      }
    else return NULL;
    if((pnew_list->plist = malloc_list_arr[type_new_list](dim_array)) == NULL){
        free(pnew_list);
        return NULL;
      }

    pnew_list->tlist = type_new_list;
    return pnew_list;
  }

/* malloc_list_with_resize: crea una nuova lista come sopra, e specifica il tipo di
 *                          resize della tabella che contiene la lista.
 * type_resize:             tipo di resize della tabella. Di default è type_resize_default,
 *                          ma puo' essere selezionato tra:
 *                          - type_resize_default: la tabella si espande automaticamente
 *                          quando piena
 *                          - type_resize_manual: le funzioni di inserimento tornano
 *                          errore quando la tabella e' piena. Essa va espansa manualmente
 *                          con la funzione expand_table
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list_with_resize(type_list type_list, pchar type_string, unsi dim_array, type_resize type_resize){
    return NULL;
  }

/* change_resize_table: cambia il tipo di resize della tabella che contiene plist.
 * type_resize:         tipo di resize da impostare per la tabella. Puo' essere:
 *                      - type_resize_default: la table viene ampliata automaticamente
 *                                             una volta riempita
 *                      - type_resize_manual:  la table torna errore quando si cerca di
 *                                             inserire elementi oltre la sua capienza.
 *                                             Deve essere ampliata manualmente
 *                                             attraverso shrink_table
 *
 * */
pvoid change_resize_table(pvoid plist, type_resize type_resize){
  return NULL;
}

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list(pvoid plist){
    return;
  }

/* get_type_list: questa funzione permette di scoprire il tipo della lista e
 *                del dato contenuto
 * plist:         lista di cui si vuole conoscere il tipo
 *
 * return:        stringa del tipo:
 *                "DINAMICA INT"
 *                "DINAMICA ARRAY INT"
 *                "TABLE INT"
 * */
pchar get_type_list(pvoid plist){
    return 0;
  }

/* insert_first: inserisce un elemento in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * value:        elemento da inserire, da castare a (ALL_TYPE)
 * size:         deve essere rispettivamente:
 *               - type_data_generic: dimensione del dato da inserire
 *               - altri:             non ha importanza
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 * */
int insert_first(pvoid plist, ALL_TYPE value, unsi size){
    pmy_list plist_casted = (pmy_list) plist;

    return insert_first_arr[plist_casted->tlist](plist_casted->plist, value, size);
  }

/* extract_first: estrae l'elemento in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * pvalue:        indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: indirizzo dell'elemento estratto
 *                - type_data_array_*: indirizzo dell'array estratto
 *                - altri:             valore dell'elemento estratto
 *                pvalue va castato a (ALL_TYPE*) per evitare warning
 * psize:         indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: size dell'elemento estratto
 *                - type_data_array_*: numero di elementi dell'array estratto
 *                - altri:             niente
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int extract_first(pvoid plist, ALL_TYPE pvalue, punsi psize){
    pmy_list plist_casted = (pmy_list) plist;

    return extract_first_arr[plist_casted->tlist](plist_casted->plist, pvalue, psize);
  }

/* search_first:   ritorna la prima occorrenza dell'elemento cercato (cioe' il primo
 *                 elemento della lista uguale a quello fornito in input
 *                 secondo pinput_compare)
 * paddr_searched: indirizzo dell'elemento da confrontare
 * size_searched:  rapresenta rispettivamente:
 *                 - type_data_generic: dimensione dell'elemento da confrontare
 *                 - type_data_array_*: numero di elem. dell'array da confrontare
 * ppaddr_found:   indirizzo in cui scrivere l'indirizzo dell'elemento trovato
 * psize_found:    indirizzo in cui scrivere la size dell'elemento trovato
 *                 (size intesa nello stesso modo di size_searched)
 * pinput_compare: funzione con cui comparare due elementi della lista.
 *                 E' una funzione del tipo:
 *                 int (*pcustom_compare)(pvoid pvalue1, unsi size1, pvoid pvalue2, unsi size2, pint presult);
 *                 Deve scrivere in presult 0 se sono uguali
 *
 * Torna 1 se lo ha trovato, 0 altrimenti
 *
 * NB: pinput_compare puo' essere NULL, e in quel caso sara' utilizzata la plist_compare
 * fornita con la funzione add_functions se presente
 * */
int search_first(pvoid plist,
                 pvoid  paddr_searched, unsi  size_searched,
                 ppvoid ppaddr_found,   punsi psize_found,
                 pcustom_compare pinput_compare){
    return 0;
  }

/* get_max:        trova il massimo della lista (cioe' l'elemento che e' piu' grande di
 *                 tutti gli altri secondo pinput_compare)
 * ppaddr_max:     indirizzo in cui scrivere l'indirizzo dell'elemento massimo
 * psize_max:      indirizzo in cui scrivere rispettivamente:
 *                 - type_data_generic: dimensione dell'elemento massimo
 *                 - type_data_array_*: numero di elem. dell'array massimo
 * pinput_compare: funzione con cui comparare due elementi della lista.
 *                 E' una funzione del tipo:
 *                 int (*pcustom_compare)(pvoid pvalue1, unsi size1, pvoid pvalue2, unsi size2, pint presult);
 *                 Deve scrivere in presult:
 *                 - 0  se sono uguali
 *                 - >0 se il primo è maggiore
 *                 - <0 se il secondo è maggiore
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 *
 * NB: pinput_compare puo' essere NULL, e in quel caso sara' utilizzata la plist_compare
 * fornita con la funzione add_functions se presente
 * */
int get_max(pvoid plist, ppvoid ppaddr_max, punsi psize_max, pcustom_compare pinput_compare){
    return 0;
  }

/* sort_list:      ordina la lista secondo pinput_compare
 * plist:          lista da ordinare
 * pinput_compare: funzione con cui comparare due elementi della lista.
 *                 E' una funzione del tipo:
 *                 int (*pcustom_compare)(pvoid pvalue1, unsi size1, pvoid pvalue2, unsi size2, pint presult);
 *                 Deve scrivere in presult:
 *                 - 0  se sono uguali
 *                 - >0 se il primo è maggiore
 *                 - <0 se il secondo è maggiore
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 *
 * NB: pinput_compare puo' essere NULL, e in quel caso sara' utilizzata la plist_compare
 * fornita con la funzione add_functions se presente
 * */
int sort_list(pvoid plist, pcustom_compare pinput_compare){
    return 0;
  }

/* print_list:    stampa la lista e le sue proprieta' nel formato
 *                type_list: ...
 *                type_data: ...
 *                Numero di elementi: ...
 *                Lista: v1->v2->v3->...
 *                dove vi e' il valore contenuto nell'i-esimo elemento della lista.
 * plist:         lista da stampare
 *
 * pinput_print: funzione con cui stampare il valore contenuto in un elemento della lista.
 *               E' una funzione del tipo
 *               int (*pcustom_print)(pvoid pvalue, unsi size)
 *               che deve tornare 1 se stampa bene o 0 altrimenti.
 *
 * NB: pinput_print puo' essere NULL e in questo caso
 *     sara' utilizzata la pcustom_print fornita con add_functions, o stampata nel
 *     formato
 *     v->v->v->...
 *     se pcustom_print non è stata fornita precedentemente (per i tipi base
 *     sono fornite delle pinput_print di default)
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 * */
int print_list(pvoid plist, pcustom_print pinput_print){
    pmy_list plist_casted = (pmy_list) plist;

    return print_list_arr[plist_casted->tlist](plist_casted->plist, pinput_print);
  }

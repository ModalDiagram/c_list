#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_dynamic_array_BASETYPE.h"
#include "list_dynamic_array_BASETYPE.hidden"

#ifndef GET_PNEXT
#define GET_PNEXT(pvalue) (*((ppvoid)(((pBASETYPE) pvalue )+size_array)))
#endif

static unsi size_type = sizeof(BASETYPE);

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
pvoid malloc_list_dynamic_array_BASETYPE(unsi dim_array){
  plist_dynamic_array_BASETYPE pnew_list;

  if((pnew_list = malloc(sizeof(list_dynamic_array_BASETYPE))) == NULL) return NULL;
  pnew_list->pstart = pnew_list->pend = NULL;
  pnew_list->n_elem = 0;
  pnew_list->size_array = dim_array;
  return pnew_list;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_dynamic_array_BASETYPE(pvoid plist){
  plist_dynamic_array_BASETYPE plist_casted = (plist_dynamic_array_BASETYPE) plist;
  pvoid pelem_moving, pelem_tmp;
  unsi size_array = plist_casted->size_array;

  pelem_moving = plist_casted->pstart;
  if(pelem_moving != NULL){
    pelem_tmp = GET_PNEXT(pelem_moving);
    while(pelem_tmp != NULL){
      free(pelem_moving);
      pelem_moving = pelem_tmp;
      pelem_tmp = GET_PNEXT(pelem_moving);
     }
    free(pelem_moving);
   }
  free(plist_casted);

  return;
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
int insert_first_dynamic_array_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_dynamic_array_BASETYPE plist_casted;
  pvoid pnew_elem;
  unsi  size_array;

  plist_casted = (plist_dynamic_array_BASETYPE) plist;
  size_array = plist_casted->size_array;
  if((pnew_elem = malloc(size_array * size_type + sizeof(pvoid))) == NULL) return 0;

  memcpy(pnew_elem, value.pv, size_array*size_type);

  GET_PNEXT(pnew_elem) = plist_casted->pstart;
  plist_casted->pstart = pnew_elem;

  if(!((plist_casted->n_elem)++)) plist_casted->pend = pnew_elem;

  return 1;
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
int extract_first_dynamic_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  plist_dynamic_array_BASETYPE plist_casted = (plist_dynamic_array_BASETYPE) plist;
  pvoid pelem_to_remove;
  ppvoid ppvalue_input = pvalue.pv;
  unsi size_array = plist_casted->size_array;

  if(plist_casted->n_elem == 0){
    return 0;
   }

  pelem_to_remove = plist_casted->pstart;
  plist_casted->pstart = GET_PNEXT(pelem_to_remove);

  if((*ppvalue_input = malloc(size_array*size_type)) == NULL) return 0;
  memcpy(*ppvalue_input, pelem_to_remove, size_array*size_type);
  free(pelem_to_remove);

  if(!(--(plist_casted->n_elem))) plist_casted->pend = NULL;

  return 1;
 }

/* insert_last: inserisce un elemento in coda alla lista.
 * plist:        lista al cui termine inserire l'elemento
 * value:        elemento da inserire
 * size:         deve essere rispettivamente:
 *               - type_data_generic: dimensione del dato da inserire
 *               - altri:             non ha importanza
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * NB: per evitare errori, value deve essere del tipo appropriato e castato a (all_type), ad esempio
 * - insert_last(mia_lista_double, (all_type)(2.4), 0)
 * - insert_last(mia_lista_generic, (all_type)((pvoid)&var_da_inserire), sizeof(var_da_inserire))
 * */
int insert_last_dynamic_array_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_dynamic_array_BASETYPE plist_casted;
  pvoid pnew_elem;
  unsi  size_array;

  plist_casted = (plist_dynamic_array_BASETYPE) plist;
  size_array = plist_casted->size_array;
  if((pnew_elem = malloc(size_array * size_type + sizeof(pvoid))) == NULL) return 0;

  memcpy(pnew_elem, value.pv, size_array*size_type);

  GET_PNEXT(pnew_elem) = NULL;
  if(plist_casted->pend != NULL){
    GET_PNEXT(plist_casted->pend) = pnew_elem;
   }
  plist_casted->pend = pnew_elem;

  if(!((plist_casted->n_elem)++)) plist_casted->pstart = pnew_elem;

  return 1;
 }

/* extract_last: estrae l'elemento in coda alla lista
 * plist:         lista dal cui termine estrarre l'elemento
 * pvalue:        indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: indirizzo dell'elemento estratto
 *                - type_data_array_*: indirizzo dell'array estratto
 *                - altri:             valore dell'elemento estratto
 * psize:         indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: size dell'elemento estratto
 *                - type_data_array_*: numero di elementi dell'array estratto
 *                - altri:             niente
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 *
 * NB: per evitare errori, value deve essere del tipo appropriato e castato a (all_type), ad esempio
 * - extract_last(mia_lista_double, (all_type)((pvoid) &d), 0)
 * dove d e' la variabile in cui salvare il valore estratto. psize non e' importante
 * in questo caso dato che si tratta di una lista di double e non generic.
 *
 * */
int extract_last_dynamic_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  plist_dynamic_array_BASETYPE plist_casted = (plist_dynamic_array_BASETYPE) plist;
  pvoid         pelem_moving, pelem_tmp=NULL;
  ppvoid        ppvalue_input = pvalue.pv;
  unsi          size_array = plist_casted->size_array;

  if(plist_casted->n_elem == 0) return 0;

  /* raggiungo l'ultimo elemento con pelem_moving e conservo in pelem_tmp il penultimo */
  pelem_moving = plist_casted->pstart;
  while(GET_PNEXT(pelem_moving) != NULL){
    pelem_tmp = pelem_moving;
    pelem_moving = GET_PNEXT(pelem_moving);
   }

  /* restituisco il valore estratto e libero l'elemento */
  if((*ppvalue_input = malloc(size_array*size_type)) == NULL) return 0;
  memcpy(*ppvalue_input, pelem_moving, size_array*size_type);

  free(pelem_moving);
  /* cambio il pnext del penultimo solo se questo e' diverso da NULL (che e'
   * il caso in cui la lista ha un solo elemento) */
  if(pelem_tmp != NULL){
    GET_PNEXT(pelem_tmp) = NULL;
   }

  /* aggiorno le informazioni della lista */
  plist_casted->pend = pelem_tmp;
  if(!(--(plist_casted->n_elem))) plist_casted->pstart = NULL;

  return 1;
 }

/* insert_nth: inserisce un elemento all'n-esima posizione della lista
 * plist:      lista in cui inserire l'elemento
 * value:      elemento da inserire
 * size:       deve essere rispettivamente:
 *             - type_data_generic: dimensione del dato da inserire
 *             - altri:             non ha importanza
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * NB: per evitare errori, value deve essere del tipo appropriato e castato a (all_type), ad esempio
 * - insert_nth(mia_lista_double, (all_type)(2.4), 0, 3)
 * - insert_nth(mia_lista_generic, (all_type)((pvoid)&var_da_inserire), sizeof(var_da_inserire), 3)
 *
 * NB2: le posizioni sono contate a partire da 1, n=1 vuol dire inserire l'elemento
 *      in cima alla lista, n=2 dopo il primo elemento e cosi' via
 * */
int insert_nth_dynamic_array_BASETYPE(pvoid plist, all_type value, unsi size, unsi n){
  return 0;
 }

/* extract_nth: estrae l'elemento all'n-esima posizione della lista
 * plist:         lista da cui estrarre l'elemento
 * pvalue:        indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: indirizzo dell'elemento estratto
 *                - type_data_array_*: indirizzo dell'array estratto
 *                - altri:             valore dell'elemento estratto
 * psize:         indirizzo in cui verra' scritto rispettivamente:
 *                - type_data_generic: size dell'elemento estratto
 *                - type_data_array_*: numero di elementi dell'array estratto
 *                - altri:             niente
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 *
 * NB: per evitare errori, value deve essere del tipo appropriato e castato a (all_type), ad esempio
 * - extract_first(mia_lista_double, (all_type)((pvoid) &d), 0)
 * dove d e' la variabile in cui salvare il valore estratto. psize non e' importante
 * in questo caso dato che si tratta di una lista di double e non generic.
 *
 * NB2: le posizioni sono contate a partire da 1, n=1 vuol dire estrarre l'elemento
 *      in cima alla lista, n=2 dopo il primo elemento e cosi' via
 * */
int extract_nth_dynamic_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize, unsi n){
  return 0;
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
int search_first_dynamic_array_BASETYPE(pvoid plist,
                         all_type value_searched, unsi size_searched,
                         all_type pvalue_found,   punsi psize_found,
                         pcustom_compare pinput_compare){
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
int print_list_dynamic_array_BASETYPE(pvoid plist, pcustom_print pinput_print){
  plist_dynamic_array_BASETYPE plist_casted = (plist_dynamic_array_BASETYPE) plist;
  pvoid pelem_moving = plist_casted->pstart;
  unsi size_array = plist_casted->size_array;
  int i = 0;

  printf("type_list: type_list_dynamic\n");
  printf("type_data: array BASETYPE\n");
  printf("Numero di elementi degli array contenuti: %u\n", plist_casted->size_array);
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  if (pelem_moving == NULL) return 1;

  if(pinput_print != NULL){
    pinput_print((all_type) pelem_moving, size_array);
    pelem_moving = GET_PNEXT(pelem_moving);
    while(pelem_moving != NULL){
      i++;
      if(i == 5) break;
      printf("->");
      pinput_print((all_type)pelem_moving, size_array);
      pelem_moving = GET_PNEXT(pelem_moving);
     }
    printf("\n");
   }

  return 1;
 }

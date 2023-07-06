#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_dynamic_generic.h"
#include "list_dynamic_generic.hidden"

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
pvoid malloc_list_dynamic_generic(unsi dim_array){
  plist_dynamic_generic pnew_list;

  if((pnew_list = malloc(sizeof(list_dynamic_generic))) == NULL) return NULL;
  pnew_list->pstart = pnew_list->pend = NULL;
  pnew_list->n_elem = 0;
  return pnew_list;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_dynamic_generic(pvoid plist){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic         pelem_moving, pelem_tmp;

  /* scorro lungo la lista e libero il valore dell'elemento generic e
   * l'elemento in se della lista */
  pelem_moving = plist_casted->pstart;
  if(pelem_moving != NULL){
    pelem_tmp = pelem_moving->pnext;
    while(pelem_tmp != NULL){
      free(pelem_moving->paddr);
      free(pelem_moving);
      pelem_moving = pelem_tmp;
      pelem_tmp = pelem_moving->pnext;
     }
    free(pelem_moving->paddr);
    free(pelem_moving);
   }
  /* infine libero l'intera struttura */
  free(plist_casted);

  return;
 }

/* insert_first_dynamic_generic: inserisce un elemento di tipo generic in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * value:        indirizzo dell'elemento da inserire
 * size:         size dell'elemento generic da inserire
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 * */
int insert_first_dynamic_generic(pvoid plist, all_type value, unsi size){
  plist_dynamic_generic plist_casted;
  pelem_generic         pnew_elem;

  /* alloco il nuovo elemento e salvo il valore preso in input */
  plist_casted = (plist_dynamic_generic) plist;
  if((pnew_elem = malloc(sizeof(elem_generic))) == NULL) return 0;
  if((pnew_elem->paddr = (pvoid) malloc(size)) == NULL){
    free(pnew_elem);
    return 0;
   }
  memcpy(pnew_elem->paddr, value.pv, (pnew_elem->size) = size);

  /* lo metto in cima alla lista */
  pnew_elem->pnext = plist_casted->pstart;
  plist_casted->pstart = pnew_elem;

  /* se e' il primo elemento devo anche cambiare pend */
  if(!((plist_casted->n_elem)++)) plist_casted->pend = pnew_elem;

  return 1;
 }

/* extract_first_dynamic_generic: estrae l'elemento di tipo generic in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * pvalue:        indirizzo in cui verra' scritto l'indirizzo dell'elemento estratto
 * psize:         indirizzo in cui verra' scritto rispettivamente la size dell'elemento estratto
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int extract_first_dynamic_generic(pvoid plist, all_type pvalue, punsi psize){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic         pelem_to_remove;
  ppvoid                ppvalue_input = pvalue.pv;

  if(plist_casted->n_elem == 0) return 0;

  /* salvo l'indirizzo dell'elemento da rimuovere e sposto l'inizio della lista */
  pelem_to_remove = plist_casted->pstart;
  plist_casted->pstart = pelem_to_remove->pnext;

  /* restituisco il valore contenuto nell'elemento e lo libero */
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_to_remove->paddr;
   }
  if(psize != NULL){
    *psize = pelem_to_remove->size;
   }
  free(pelem_to_remove);

  /* se era l'ultimo elemento devo anche aggiornare pend */
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
int insert_last_dynamic_generic(pvoid plist, all_type value, unsi size){
  plist_dynamic_generic plist_casted;
  pelem_generic pnew_elem;

  plist_casted = (plist_dynamic_generic) plist;

  /* Alloco nuovo elemento */
  if((pnew_elem = malloc(sizeof(elem_generic))) == NULL) return 0;
  /* Scrivo il valore */
  if((pnew_elem->paddr = (pvoid) malloc(size)) == NULL){
    free(pnew_elem);
    return 0;
   }
  memcpy(pnew_elem->paddr, value.pv, (pnew_elem->size) = size);

  /* Scrivo pnext */
  pnew_elem->pnext = NULL;

  /* Aggiorno informazioni */
  if(plist_casted->pend != NULL){
    plist_casted->pend->pnext = pnew_elem;
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
int extract_last_dynamic_generic(pvoid plist, all_type pvalue, punsi psize){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic         pelem_moving, pelem_tmp=NULL;
  ppvoid                ppvalue_input = pvalue.pv;

  if(plist_casted->n_elem == 0) return 0;

  /* raggiungo l'ultimo elemento con pelem_moving e conservo in pelem_tmp il penultimo */
  pelem_moving = plist_casted->pstart;
  while(pelem_moving->pnext != NULL){
    pelem_tmp = pelem_moving;
    pelem_moving = pelem_moving->pnext;
   }

  /* restituisco il valore estratto e libero l'elemento */
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_moving->paddr;
   }
  if(psize != NULL){
    *psize = pelem_moving->size;
   }
  free(pelem_moving);
  /* cambio il pnext del penultimo solo se questo e' diverso da NULL (che e'
   * il caso in cui la lista ha un solo elemento) */
  if(pelem_tmp != NULL){
    pelem_tmp->pnext = NULL;
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
int insert_nth_dynamic_generic(pvoid plist, all_type value, unsi size, unsi n){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic pnew_elem, pelem_moving;
  int           i;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem + 1)) return 0;
  if(n == (plist_casted->n_elem + 1)){
    return insert_last(plist, value, size);
   }
  if(n == 1){
    return insert_first(plist, value, size);
   }

  /* alloco nuovo elemento */
  if((pnew_elem = (pelem_generic) malloc(sizeof(elem_generic))) == NULL) return 0;

  /* salvo informazioni in input nell'elemento */
  if((pnew_elem->paddr = (pvoid) malloc(size)) == NULL){
    free(pnew_elem);
    return 0;
   }
  memcpy(pnew_elem->paddr, value.pv, (pnew_elem->size) = size);

  /* raggiungo l'(n-1)esimo elemento */
  for (i = 2, pelem_moving = plist_casted->pstart; i < n; i++) {
    pelem_moving = pelem_moving->pnext;
   }

  /* metto il nuovo elemento tra l'(n-1)-esimo e l'n-esimo */
  pnew_elem->pnext = pelem_moving->pnext;
  pelem_moving->pnext = pnew_elem;

  (plist_casted->n_elem)++;
  return 1;
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
int extract_nth_dynamic_generic(pvoid plist, all_type pvalue, punsi psize, unsi n){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic pelem_to_extract, pelem_moving;
  int           i;
  ppvoid        ppvalue_input = pvalue.pv;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem)) return 0;
  if(n == (plist_casted->n_elem)){
    return extract_last(plist, pvalue, psize);
   }
  if(n == 1){
    return extract_first(plist, pvalue, psize);
   }

  /* raggiungo l'(n-1)esimo elemento con pelem_moving */
  for (i = 2, pelem_moving = plist_casted->pstart; i < n; i++) {
    pelem_moving = pelem_moving->pnext;
  }

  /* pelem_to_extract e' l'n-esimo elemento, cioe' quello da estrarre */
  pelem_to_extract = pelem_moving->pnext;

  /* restituisco il valore contenuto in pelem_to_extract */
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_to_extract->paddr;
   }
  if(psize != NULL){
    *psize = pelem_to_extract->size;
   }

  /* sposto il puntatore dell'(n-1)-esimo elemento e libero l'n-esimo */
  pelem_moving->pnext = pelem_to_extract->pnext;
  free(pelem_to_extract);

  (plist_casted->n_elem)--;

  return 1;
 }

/* get_n_elem: restituisce il numero di elementi della lista
 * plist:      lista di cui si vuole conoscere il numero di elementi
 *
 * return:     numero di elementi della lista
 * */
unsi get_n_elem_dynamic_generic(pvoid plist){
  return ((plist_dynamic_generic) plist)->n_elem;
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
int search_first_dynamic_generic(pvoid plist,
                         all_type value_searched, unsi size_searched,
                         all_type pvalue_found,   punsi psize_found,
                         pcustom_compare pinput_compare){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic         pelem_moving;

  if (pinput_compare == NULL) return 0;
  /* scorro lungo la lista e applico pinput_compare a ciascun elemento e quello cercato.
   * Se trovo un match (pinput_compare ritorna 0) scrivo l'elemento trovato ed esco */
  pelem_moving = plist_casted->pstart;
  while(pelem_moving != NULL){
    if(!pinput_compare(value_searched, size_searched,(all_type)pelem_moving->paddr, pelem_moving->size)){
      *((ppvoid)(pvalue_found.pv)) = pelem_moving->paddr;
      *psize_found = pelem_moving->size;
      return 1;
     }
    pelem_moving = pelem_moving->pnext;
   }
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
int print_list_dynamic_generic(pvoid plist, pcustom_print pinput_print){
  plist_dynamic_generic plist_casted = (plist_dynamic_generic) plist;
  pelem_generic pelem_moving = plist_casted->pstart;
  int i=0;

  printf("type_list: type_list_dynamic\n");
  printf("type_data: generic\n");
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  if (plist_casted->pstart == NULL) return 1;
  if(pinput_print != NULL){
    printf("Stampo i primi 5 elementi:\n");
    pinput_print((all_type)(pelem_moving->paddr), 0);
    pelem_moving = pelem_moving->pnext;
    while(pelem_moving != NULL){
      i++;
      if(i == 5) break;
      printf("->");
      pinput_print((all_type)(pelem_moving->paddr), 0);
      pelem_moving = pelem_moving->pnext;
     }
    printf("\n");
   }

  return 1;
 }

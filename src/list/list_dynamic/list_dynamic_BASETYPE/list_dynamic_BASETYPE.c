#include <stdio.h>
#include <stdlib.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_dynamic_BASETYPE.h"
#include "list_dynamic_BASETYPE.hidden"

/* Sono fornite le seguenti funzioni membro: */

/* malloc_list_dynamic_BASETYPE: istanzia una nuova lista di tipo type_list_dynamic
 *              che contiene elementi di tipo BASETYPE
 * dim_array:   non viene utilizzato in questo caso perche' la lista contiene un
 *              solo elemento, ma il parametro serve per funzioni di altre classi
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list_dynamic_BASETYPE(unsi dim_array){
  plist_dynamic_BASETYPE pnew_list;

  if((pnew_list = malloc(sizeof(list_dynamic_BASETYPE))) == NULL) return NULL;
  pnew_list->pstart = pnew_list->pend = NULL;
  pnew_list->n_elem = 0;
  return pnew_list;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_dynamic_BASETYPE(pvoid plist){
  plist_dynamic_BASETYPE plist_casted = (plist_dynamic_BASETYPE) plist;
  pelem_BASETYPE         pelem_moving, pelem_tmp;

  pelem_moving = plist_casted->pstart;
  if(pelem_moving != NULL){
    pelem_tmp = pelem_moving->pnext;
    while(pelem_tmp != NULL){
      free(pelem_moving);
      pelem_moving = pelem_tmp;
      pelem_tmp = pelem_moving->pnext;
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
int insert_first_dynamic_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_dynamic_BASETYPE plist_casted;
  pelem_BASETYPE pnew_elem;

  plist_casted = (plist_dynamic_BASETYPE) plist;
  if((pnew_elem = malloc(sizeof(elem_BASETYPE))) == NULL) return 0;

  pnew_elem->pnext = plist_casted->pstart;
  plist_casted->pstart = pnew_elem;

  pnew_elem->val = value.MEMBERTYPE;

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
int extract_first_dynamic_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  plist_dynamic_BASETYPE plist_casted = (plist_dynamic_BASETYPE) plist;
  pelem_BASETYPE pelem_to_remove;
  pBASETYPE pvalue_input = (pBASETYPE) pvalue.pv;

  pelem_to_remove = plist_casted->pstart;
  plist_casted->pstart = pelem_to_remove->pnext;

  (*pvalue_input) = pelem_to_remove->val;
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
int insert_last_dynamic_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_dynamic_BASETYPE plist_casted;
  pelem_BASETYPE pnew_elem;
  plist_casted = (plist_dynamic_BASETYPE) plist;

  /* Alloco nuovo elemento */
  if((pnew_elem = malloc(sizeof(elem_BASETYPE))) == NULL) return 0;
  /* Scrivo valore */
  pnew_elem->val = value.MEMBERTYPE;

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
int extract_last_dynamic_BASETYPE(pvoid plista, all_type pvalue, punsi psize){
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
 *                 Deve tornare 0 se sono uguali
 *
 * Torna 1 se lo ha trovato, 0 altrimenti
 *
 * NB: pinput_compare puo' essere NULL, e in quel caso sara' utilizzata la plist_compare
 * fornita con la funzione add_functions se presente
 * */
int search_first_dynamic_BASETYPE(pvoid plist,
                         all_type value_searched, unsi size_searched,
                         all_type pvalue_found,   punsi psize_found,
                         pcustom_compare pinput_compare){
  plist_dynamic_BASETYPE plist_casted = (plist_dynamic_BASETYPE) plist;
  pelem_BASETYPE         pelem_moving;

  if (pinput_compare == NULL) return 0;
  pelem_moving = plist_casted->pstart;
  while(pelem_moving != NULL){
    if(!pinput_compare(value_searched, 0,(all_type)pelem_moving->val, 0)){
      *((pBASETYPE) pvalue_found.pv) = pelem_moving->val;
      return 1;
     }
    pelem_moving = pelem_moving->pnext;
   }
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
int get_max_dynamic_BASETYPE(pvoid plist, ppvoid ppaddr_max, punsi psize_max, pcustom_compare pinput_compare){
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
int sort_list_dynamic_BASETYPE(pvoid plist, pcustom_compare pinput_compare){
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
int print_list_dynamic_BASETYPE(pvoid plist, pcustom_print pinput_print){
  plist_dynamic_BASETYPE plist_casted = (plist_dynamic_BASETYPE) plist;
  pelem_BASETYPE pelem_moving = plist_casted->pstart;
  int i=0;

  printf("type_list: type_list_dynamic\n");
  printf("type_data: BASETYPE\n");
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  if (plist_casted->pstart == NULL) return 1;
  if(pinput_print != NULL){
    pinput_print((all_type)(pelem_moving->val), 0);
    pelem_moving = pelem_moving->pnext;
    while(pelem_moving != NULL){
      i++;
      if(i == 5) break;
      printf("->");
      pinput_print((all_type)(pelem_moving->val), 0);
      pelem_moving = pelem_moving->pnext;
     }
    printf("\n");
   }

  return 1;
 }

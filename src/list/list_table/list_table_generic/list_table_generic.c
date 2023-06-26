#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../../util/defines_typedef.h"
#include "./../../../all_type/define_all_type.h"
#include "./../../list.h"
#include "list_table_generic.hidden"
#include "list_table_generic.h"

#define DEBUG_LIST_TABLE_GENERIC
#define IDX_FINE_LISTA 0

static pvoid ptable = NULL;
static unsi  idx_void_list = 1;

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
pvoid malloc_list_table_generic(unsi dim_array){
  return malloc_list_specify_table_table_generic(dim_array, type_resize_default, TABLE_DEFAULT_DIM);
 }

/* malloc_list_specify_table: crea una nuova lista come sopra, e specifica il tipo di
 *                            resize della tabella che contiene la lista.
 * type_resize:               tipo di resize della tabella. Di default è type_resize_default,
 *                            ma puo' essere selezionato tra:
 *                            - type_resize_default: la tabella si espande automaticamente
 *                            quando piena
 *                            - type_resize_manual: le funzioni di inserimento tornano
 *                            errore quando la tabella e' piena. Essa va espansa manualmente
 *                            con la funzione expand_table
 * dim_table:                 numero di elementi che puo' contenere la tabella creata,
 *                            nel caso in cui questa non esistesse e dovesse essere creata.
 *                            Se la tabella e' stata gia' creata, ad esempio semplicemente
 *                            con malloc_list(), essa ha la dimensione di default TABLE_DEFAULT_DIM
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list_specify_table_table_generic(unsi dim_array, type_resize type_resize, unsi dim_table){
  pelem_table_generic  pfirst_elem_of_new_list;
  plist_table_generic pnew_list;

  if(ptable == NULL){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("---- DEBUG MALLOC ----\n");
    printf("Tabella non esiste\n");
    printf("---- FINE DEBUG ----\n\n");
    #endif
    if((create_table(type_resize, dim_table)) == NULL) return NULL;
   }

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata esaurita\n");
    return NULL;
   }

  /* STEP 1 */
  if((pnew_list = (plist_table_generic) malloc(sizeof(plist_table_generic))) == NULL) return NULL;
  pnew_list->n_elem = 0;
  pnew_list->idx_start = idx_void_list;
  pnew_list->idx_end = idx_void_list;

  /* STEP 2 */
  pfirst_elem_of_new_list = ((pelem_table_generic) ptable) + idx_void_list;
  idx_void_list = pfirst_elem_of_new_list->idx_next;

  /* STEP 3 */
  pfirst_elem_of_new_list->idx_next = IDX_FINE_LISTA;

  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG MALLOC ----\n");
  printf("Nuova lista creata, indice: %d\n", pnew_list->idx_start);
  printf("idx_void_list: %u\n", idx_void_list);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  return (pvoid) pnew_list;
 }

/* create_table: alloca la memoria della tabella
 * dim:          numero massimo di elementi contenuti nella tabella
 *
 * return:       torna l'indirizzo della tabella
 *
 * FUNZIONAMENTO:
 * 1) Alloco lo spazio di un array di elem_tabella (definiti in lista_veloce.hidden)
 *    di dimensione presa in input
 * 2) Inizializzo la tabella: e' fatta solo dalla lista dai vuoti. Questa inizia
 *    all'indice 0 e ha per elementi successivi gli indici 1->2->3->... fino
 *    all'ultimo che ha per elemento successivo -1
 * */
pvoid create_table(type_resize type_resize, unsi dim)
 {
  pelem_table_generic pelem_tmp;
  int                 i;


  /* STEP 1 */
  if((ptable = malloc(sizeof(elem_table_generic)*dim)) == NULL) return NULL;

  pelem_tmp = (pelem_table_generic) ptable;
  pelem_tmp->idx_next = type_resize;
  pelem_tmp++;

  /* STEP 2 */
  for (i = 2; i < dim; i++, pelem_tmp++){
    pelem_tmp->idx_next = i;
   }
  pelem_tmp->idx_next = IDX_FINE_LISTA;

  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG CREATE_TABLE ----\n");
  printf("Creo tabella\n");
  printf("Ecco la tabella:\n");
  printf("idx,  val,  idx_next\n");
  for (i = 0, pelem_tmp = (pelem_table_generic) ptable; i < dim; i++) {
    printf("%d,    val%d,   %u\n", i, i, pelem_tmp->idx_next);
    pelem_tmp++;
    if(i>10) break;
   }
  printf("---- FINE DEBUG ----\n\n");
  #endif

  return ptable;
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
int change_resize_table_table_generic(pvoid plist, type_resize type_resize){
  ((pelem_table_generic) ptable)->idx_next = type_resize;
  return 0;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_table_generic(pvoid plist){
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
int insert_first_table_generic(pvoid plist, ALL_TYPE value, unsi size){
  pelem_table_generic pfirst_free_elem;
  plist_table_generic plist_casted = (plist_table_generic) plist;
  int                 int_tmp;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0)
   {
    pfirst_free_elem = ((pelem_table_generic) ptable) + plist_casted->idx_start;
    if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
    memcpy(pfirst_free_elem->paddr,TO_PVOID(value),(pfirst_free_elem->size)=size);
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata piena\n");
    return 0;
   }
  /* STEP 1 */
  pfirst_free_elem = ((pelem_table_generic) ptable) + idx_void_list;

  /* STEP 2 */
  if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
  memcpy(pfirst_free_elem->paddr,TO_PVOID(value),(pfirst_free_elem->size)=size);
  int_tmp = pfirst_free_elem->idx_next;
  pfirst_free_elem->idx_next = plist_casted->idx_start;

  /* STEP 3 */
  plist_casted->idx_start = idx_void_list;
  (plist_casted->n_elem)++;

  /* STEP 4 */
  idx_void_list = int_tmp;
  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG INSERT_FIRST ----\n");
  printf("Nuovo idx_void_list: %d\n", idx_void_list);
  printf("---- FINE DEBUG ----\n\n");
  #endif

  return 1;
 }

/* extract_first: estrae l'elemento in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * pvalue:        indirizzo in cui verra' scritto risettivamente:
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
int extract_first_table_generic(pvoid plist, ALL_TYPE pvalue, punsi psize){
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
int search_first_table_generic(pvoid plist,
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
int get_max_table_generic(pvoid plist, ppvoid ppaddr_max, punsi psize_max, pcustom_compare pinput_compare){
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
int sort_list_table_generic(pvoid plist, pcustom_compare pinput_compare){
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
int print_list_table_generic(pvoid plist, pcustom_print pinput_print){
  plist_table_generic plist_casted = (plist_table_generic) plist;
  pelem_table_generic pelem_start = (pelem_table_generic) ptable, pelem_tmp;

  printf("Tipo lista: VELOCE.\n");
  printf("Numero di elementi: %d.\n", plist_casted->n_elem);
  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG PRINT_LIST ----\n");
  printf("Idx_start: %d, idx_end: %d, n_elem: %d, idx_void_list: %d\n", plist_casted->idx_start, plist_casted->idx_end, plist_casted->n_elem, idx_void_list);
  printf("Tipo di resize: %d\n", ((pelem_table_generic)ptable)->idx_next);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  if((plist_casted->n_elem == 0) || (pinput_print == NULL)) return 1;

  pelem_tmp = pelem_start + plist_casted->idx_start;
  while(pelem_tmp->idx_next != -1)
   {
    if(!(pinput_print(TO_ALLTYPE(pelem_tmp->paddr), pelem_tmp->size))) return 0;
    printf("->");
    pelem_tmp = pelem_start + pelem_tmp->idx_next;
   }
  if(!(pinput_print(TO_ALLTYPE(pelem_tmp->paddr), pelem_tmp->size))) return 0;
  printf("\n");

  return 1;
 }

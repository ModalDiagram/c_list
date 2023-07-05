#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_table_array_BASETYPE.hidden"
#include "list_table_array_BASETYPE.h"

/* #define DEBUG_LIST_TABLE_ARRAY_BASETYPE */

#define GET_IDX_NEXT(pvalue) (*((punsi)(((pchar) pvalue )+sizeof_array)))
#define GET_NEXT_ELEM(pvalue, i) (((pchar) pvalue ) + (sizeof_array + sizeof(unsi))* i)
#define SIZEOFELEM (sizeof_array+sizeof(unsi))

static ppvoid pptables = NULL;

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
pvoid malloc_list_table_array_BASETYPE(unsi dim_array){
  return malloc_list_specify_table_table_array_BASETYPE(dim_array, type_resize_default, TABLE_DEFAULT_DIM);
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
 * return:                    puntatore alla nuova lista, NULL se l'istanziamento non
 *                            va a buon fine
 * */
pvoid malloc_list_specify_table_table_array_BASETYPE(unsi size_array, type_resize type_resize, unsi dim_table){
  plist_table_array_BASETYPE pnew_list;
  pvoid                      pfirst_elem_of_new_list, ptable;
  ptable_info_array_BASETYPE                pmy_info;
  unsi                       idx_void_list;
  unsi                       sizeof_array = size_array * sizeof(BASETYPE_SPACE);

  /* se non esiste la lista di tabelle, la creo */
  if(pptables == NULL){
    #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
    printf("---- DEBUG MALLOC ----\n");
    printf("Lista di tabelle non esiste\n");
    printf("---- FINE DEBUG ----\n\n");
    #endif
    if((pptables = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL) return NULL;
    ptable = create_table_array_BASETYPE(sizeof_array, type_resize, dim_table);
   }
  /* se esiste cerco la tabella che contiene array con dimensione uguale a quello
   * da istanziare (size_array) */
  else{
    /* se la tabella corrispondente esiste, search_first la scrive in ptable;
     * se non esiste la creo e la inserisco nella lista (in create_table) */
    if(!search_first(pptables, (all_type)(int)sizeof_array, 0, (all_type)(pvoid)&ptable, 0, pfind_table_array_BASETYPE)){
      #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
      printf("---- DEBUG MALLOC ----\n");
      printf("Tabella con dim %u non esiste\n", size_array);
      printf("---- FINE DEBUG ----\n\n");
      #endif
      ptable = create_table_array_BASETYPE(sizeof_array, type_resize, dim_table);
     }
    #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
    else{
      printf("---- DEBUG MALLOC ----\n");
      printf("Tabella esiste, indirizzo %lu\n", (long)ptable);
      printf("---- FINE DEBUG ----\n\n");
     }
    #endif
   }
  pmy_info = (ptable_info_array_BASETYPE) ptable;
  ptable = pmy_info + 1;
  idx_void_list = pmy_info->idx_void_list;

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata esaurita\n");
    return NULL;
   }

  /* STEP 1 */
  if((pnew_list = (plist_table_array_BASETYPE) malloc(sizeof(plist_table_array_BASETYPE))) == NULL) return NULL;

  /* devo aggiungere la lista creata alla lista di liste contenute nella table */
  if(!insert_first(pmy_info->plist_of_lists, (all_type)((pvoid)pnew_list), 0)){
    free(pnew_list);
    return 0;
   }

  pnew_list->n_elem = 0;
  pnew_list->idx_start = idx_void_list;
  pnew_list->idx_end = idx_void_list;
  pnew_list->ptable = pmy_info;

  /* STEP 2 */
  pfirst_elem_of_new_list = GET_NEXT_ELEM(ptable, idx_void_list);
  idx_void_list = GET_IDX_NEXT(pfirst_elem_of_new_list);

  /* STEP 3 */
  GET_IDX_NEXT(pfirst_elem_of_new_list) = IDX_FINE_LISTA;

  pmy_info->idx_void_list = idx_void_list;

  (pmy_info->n_occupied)++;

  #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
  printf("---- DEBUG MALLOC ----\n");
  printf("Nuova lista creata, indice: %d\n", pnew_list->idx_start);
  printf("idx_void_list: %u\n", idx_void_list);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  return (pvoid) pnew_list;
 }

/* pfind_table: funzione con cui trovare la tabella con dimensione corretta
 * value1:      intero corrispondente alla size degli array da cercare
 * size1:       non importa
 * value2:      indirizzo delintegrate e^(-2ix) dcosx++integrate e^(-2ix) dcosx++la tabella da cercare, posso prendere la dimensione degli
 *              array contenuti con ((ptable_info_array_BASETYPE) ptable)->size_array
 * size2:       non importa
 * presult:     vi scrive 0 quando l'intero in value1 e' uguale alla dimensione degli
 *              array in value2
 * */
int pfind_table_array_BASETYPE(all_type value1, unsi size1,
                all_type value2, unsi size2){
  if(((ptable_info_array_BASETYPE)(value2.pv))->sizeof_array == value1.i){
    return 0;
   }
  return 1;
 }

/* create_table: alloca la memoria della tabella
 * dim:          numero massimo di elementi contenuti nella tabella
 *
 * return:       torna l'indirizzo della tabella
 *
 * FUNZIONAMENTO:
 * 1) Alloco lo spazio della tabella, che corrisponde al sizeof(table_info_array_BASETYPE), il quale
 *    contiene size degli array contenuti e idx_void_list, piu' dim volte il size di un
 *    elemento della tabella, che e' size_array*sizeof(tipo).
 *    La aggiungo alla lista di tabelle.
 * 2) Imposto le informazioni sulla tabella:
 *    - size_array in ptable_info_array_BASETYPE prima della tabella
 *    - idx_void_list in ptable_info_array_BASETYPE prima della tabella
 *    - type_resize nell'idx_next del primo elemento della tabella
 *    Poi sposto table di ptable_info_array_BASETYPE in modo che coincida con l'inizio della tabella
 *    vera e propria.
 * 3) Inizializzo la tabella: e' fatta solo dalla lista dai vuoti. Questa inizia
 *    all'indice 1 e ha per elementi successivi gli indici 2->3->... fino
 *    all'ultimo che ha per elemento successivo 0 (IDX_FINE_LISTA)
 * */
pvoid create_table_array_BASETYPE(unsi sizeof_array, type_resize type_resize, unsi dim){
  int         i;
  pvoid       pelem_tmp, ptable;
  ptable_info_array_BASETYPE pinfo_new_table;
  unsi        actual_size = dim + 1;

  /* STEP 1 */
  if((ptable = malloc(sizeof(table_info_array_BASETYPE) + SIZEOFELEM*actual_size)) == NULL) return NULL;
  #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
  printf("---- DEBUG CREATE_TABLE ----\n");
  printf("Nuova tabella creata, indirizzo: %lu\n", (long)ptable);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  insert_first(pptables, (all_type)ptable, 0);
  /* print_list(pptables, NULL); */

  /* STEP 2 */
  pinfo_new_table = ptable;
  if((pinfo_new_table->plist_of_lists = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL){
    free(ptable);
    return 0;
   }
  pinfo_new_table->sizeof_array = sizeof_array;
  pinfo_new_table->idx_void_list = 1;
  pinfo_new_table->n_occupied = 0;
  pinfo_new_table->n_entries = dim;
  ptable = pinfo_new_table + 1;
  pelem_tmp = ptable;
  GET_IDX_NEXT(pelem_tmp) = type_resize;
  pelem_tmp = GET_NEXT_ELEM(ptable, 1);

  /* STEP 3 */
  for (i = 2; i < actual_size; i++, pelem_tmp = GET_NEXT_ELEM(pelem_tmp, 1)){
    GET_IDX_NEXT(pelem_tmp) = i;
   }
  GET_IDX_NEXT(pelem_tmp) = IDX_FINE_LISTA;

  #ifdef DEBUG_LIST_TABLE_ARRAY_BASETYPE
  printf("---- DEBUG CREATE_TABLE ----\n");
  printf("Creo tabella\n");
  printf("Ecco la tabella:\n");
  printf("idx,  val,  idx_next\n");
  for (i = 0, pelem_tmp = ptable; i < dim; i++) {
    printf("%d,    val%d,   %u\n", i, i, GET_IDX_NEXT(pelem_tmp));
    pelem_tmp = GET_NEXT_ELEM(pelem_tmp, 1);
    if(i>5) break;
   }
  printf("---- FINE DEBUG ----\n\n");
  #endif

  return pinfo_new_table;
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
int change_resize_table_table_array_BASETYPE(pvoid plist, type_resize type_resize){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE                pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  unsi  sizeof_array = pmy_info->sizeof_array;
  pvoid ptable = pmy_info + 1;

  GET_IDX_NEXT(ptable) = type_resize;
  return 1;
 }

/* resize_table: cambia il numero di elementi complessivi della tabella che contiene plist
 * n_entries:    numero di elementi complessivi della tabella, dopo che è stata ridimensionata;
 *
 * return:       1 se ridimensionata correttamente, 0 altrimenti, ad esempio se
 * n_entries è minore del numero di elementi delle liste contenute.
 */
int resize_table_table_array_BASETYPE(pvoid plist, unsi n_entries){
  ptable_info_array_BASETYPE pmy_info;
  ptable_info_array_BASETYPE ptable_and_elem;
  pvoid                      pnew_table_start, pelem_moving, ptable_extracted;
  int                 i;
  unsi                actual_size = n_entries + 1, new_idx_void, sizeof_array, idx_void_list;
  plist_table_array_BASETYPE plist_extracted, plist_casted = (plist_table_array_BASETYPE) plist;

  pmy_info = plist_casted->ptable;
  sizeof_array = pmy_info->sizeof_array;
  idx_void_list = pmy_info->idx_void_list;
  /* CASO 1: dimensione troppo piccola
   * se la dimensione scelta e' minore del numero degli elementi attualmente contenuti
   * da' errore perche' non li puo' scartare */
  if(n_entries < pmy_info->n_occupied) return 0;

  /* CASO 2: ingrandimento
   * posso semplicemente allocare una zona di memoria piu' grande e fare un memcopy.
   * Poi aggiungo tutti gli elementi successivi in cima alla lista dei vuoti */
  if(n_entries > pmy_info->n_entries){
    /* alloco la nuovo table */
    if((ptable_and_elem = malloc(sizeof(table_info_array_BASETYPE) + SIZEOFELEM * actual_size)) == NULL) return 0;
    /* copio la tabella aggiornando il numero totale di elementi */
    memcpy(ptable_and_elem, pmy_info, sizeof(table_info_array_BASETYPE) + SIZEOFELEM * (pmy_info->n_entries + 1));

    ptable_and_elem->n_entries = n_entries;
    pnew_table_start = (pvoid)(ptable_and_elem + 1);

    /* collego tutti i nuovi elementi con una lista che termina all'inizio della lista dei vuoti */
    for (i = pmy_info->n_entries + 1,
         pelem_moving = GET_NEXT_ELEM(pnew_table_start, i);
         i < n_entries; i++) {
      GET_IDX_NEXT(pelem_moving) = i + 1;
      pelem_moving = GET_NEXT_ELEM(pelem_moving, 1);
     }
    GET_IDX_NEXT(pelem_moving) = idx_void_list;

    /* sposto l'inizio della lista dei vuoti all'inizio della nuova lista */
    ptable_and_elem->idx_void_list = pmy_info->n_entries + 1;

    /* aggiorno il ptable di tutte le liste della table alla nuova table */
    while (extract_first(ptable_and_elem->plist_of_lists, (all_type)((pvoid) &plist_extracted), NULL)) {
      if(plist_extracted->ptable == ptable_and_elem){
        break;
       }
      plist_extracted->ptable = ptable_and_elem;
      insert_last(ptable_and_elem->plist_of_lists, (all_type)((pvoid) plist_extracted), 0);
     }
    insert_last(ptable_and_elem->plist_of_lists, (all_type)((pvoid) plist_extracted), 0);

    /* devo aggiungere la nuova tabella alla lista di tabelle e rimuovere quella vecchia */
    while(extract_first(pptables, (all_type)((pvoid)&ptable_extracted), 0)){
      if(ptable_extracted == pmy_info){
        break;
       }
      insert_last(pptables, (all_type)ptable_extracted, 0);
     }
    insert_first(pptables, (all_type) ((pvoid)ptable_and_elem), 0);

    free(pmy_info);

    return 1;
   }
  /* CASO 3: rimpicciolimento
   * in questo caso n_entries<ptable->n_entries quindi dobbiamo rimpicciolire, ma non
   * possiamo usare memcopy.
   * Procedo per step:
   * 1) Alloco la nuova table con una nuova lista di liste
   * 2) ricopio ogni lista nella vecchia lista di liste e la inserisco in quella nuova
   * 3) metto tutti gli elementi successivi in coda alla lista dei vuoti
   * 4) aggiorno info e sostituisco vecchia table
   * */
  else{
    /* STEP 1 */
    if((ptable_and_elem = malloc(sizeof(table_info_array_BASETYPE) + SIZEOFELEM * actual_size)) == NULL) return 0;
    if((ptable_and_elem->plist_of_lists = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL){
      free(ptable_and_elem);
      return 0;
     }

    /* STEP 2
     * Per ricopiare sfrutto il fatto che la tabella e' vuota quindi ricopio ogni lista
     * in ordine, aumentando di volta in volta di 1 il new_idx_void */
    new_idx_void = 1;
    pnew_table_start = (pvoid)(ptable_and_elem + 1);
    while(extract_first(pmy_info->plist_of_lists, (all_type)((pvoid)&plist_extracted), NULL)){
      printf("Copio una lista\n");
      /* qui si devono aggiungere controlli appropriati */
      /* devo copiare il resize */
      insert_first(ptable_and_elem->plist_of_lists, (all_type)((pvoid)plist_extracted), 0);
      /* se ha 0 elementi basta aggiungere il posto della lista */
      if(plist_extracted->n_elem == 0){
        plist_extracted->idx_start = plist_extracted->idx_end = new_idx_void;
        plist_extracted->ptable = (pvoid) ptable_and_elem;
        new_idx_void++;
       }
      /* se ha piu' elementi li ricopio */
      else{
        copy_list_array_BASETYPE(pnew_table_start, &new_idx_void,
                                 (pvoid)(pmy_info + 1), &idx_void_list,
                                 ((plist_table_array_BASETYPE)plist_extracted)->idx_start, sizeof_array);
        plist_extracted->idx_start = new_idx_void - plist_extracted->n_elem + 1;
        plist_extracted->idx_end = new_idx_void;
        plist_extracted->ptable = (pvoid) ptable_and_elem;
        new_idx_void++;
       }
     }
    /* STEP 3 */
    for (i = new_idx_void, pelem_moving = GET_NEXT_ELEM(pnew_table_start, i); i < n_entries; i++) {
      GET_IDX_NEXT(pelem_moving) = i + 1;
      pelem_moving = GET_NEXT_ELEM(pelem_moving,1);
     }
    GET_IDX_NEXT(pelem_moving) = IDX_FINE_LISTA;

    /* STEP 4 */
    ptable_and_elem->sizeof_array = sizeof_array;
    ptable_and_elem->n_entries = n_entries;
    ptable_and_elem->n_occupied = pmy_info->n_occupied;
    ptable_and_elem->plist_of_lists = pmy_info->plist_of_lists;
    ptable_and_elem->idx_void_list = new_idx_void;

    /* devo aggiungere la nuova tabella alla lista di tabelle e rimuovere quella vecchia */
    while(extract_first(pptables, (all_type)((pvoid)&ptable_extracted), 0)){
      if(ptable_extracted == pmy_info){
        break;
       }
      insert_last(pptables, (all_type)ptable_extracted, 0);
     }
    insert_first(pptables, (all_type) ((pvoid)ptable_and_elem), 0);

    free(pmy_info);

    return 1;
   }
  return 0;
 }

/* copy_list: copia una lista da una table a un'altra
 * ptable_dest: table in cui copiare la lista
 * idx_void_list_dest: indirizzo in cui legge e scrive l'idx_void_list della tabella
 *                     di destinazione
 * ptable_orig:        table da cui copiare la lista
 * idx_void_list_orig: indirizzo in cui legge e scrive l'idx_void_list della tabella
 *                     di partenza
 * idx_start:          indirizzo di partenza della lista da copiare
 * */
int copy_list_array_BASETYPE(pvoid ptable_dest, punsi pidx_void_list_dest,
                             pvoid ptable_orig, punsi pidx_void_list_orig,
                             unsi idx_start, unsi sizeof_array){
  pvoid pelem_moving_dest, pelem_moving_origin;

  pelem_moving_dest = GET_NEXT_ELEM(ptable_dest, *pidx_void_list_dest);
  pelem_moving_origin = GET_NEXT_ELEM(ptable_orig, idx_start);
  while(GET_IDX_NEXT(pelem_moving_origin) != IDX_FINE_LISTA){
    memcpy(pelem_moving_dest, pelem_moving_origin, sizeof_array);
    GET_IDX_NEXT(pelem_moving_dest) = ++(*pidx_void_list_dest);
    pelem_moving_dest = GET_NEXT_ELEM(pelem_moving_dest, 1);
    pelem_moving_origin = GET_NEXT_ELEM(ptable_orig, GET_IDX_NEXT(pelem_moving_origin));
   }
  memcpy(pelem_moving_dest, pelem_moving_origin, sizeof_array);
  GET_IDX_NEXT(pelem_moving_dest) = IDX_FINE_LISTA;
  return 1;
 }

/* get_table_info_array_BASETYPE: fornisce informazioni sulla tabella che contiene plist
 * pn_entries: indirizzo in cui scrivere il numero di elementi complessivi della tabella;
 * pn_occupied: indirizzo in cui scrivere il numero di elementi occupati della tabella;
 *
 * return: 1 se tutto va bene, 0 altrimenti
 * */
int get_info_table_table_array_BASETYPE(pvoid plist,
                   punsi pn_entries,
                   punsi pn_occupied){
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (((plist_table_array_BASETYPE) plist)->ptable);

  *pn_entries = pmy_info->n_entries;
  *pn_occupied = pmy_info->n_occupied;

  return 1;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_table_array_BASETYPE(pvoid plist){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  unsi                       sizeof_array = pmy_info->sizeof_array;
  unsi                       idx_void_list = pmy_info->idx_void_list;
  pvoid                      ptable = pmy_info + 1, pextracted;

  if(plist_casted->n_elem == 0)
   {
    GET_IDX_NEXT(GET_NEXT_ELEM(ptable, plist_casted->idx_start)) = idx_void_list;
    idx_void_list = plist_casted->idx_start;

    /* tolgo la lista liberata dalla list_of_lists */
    while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
      if(pextracted == plist){
        break;
       }
      insert_last(pmy_info->plist_of_lists, (all_type)pextracted, 0);
     }

    free(plist);
    (pmy_info->n_occupied)--;

    #ifdef DEBUG_LIST_TABLE_BASETYPE
    printf("---- DEBUG ----\n");
    printf("Lista liberata. Indirizzo lista dei vuoti: %d\n", idx_void_list);
    printf("---- FINE DEBUG ----\n");
    #endif
    return;
   }

  /* STEP 1 */
  GET_IDX_NEXT(GET_NEXT_ELEM(ptable, plist_casted->idx_end)) = idx_void_list;

  /* STEP 2 */
  idx_void_list = plist_casted->idx_start;
  pmy_info->idx_void_list = idx_void_list;

  /* STEP 3 */
  /* tolgo la lista liberata dalla list_of_lists */
  while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
    if(pextracted == plist){
      break;
     }
    insert_last(pmy_info->plist_of_lists, (all_type)pextracted, 0);
   }
  ((pmy_info)->n_occupied) -= plist_casted->n_elem;
  free(plist);

  #ifdef DEBUG_LIST_TABLE_BASETYPE
  printf("---- DEBUG ----\n");
  printf("Lista liberata. Indirizzo lista dei vuoti: %d\n", idx_void_list);
  printf("---- FINE DEBUG ----\n");
  #endif
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
int insert_first_table_array_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid               pfirst_free_elem, ptable = pmy_info + 1;
  int                 int_tmp;
  unsi                sizeof_array = pmy_info->sizeof_array;
  unsi                idx_void_list = pmy_info->idx_void_list;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0)
   {
    pfirst_free_elem = GET_NEXT_ELEM(ptable, plist_casted->idx_start);
    memcpy(pfirst_free_elem, value.pv, sizeof_array);
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata piena\n");
    return 0;
   }
  /* STEP 1 */
  pfirst_free_elem = GET_NEXT_ELEM(ptable, idx_void_list);

  /* STEP 2 */
  memcpy(pfirst_free_elem,value.pv, sizeof_array);
  int_tmp = GET_IDX_NEXT(pfirst_free_elem);
  GET_IDX_NEXT(pfirst_free_elem) = plist_casted->idx_start;

  /* STEP 3 */
  plist_casted->idx_start = idx_void_list;
  (plist_casted->n_elem)++;

  /* STEP 4 */
  idx_void_list = int_tmp;
  pmy_info->idx_void_list = idx_void_list;

  (pmy_info->n_occupied)++;

  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG INSERT_FIRST ----\n");
  printf("Nuovo idx_void_list: %d\n", idx_void_list);
  printf("---- FINE DEBUG ----\n\n");
  #endif

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
int extract_first_table_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid               pelem_to_extract, ptable = pmy_info + 1;
  ppvoid              ppvalue_input = pvalue.pv;
  int                 tmp_int;
  unsi                sizeof_array = pmy_info->sizeof_array;
  unsi                idx_void_list = pmy_info->idx_void_list;

  if(plist_casted->n_elem == 0){
    return 0;
   }

  /* STEP 1 */
  pelem_to_extract = GET_NEXT_ELEM(ptable, plist_casted->idx_start);

  /* STEP 2 */
  if((*ppvalue_input = malloc(sizeof_array)) == NULL) return 0;
  memcpy(*ppvalue_input, pelem_to_extract, sizeof_array);

  /* se la lista ha 1 solo elemento non devo liberare lo spazio (ogni lista
   * ha almeno uno spazio), ma basta decrementare il n_elem,
   * perche' quando aggiungero' l'elemento successivo paddr e size
   * saranno sovrascritti essendo n_elem==0 */
  if(plist_casted->n_elem == 1)
   {
    (plist_casted->n_elem)--;
    return 1;
   }

  /* STEP 3 */
  tmp_int = GET_IDX_NEXT(pelem_to_extract);
  GET_IDX_NEXT(pelem_to_extract) = idx_void_list;

  /* STEP 4 */
  pmy_info->idx_void_list = plist_casted->idx_start;

  /* STEP 5 */
  plist_casted->idx_start = tmp_int;
  (plist_casted->n_elem)--;
  (pmy_info->n_occupied)--;

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
int insert_last_table_array_BASETYPE(pvoid plist, all_type value, unsi size){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid                      pfirst_free_elem, ptable = pmy_info + 1;
  unsi                       sizeof_array = pmy_info->sizeof_array;
  unsi                       idx_void_list = pmy_info->idx_void_list;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0){
    pfirst_free_elem = GET_NEXT_ELEM(ptable, plist_casted->idx_start);
    memcpy(pfirst_free_elem,value.pv, sizeof_array);
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA){
    printf("Memoria preallocata piena\n");
    return 0;
   }
  /* salvo l'indirizzo del primo elemento libero */
  pfirst_free_elem = GET_NEXT_ELEM(ptable, idx_void_list);

  /* salvo le informazioni prese in input nell'elemento */
  memcpy(pfirst_free_elem,value.pv, sizeof_array);

  /* aggiorno l'idx_next del penultimo elemento nell'indice del nuovo elemento */
  GET_IDX_NEXT(GET_NEXT_ELEM(ptable, plist_casted->idx_end)) = idx_void_list;

  /* aggiorno le informazioni sulla lista */
  plist_casted->idx_end = idx_void_list;
  (plist_casted->n_elem)++;

  /* aggiorno l'indice della lista dei vuoti e dell'ultimo elemento della lista */
  pmy_info->idx_void_list = GET_IDX_NEXT(pfirst_free_elem);
  GET_IDX_NEXT(pfirst_free_elem) = IDX_FINE_LISTA;

  (pmy_info->n_occupied)++;

  /* print_table_array_BASETYPE(plist_casted); */
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
int extract_last_table_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE                pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid                      pelem_moving, ptable = pmy_info + 1;
  ppvoid                     ppvalue_input = pvalue.pv;
  unsi                       idx_current, idx_end = plist_casted->idx_end;
  unsi                       sizeof_array = pmy_info->sizeof_array;
  unsi                       idx_void_list = pmy_info->idx_void_list;

  if(plist_casted->n_elem == 0){
    return 0;
   }

  /* se la lista ha 1 solo elemento non devo liberare lo spazio (ogni lista
   * ha almeno uno spazio), ma basta decrementare il n_elem,
   * perche' quando aggiungero' l'elemento successivo il valore contenuto
   * sara' sovrascritti essendo n_elem==0 */
  pelem_moving = GET_NEXT_ELEM(ptable, plist_casted->idx_start);
  if(plist_casted->n_elem == 1){
    if((*ppvalue_input = malloc(sizeof_array)) == NULL) return 0;
    memcpy(*ppvalue_input, pelem_moving, sizeof_array);
    (plist_casted->n_elem)--;
    return 1;
   }

  /* raggiungo l'ultimo elemento con pelem_moving e conservo in idx_current l'indice del penultimo */
  idx_current = plist_casted->idx_start;
  while(GET_IDX_NEXT(pelem_moving) != idx_end){
    pelem_moving = GET_NEXT_ELEM(ptable, (idx_current = GET_IDX_NEXT(pelem_moving)));
   }
  pelem_moving = GET_NEXT_ELEM(ptable, GET_IDX_NEXT(pelem_moving));

  /* scrivo IDX_FINE_LISTA nell'idx_next del penultimo elemento */
  GET_IDX_NEXT(GET_NEXT_ELEM(ptable, idx_current)) = IDX_FINE_LISTA;

  /* restituisco il valore estratto */
  if((*ppvalue_input = malloc(sizeof_array)) == NULL) return 0;
  memcpy(*ppvalue_input, pelem_moving, sizeof_array);

  /* metto l'ultimo elemento in cima alla lista dei vuoti */
  GET_IDX_NEXT(pelem_moving) = idx_void_list;
  pmy_info->idx_void_list = plist_casted->idx_end;

  /* aggiorno le informazioni della lista */
  plist_casted->idx_end = idx_current;
  (plist_casted->n_elem)--;
  (pmy_info->n_occupied)--;

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
int insert_nth_table_array_BASETYPE(pvoid plist, all_type value, unsi size, unsi n){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid                      pnew_elem, pelem_moving, ptable = (pvoid)(pmy_info + 1);
  int                        int_tmp, i;
  unsi                       sizeof_array = pmy_info->sizeof_array;
  unsi                       idx_void_list = pmy_info->idx_void_list;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem + 1)) return 0;
  if(n == (plist_casted->n_elem + 1)) return insert_last(plist, value, size);
  if(n == 1) return insert_first(plist, value, size);

  /* salvo l'indirizzo del nuovo elemento e vi salvo il valore preso in input */
  pnew_elem = GET_NEXT_ELEM(ptable, idx_void_list);
  memcpy(pnew_elem, value.pv, sizeof_array);

  /* salvo il suo idx_next, che corrisponde al nuovo inizio della idx_void_list */
  int_tmp = GET_IDX_NEXT(pnew_elem);

  /* raggiungo l'(n-1)-esimo elemento con pelem_moving */
  for (i = 2, pelem_moving = GET_NEXT_ELEM(ptable, plist_casted->idx_start); i < n; i++) {
    pelem_moving = GET_NEXT_ELEM(ptable, GET_IDX_NEXT(pelem_moving));
  }
  /* metto pnew elem tra l'(n-1)-esimo e l'n-esimo elemento */
  GET_IDX_NEXT(pnew_elem) = GET_IDX_NEXT(pelem_moving);
  GET_IDX_NEXT(pelem_moving) = idx_void_list;

  /* aggiorno l'indice di inizio della lista dei vuoti */
  pmy_info->idx_void_list = int_tmp;

  (plist_casted->n_elem)++;
  (pmy_info->n_occupied)++;

  return 1;
 }

/* extract_first: estrae l'elemento all'n-esima posizione della lista
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
int extract_nth_table_array_BASETYPE(pvoid plist, all_type pvalue, punsi psize, unsi n){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE pmy_info = (ptable_info_array_BASETYPE) plist_casted->ptable;
  pvoid  pelem_to_extract, pelem_moving, ptable = (pvoid)(pmy_info + 1);
  int    int_tmp, i;
  ppvoid ppvalue_input = (ppvoid) (pvalue.pv);
  unsi sizeof_array = pmy_info->sizeof_array, idx_void_list = pmy_info->idx_void_list;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem)) return 0;
  if(n == (plist_casted->n_elem)){
    return extract_last(plist, pvalue, psize);
   }
  if(n == 1){
    return extract_first(plist, pvalue, psize);
   }

  /* raggiungo con pelem_moving l'(n-1)-esimo elemento */
  for (i = 2, pelem_moving = GET_NEXT_ELEM(ptable, plist_casted->idx_start); i < n; i++) {
    pelem_moving = GET_NEXT_ELEM(ptable, GET_IDX_NEXT(pelem_moving));
   }
  /* salvo in pelem_to_extract l'indirizzo dell'n-esimo elemento, ovvero quello da estrarre */
  pelem_to_extract = GET_NEXT_ELEM(ptable, GET_IDX_NEXT(pelem_moving));

  /* restituisco il valore contenuto in pelem_to_extract */
  if((*ppvalue_input = malloc(sizeof_array)) == NULL) return 0;
  memcpy(*ppvalue_input, pelem_to_extract, sizeof_array);
  int_tmp = GET_IDX_NEXT(pelem_to_extract);

  /* cambio l'idx_next di pelem_to_extract in modo che diventi il primo elemento della void_list */
  GET_IDX_NEXT(pelem_to_extract) = idx_void_list;
  pmy_info->idx_void_list = GET_IDX_NEXT(pelem_moving);

  /* aggiorno l'idx_next dell'(n-1)-esimo elemento */
  GET_IDX_NEXT(pelem_moving) = int_tmp;

  (plist_casted->n_elem)--;
  (pmy_info->n_occupied)--;

  return 1;
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
int search_first_table_array_BASETYPE(pvoid plist,
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
int print_list_table_array_BASETYPE(pvoid plist, pcustom_print pinput_print){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE) plist;
  ptable_info_array_BASETYPE                pmy_info = (ptable_info_array_BASETYPE) (plist_casted->ptable);
  pvoid pelem_tmp, ptable = pmy_info + 1;
  unsi  sizeof_array = pmy_info->sizeof_array;
  int   i=0;

  printf("type_list: type_list_table\n");
  printf("type_data: array BASETYPE\n");
  printf("Numero di elementi degli array contenuti: %lu\n", sizeof_array / sizeof(BASETYPE_SPACE));
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG PRINT_LIST ----\n");
  printf("Idx_start: %d, idx_end: %d, n_elem: %d, idx_void_list: %d\n", plist_casted->idx_start, plist_casted->idx_end, plist_casted->n_elem, idx_void_list);
  printf("Tipo di resize: %d\n", GET_IDX_NEXT(ptable));
  printf("---- FINE DEBUG ----\n\n");
  #endif
  if((plist_casted->n_elem == 0) || (pinput_print == NULL)) return 1;

  pelem_tmp = GET_NEXT_ELEM(ptable, plist_casted->idx_start);
  while(GET_IDX_NEXT(pelem_tmp) != IDX_FINE_LISTA)
   {
    i++;
    if(i == 5) break;
    if(!(pinput_print((all_type)(pelem_tmp), sizeof_array / sizeof(BASETYPE_SPACE)))) return 0;
    printf("->");
    pelem_tmp = GET_NEXT_ELEM(ptable, GET_IDX_NEXT(pelem_tmp));
   }
  if(!(pinput_print((all_type)(pelem_tmp), sizeof_array / sizeof(BASETYPE_SPACE)))) return 0;
  printf("\n");

  return 1;
 }

void print_table_array_BASETYPE(plist_table_array_BASETYPE plist){
  ptable_info_array_BASETYPE pmy_info = plist->ptable;
  int                 i;
  pvoid pelem_moving, ptable = (pvoid)(pmy_info + 1);
  unsi sizeof_array = pmy_info->sizeof_array;

  printf("Stampo tabella che inizia a %lu\n", (long)(pmy_info));
  printf("Stampo tabella a indirizzo %lu\n", (long)(ptable));
  printf("idx_void_list: %u\n", pmy_info->idx_void_list);
  for (i = 0; i < (pmy_info->n_entries + 1); i++) {
    pelem_moving = GET_NEXT_ELEM(ptable, i);
    printf("%d -> %d\n", i, GET_IDX_NEXT(pelem_moving));
  }
 }

int print_list_of_lists_array_BASETYPE(all_type plist, unsi size){
  plist_table_array_BASETYPE plist_casted = (plist_table_array_BASETYPE)(plist.pv);

  printf("idx_start %u, n_elem %u, table %lu\n", plist_casted->idx_start, plist_casted->n_elem, (long)plist_casted->ptable);
  return 1;
 }

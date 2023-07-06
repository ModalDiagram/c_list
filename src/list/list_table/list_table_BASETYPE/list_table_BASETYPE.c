#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_table_BASETYPE.hidden"
#include "list_table_BASETYPE.h"

/* #define DEBUG_LIST_TABLE_BASETYPE */

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
pvoid malloc_list_table_BASETYPE(unsi dim_array){
  return malloc_list_specify_table_table_BASETYPE(dim_array, type_resize_default, TABLE_DEFAULT_DIM);
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
pvoid malloc_list_specify_table_table_BASETYPE(unsi dim_array, type_resize type_resize, unsi dim_table){
  pelem_table_BASETYPE pfirst_elem_of_new_list;
  plist_table_BASETYPE pnew_list;
  ptable_info_BASETYPE pmy_info;

  if(ptable == NULL){
    #ifdef DEBUG_LIST_TABLE_BASETYPE
    printf("---- DEBUG MALLOC ----\n");
    printf("Tabella non esiste\n");
    printf("---- FINE DEBUG ----\n\n");
    #endif
    if((create_table_BASETYPE(type_resize, dim_table)) == NULL) return NULL;
   }
  if(ptable == NULL) printf("Ancora vuota\n");

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata esaurita\n");
    return NULL;
   }

  /* STEP 1 */
  if((pnew_list = (plist_table_BASETYPE) malloc(sizeof(plist_table_BASETYPE))) == NULL) return NULL;

  pmy_info = ((ptable_info_BASETYPE)ptable) - 1;
  /* devo aggiungere la lista creata alla lista di liste contenute nella table */
  if(!insert_first(pmy_info->plist_of_lists, (all_type)((pvoid)pnew_list), 0)){
    free(pnew_list);
    return 0;
   }

  pnew_list->n_elem = 0;
  pnew_list->idx_start = idx_void_list;
  pnew_list->idx_end = idx_void_list;

  /* STEP 2 */
  pfirst_elem_of_new_list = ((pelem_table_BASETYPE) ptable) + idx_void_list;
  idx_void_list = pfirst_elem_of_new_list->idx_next;

  /* STEP 3 */
  pfirst_elem_of_new_list->idx_next = IDX_FINE_LISTA;

  (pmy_info->n_occupied)++;

  #ifdef DEBUG_LIST_TABLE_BASETYPE
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
pvoid create_table_BASETYPE(type_resize type_resize, unsi dim){
  pelem_table_BASETYPE pelem_tmp;
  int                  i;
  ptable_info_BASETYPE ptable_and_elem;
  unsi                 actual_size = dim+1;


  /* STEP 1 */
  if((ptable_and_elem = malloc(sizeof(table_info_BASETYPE) + sizeof(elem_table_BASETYPE)*actual_size)) == NULL) return NULL;

  if((ptable_and_elem->plist_of_lists = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL){
    free(ptable_and_elem);
    return 0;
   }

  ptable_and_elem->n_entries = dim;
  ptable_and_elem->n_occupied = 0;

  ptable = ((pchar) ptable_and_elem) + sizeof(table_info_BASETYPE);
  pelem_tmp = (pelem_table_BASETYPE) ptable;
  pelem_tmp->idx_next = type_resize;
  pelem_tmp++;

  /* STEP 2 */
  for (i = 2; i < actual_size; i++, pelem_tmp++){
    pelem_tmp->idx_next = i;
   }
  pelem_tmp->idx_next = IDX_FINE_LISTA;

  #ifdef DEBUG_LIST_TABLE_BASETYPE
  printf("---- DEBUG CREATE_TABLE ----\n");
  printf("Creo tabella\n");
  printf("Ecco la tabella:\n");
  printf("idx,  val,  idx_next\n");
  for (i = 0, pelem_tmp = (pelem_table_BASETYPE) ptable; i < dim; i++) {
    printf("%d,    val%d,   %u\n", i, i, pelem_tmp->idx_next);
    pelem_tmp++;
    if(i>5) break;
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
int change_resize_table_table_BASETYPE(pvoid plist, type_resize type_resize){
  ((pelem_table_BASETYPE) ptable)->idx_next = type_resize;
  return 1;
 }

/* resize_table: cambia il numero di elementi complessivi della tabella che contiene plist
 * n_entries:    numero di elementi complessivi della tabella, dopo che è stata ridimensionata;
 *
 * return:       1 se ridimensionata correttamente, 0 altrimenti, ad esempio se
 * n_entries è minore del numero di elementi delle liste contenute.
 */
int resize_table_table_BASETYPE(pvoid plist, unsi n_entries){
  ptable_info_BASETYPE pmy_info;
  ptable_info_BASETYPE ptable_and_elem;
  pelem_table_BASETYPE pnew_table_start, pelem_moving;
  int                 i;
  unsi                actual_size = n_entries + 1, new_idx_void;
  plist_table_BASETYPE plist_extracted;

  pmy_info = ptable - sizeof(table_info_BASETYPE);
  /* CASO 1: dimensione troppo piccola
   * se la dimensione scelta e' minore del numero degli elementi attualmente contenuti
   * da' errore perche' non li puo' scartare */
  if(n_entries < pmy_info->n_occupied) return 0;

  /* CASO 2: ingrandimento
   * posso semplicemente allocare una zona di memoria piu' grande e fare un memcopy.
   * Poi aggiungo tutti gli elementi successivi in cima alla lista dei vuoti */
  if(n_entries > pmy_info->n_entries){
    /* alloco la nuovo table e inizializzo le info */
    if((ptable_and_elem = malloc(sizeof(table_info_BASETYPE) + sizeof(elem_table_BASETYPE) * actual_size)) == NULL) return 0;
    /* copio la tabella */
    memcpy(ptable_and_elem, pmy_info, sizeof(table_info_BASETYPE) + sizeof(elem_table_BASETYPE) * (pmy_info->n_entries + 1));

    ptable_and_elem->n_entries = n_entries;
    pnew_table_start = (pelem_table_BASETYPE)(ptable_and_elem + 1);

    /* collego tutti i nuovi elementi con una lista che termina all'inizio della lista dei vuoti */
    for (i = pmy_info->n_entries + 1, pelem_moving = pnew_table_start + i; i < n_entries; i++) {
      pelem_moving->idx_next = i + 1;
      pelem_moving = pelem_moving + 1;
     }
    pelem_moving->idx_next = idx_void_list;

    /* sposto l'inizio della lista dei vuoti all'inizio della nuova lista */
    idx_void_list = pmy_info->n_entries + 1;

    free(pmy_info);
    ptable = (pvoid)pnew_table_start;
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
    /* print_list(pmy_info->plist_of_lists, print_list_of_lists); */
    /* STEP 1 */
    if((ptable_and_elem = malloc(sizeof(table_info_BASETYPE) + sizeof(elem_table_BASETYPE) * actual_size)) == NULL) return 0;
    if((ptable_and_elem->plist_of_lists = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL){
      free(ptable_and_elem);
      return 0;
     }

    /* STEP 2
     * Per ricopiare sfrutto il fatto che la tabella e' vuota quindi ricopio ogni lista
     * in ordine, aumentando di volta in volta di 1 il new_idx_void */
    new_idx_void = 1;
    pnew_table_start = (pelem_table_BASETYPE)(ptable_and_elem + 1);
    while(extract_first(pmy_info->plist_of_lists, (all_type)((pvoid)&plist_extracted), NULL)){
      printf("Copio una lista\n");
      /* qui si devono aggiungere controlli appropriati */
      /* devo copiare il resize */
      insert_first(ptable_and_elem->plist_of_lists, (all_type)((pvoid)plist_extracted), 0);
      /* se ha 0 elementi basta aggiungere il posto della lista */
      if(plist_extracted->n_elem == 0){
        plist_extracted->idx_start = plist_extracted->idx_end = new_idx_void;
        new_idx_void++;
       }
      /* se ha piu' elementi li ricopio */
      else{
        copy_list_BASETYPE(pnew_table_start, &new_idx_void, ptable, &idx_void_list, ((plist_table_BASETYPE)plist_extracted)->idx_start);
        plist_extracted->idx_start = new_idx_void - plist_extracted->n_elem + 1;
        plist_extracted->idx_end = new_idx_void;
        new_idx_void++;
       }
     }
    /* STEP 3 */
    for (i = new_idx_void, pelem_moving = pnew_table_start + i; i < n_entries; i++) {
      pelem_moving->idx_next = i + 1;
      pelem_moving++;
     }
    pelem_moving->idx_next = IDX_FINE_LISTA;

    /* STEP 4 */
    ptable_and_elem->n_entries = n_entries;
    ptable_and_elem->n_occupied = pmy_info->n_occupied;
    free(pmy_info);
    ptable = (pvoid) (ptable_and_elem + 1);
    idx_void_list = new_idx_void;
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
int copy_list_BASETYPE(pelem_table_BASETYPE ptable_dest, punsi pidx_void_list_dest,
              pelem_table_BASETYPE ptable_orig, punsi pidx_void_list_orig,
              unsi idx_start){
  pelem_table_BASETYPE pelem_moving_dest, pelem_moving_origin;

  pelem_moving_dest = ptable_dest + *pidx_void_list_dest;
  pelem_moving_origin = ptable_orig + idx_start;
  while(pelem_moving_origin->idx_next != IDX_FINE_LISTA){
    pelem_moving_dest->val = pelem_moving_origin->val;
    pelem_moving_dest->idx_next = ++(*pidx_void_list_dest);
    pelem_moving_dest++;
    pelem_moving_origin = ptable_orig + pelem_moving_origin->idx_next;
   }
  pelem_moving_dest->val = pelem_moving_origin->val;
  pelem_moving_dest->idx_next = IDX_FINE_LISTA;
  return 1;
 }

/* get_info_table: fornisce informazioni sulla tabella che contiene plist
 * pn_entries: indirizzo in cui scrivere il numero di elementi complessivi della tabella;
 * pn_occupied: indirizzo in cui scrivere il numero di elementi occupati della tabella;
 *
 * return: 1 se tutto va bene, 0 altrimenti
 * */
int get_info_table_table_BASETYPE(pvoid plist,
                   punsi pn_entries,
                   punsi pn_occupied){
  ptable_info_BASETYPE pmy_info;

  pmy_info = ptable - sizeof(table_info_BASETYPE);
  *pn_entries = pmy_info->n_entries;
  *pn_occupied = pmy_info->n_occupied;
  return 1;
 }

/* free_list: libera la memoria occupata dalla lista
 * plist:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_table_BASETYPE(pvoid plist){
  pelem_table_BASETYPE pelem_start = (pelem_table_BASETYPE) ptable;
  plist_table_BASETYPE plist_to_free = (plist_table_BASETYPE) plist;
  ptable_info_BASETYPE pmy_info = ((ptable_info_BASETYPE) ptable) - 1;
  pvoid                pextracted;

  if(plist_to_free->n_elem == 0)
   {
    (pelem_start + plist_to_free->idx_start)->idx_next = idx_void_list;
    idx_void_list = plist_to_free->idx_start;

    /* tolgo la lista liberata dalla plist_of_lists */
    while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
      if(pextracted == plist_to_free){
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
  (pelem_start + plist_to_free->idx_end)->idx_next = idx_void_list;

  /* STEP 2 */
  idx_void_list = plist_to_free->idx_start;

  /* tolgo la lista liberata dalla plist_of_lists */
  while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
    if(pextracted == plist_to_free){
      break;
     }
    insert_last(pmy_info->plist_of_lists, (all_type)pextracted, 0);
   }
  (pmy_info->n_occupied)-= plist_to_free->n_elem;
  /* STEP 3 */
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
int insert_first_table_BASETYPE(pvoid plist, all_type value, unsi size){
  pelem_table_BASETYPE pfirst_free_elem;
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  int                  int_tmp;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0)
   {
    pfirst_free_elem = ((pelem_table_BASETYPE) ptable) + plist_casted->idx_start;
    pfirst_free_elem->val = value.MEMBERTYPE;
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA)
   {
    printf("Memoria preallocata piena\n");
    return 0;
   }
  /* STEP 1 */
  pfirst_free_elem = ((pelem_table_BASETYPE) ptable) + idx_void_list;

  /* STEP 2 */
  pfirst_free_elem->val = value.MEMBERTYPE;
  int_tmp = pfirst_free_elem->idx_next;
  pfirst_free_elem->idx_next = plist_casted->idx_start;

  /* STEP 3 */
  plist_casted->idx_start = idx_void_list;
  (plist_casted->n_elem)++;

  /* STEP 4 */
  idx_void_list = int_tmp;

  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)++;

  #ifdef DEBUG_LIST_TABLE_BASETYPE
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
int extract_first_table_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  pelem_table_BASETYPE pelem_to_extract;
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  pBASETYPE            pvalue_input = (pBASETYPE) pvalue.pv;
  int                  tmp_int;

  if(plist_casted->n_elem == 0)
   {
    return 0;
   }

  /* STEP 1 */
  pelem_to_extract = ((pelem_table_BASETYPE) ptable) + plist_casted->idx_start;

  /* STEP 2 */
  *pvalue_input = pelem_to_extract->val;

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
  tmp_int = pelem_to_extract->idx_next;
  pelem_to_extract->idx_next = idx_void_list;

  /* STEP 4 */
  idx_void_list = plist_casted->idx_start;

  /* STEP 5 */
  plist_casted->idx_start = tmp_int;
  (plist_casted->n_elem)--;
  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)--;

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
int insert_last_table_BASETYPE(pvoid plist, all_type value, unsi size){
  pelem_table_BASETYPE pfirst_free_elem;
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0){
    pfirst_free_elem = ((pelem_table_BASETYPE) ptable) + plist_casted->idx_start;
    pfirst_free_elem->val = value.MEMBERTYPE;
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA){
    printf("Memoria preallocata piena\n");
    return 0;
   }
  /* salvo l'indirizzo del primo elemento libero */
  pfirst_free_elem = ((pelem_table_BASETYPE) ptable) + idx_void_list;

  /* salvo le informazioni prese in input nell'elemento */
  pfirst_free_elem->val = value.MEMBERTYPE;

  /* aggiorno l'idx_next del penultimo elemento nell'indice del nuovo elemento */
  (((pelem_table_BASETYPE) ptable) + (plist_casted->idx_end))->idx_next = idx_void_list;

  /* aggiorno le informazioni sulla lista */
  plist_casted->idx_end = idx_void_list;
  (plist_casted->n_elem)++;

  /* aggiorno l'indice della lista dei vuoti e dell'ultimo elemento della lista */
  idx_void_list = pfirst_free_elem->idx_next;
  pfirst_free_elem->idx_next = IDX_FINE_LISTA;

  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)++;

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
int extract_last_table_BASETYPE(pvoid plist, all_type pvalue, punsi psize){
  pelem_table_BASETYPE pelem_moving, ptable_casted = (pelem_table_BASETYPE) ptable;
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  int                  idx_current, idx_end = plist_casted->idx_end;
  pBASETYPE            pvalue_input = (pBASETYPE) pvalue.pv;

  if(plist_casted->n_elem == 0){
    return 0;
   }

  /* se la lista ha 1 solo elemento non devo liberare lo spazio (ogni lista
   * ha almeno uno spazio), ma basta decrementare il n_elem,
   * perche' quando aggiungero' l'elemento successivo il valore contenuto
   * sara' sovrascritti essendo n_elem==0 */
  pelem_moving = ptable_casted + plist_casted->idx_start;
  if(plist_casted->n_elem == 1){
    *pvalue_input = pelem_moving->val;
    (plist_casted->n_elem)--;
    return 1;
   }

  /* raggiungo l'ultimo elemento con pelem_moving e conservo in idx_current l'indice del penultimo */
  idx_current = plist_casted->idx_start;
  while(pelem_moving->idx_next != idx_end){
    pelem_moving = ptable_casted + (idx_current = pelem_moving->idx_next);
   }
  pelem_moving = ptable_casted + pelem_moving->idx_next;

  /* scrivo IDX_FINE_LISTA nell'idx_next del penultimo elemento */
  (ptable_casted + idx_current)->idx_next = IDX_FINE_LISTA;

  /* restituisco il valore estratto */
  *pvalue_input = pelem_moving->val;

  /* metto l'ultimo elemento in cima alla lista dei vuoti */
  (pelem_moving->idx_next) = idx_void_list;
  idx_void_list = plist_casted->idx_end;

  /* aggiorno le informazioni della lista */
  plist_casted->idx_end = idx_current;
  (plist_casted->n_elem)--;
  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)--;

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
int insert_nth_table_BASETYPE(pvoid plist, all_type value, unsi size, unsi n){
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  pelem_table_BASETYPE pnew_elem, pelem_moving, ptable_casted = (pelem_table_BASETYPE) ptable;
  int                  int_tmp, i;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem + 1)) return 0;
  if(n == (plist_casted->n_elem + 1)) return insert_last(plist, value, size);
  if(n == 1) return insert_first(plist, value, size);

  if(idx_void_list == IDX_FINE_LISTA){
    printf("Memoria preallocata piena\n");
    return 0;
   }

  /* salvo l'indirizzo del nuovo elemento e vi salvo il valore preso in input */
  pnew_elem = ptable_casted + idx_void_list;
  pnew_elem->val = value.MEMBERTYPE;

  /* salvo il suo idx_next, che corrisponde al nuovo inizio della idx_void_list */
  int_tmp = pnew_elem->idx_next;

  /* raggiungo l'(n-1)-esimo elemento con pelem_moving */
  for (i = 2, pelem_moving = ptable_casted + plist_casted->idx_start; i < n; i++) {
    pelem_moving = ptable_casted + pelem_moving->idx_next;
  }
  /* metto pnew elem tra l'(n-1)-esimo e l'n-esimo elemento */
  pnew_elem->idx_next = pelem_moving->idx_next;
  pelem_moving->idx_next = idx_void_list;

  /* aggiorno l'indice di inizio della lista dei vuoti */
  idx_void_list = int_tmp;

  (plist_casted->n_elem)++;
  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)++;

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
int extract_nth_table_BASETYPE(pvoid plist, all_type pvalue, punsi psize, unsi n){
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  pelem_table_BASETYPE pelem_to_extract, pelem_moving, ptable_casted = (pelem_table_BASETYPE) ptable;
  int                  int_tmp, i;
  pBASETYPE            pvalue_input = (pBASETYPE)(pvalue.pv);

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem)) return 0;
  if(n == (plist_casted->n_elem)){
    return extract_last(plist, pvalue, psize);
   }
  if(n == 1){
    return extract_first(plist, pvalue, psize);
   }

  /* raggiungo con pelem_moving l'(n-1)-esimo elemento */
  for (i = 2, pelem_moving = ptable_casted + plist_casted->idx_start; i < n; i++) {
    pelem_moving = ptable_casted + pelem_moving->idx_next;
   }
  /* salvo in pelem_to_extract l'indirizzo dell'n-esimo elemento, ovvero quello da estrarre */
  pelem_to_extract = ptable_casted + pelem_moving->idx_next;

  /* restituisco il valore contenuto in pelem_to_extract */
  *pvalue_input = pelem_to_extract->val;
  int_tmp = pelem_to_extract->idx_next;

  /* cambio l'idx_next di pelem_to_extract in modo che diventi il primo elemento della void_list */
  pelem_to_extract->idx_next = idx_void_list;
  idx_void_list = pelem_moving->idx_next;

  /* aggiorno l'idx_next dell'(n-1)-esimo elemento */
  pelem_moving->idx_next = int_tmp;

  (plist_casted->n_elem)--;
  ((((ptable_info_BASETYPE)ptable)-1)->n_occupied)--;

  return 1;
 }

/* get_n_elem: restituisce il numero di elementi della lista
 * plist:      lista di cui si vuole conoscere il numero di elementi
 *
 * return:     numero di elementi della lista
 * */
unsi get_n_elem_table_BASETYPE(pvoid plist){
  return ((plist_table_BASETYPE) plist)->n_elem;
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
int search_first_table_BASETYPE(pvoid plist,
                         all_type value_searched, unsi size_searched,
                         all_type pvalue_found,   punsi psize_found,
                         pcustom_compare pinput_compare){
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  pelem_table_BASETYPE pelem_moving, ptable_casted = (pelem_table_BASETYPE) ptable;

  if (pinput_compare == NULL) return 0;
  pelem_moving = ptable_casted + plist_casted->idx_start;
  while(pelem_moving != NULL){
    if(!pinput_compare(value_searched, 0,(all_type)pelem_moving->val, 0)){
      *((pBASETYPE) pvalue_found.pv) = pelem_moving->val;
      return 1;
     }
    pelem_moving = ptable_casted + pelem_moving->idx_next;
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
int print_list_table_BASETYPE(pvoid plist, pcustom_print pinput_print){
  plist_table_BASETYPE plist_casted = (plist_table_BASETYPE) plist;
  pelem_table_BASETYPE pelem_start = (pelem_table_BASETYPE) ptable, pelem_tmp;
  int i=0;

  printf("type_list: type_list_table\n");
  printf("type_data: BASETYPE\n");
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  #ifdef DEBUG_LIST_TABLE_BASETYPE
  printf("---- DEBUG PRINT_LIST ----\n");
  printf("Idx_start: %d, idx_end: %d, n_elem: %d, idx_void_list: %d\n", plist_casted->idx_start, plist_casted->idx_end, plist_casted->n_elem, idx_void_list);
  printf("Tipo di resize: %d\n", ((pelem_table_BASETYPE)ptable)->idx_next);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  if((plist_casted->n_elem == 0) || (pinput_print == NULL)) return 1;

  pelem_tmp = pelem_start + plist_casted->idx_start;
  while(pelem_tmp->idx_next != IDX_FINE_LISTA)
   {
    i++;
    if(i == 5) break;
    if(!(pinput_print((all_type)(pelem_tmp->val), 0))) return 0;
    printf("->");
    pelem_tmp = pelem_start + pelem_tmp->idx_next;
   }
  if(!(pinput_print((all_type)(pelem_tmp->val), 0))) return 0;
  printf("\n");

  return 1;
 }

void print_table_BASETYPE(){
  ptable_info_BASETYPE pmy_info;
  int                 i;
  pelem_table_BASETYPE pelem_moving;

  printf("Stampo tabella a indirizzo %lu\n", (long) ptable);
  printf("idx_void_list: %u\n", idx_void_list);
  pmy_info = ptable - sizeof(table_info_BASETYPE);
  for (i = 0; i < (pmy_info->n_entries + 1); i++) {
    pelem_moving = ((pelem_table_BASETYPE) ptable) + i;
    printf("%d -> %d\n", i, pelem_moving->idx_next);
  }
 }

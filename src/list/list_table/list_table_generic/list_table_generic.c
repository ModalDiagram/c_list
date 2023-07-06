#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./../../../util/defines_typedef.h"
#include "./../../list.h"
#include "list_table_generic.hidden"
#include "list_table_generic.h"

/* #define DEBUG_LIST_TABLE_GENERIC */

/* questi due li metto static perche' li uso sempre e non li voglio calcolare.
 * il n_entries e n_occupied lo metto subito prima della table perche' non mi servono sempre e per non
 * mettere troppi static (li alloco solo quando mi serve la table) */
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
  ptable_info_generic pmy_info;

  if(ptable == NULL){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("---- DEBUG MALLOC ----\n");
    printf("Tabella non esiste\n");
    printf("---- FINE DEBUG ----\n\n");
    #endif
    if((create_table_generic(type_resize, dim_table)) == NULL) return NULL;
   }

  if(idx_void_list == IDX_FINE_LISTA){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("Memoria preallocata piena\n");
    #endif
    if((((pelem_table_generic)ptable)->idx_next) == type_resize_default){
      if(!expand_table_fit_generic()) return 0;
      #ifdef DEBUG_LIST_TABLE_GENERIC
      printf("Ridimensionata\n");
      #endif
     }
    else return 0;
   }

  /* STEP 1 */
  if((pnew_list = (plist_table_generic) malloc(sizeof(plist_table_generic))) == NULL) return NULL;

  pmy_info = ((ptable_info_generic)ptable) - 1;
  /* devo aggiungere la lista creata alla lista di liste contenute nella table */
  if(!insert_first(pmy_info->plist_of_lists, (all_type)((pvoid)pnew_list), 0)){
    free(pnew_list);
    return 0;
   }

  pnew_list->n_elem = 0;
  pnew_list->idx_start = idx_void_list;
  pnew_list->idx_end = idx_void_list;

  /* STEP 2 */
  pfirst_elem_of_new_list = ((pelem_table_generic) ptable) + idx_void_list;
  idx_void_list = pfirst_elem_of_new_list->idx_next;

  /* STEP 3 */
  pfirst_elem_of_new_list->idx_next = IDX_FINE_LISTA;

  (pmy_info->n_occupied)++;
  if(!((++(pmy_info->n_insert)) % OCCUP_FREQ)){
    manage_moving_window_generic(pmy_info);
   }


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
pvoid create_table_generic(type_resize type_resize, unsi dim)
 {
  pelem_table_generic pelem_tmp;
  int                 i;
  ptable_info_generic ptable_and_elem;
  unsi                actual_size = dim+1;


  /* STEP 1 */
  if((ptable_and_elem = malloc(sizeof(table_info_generic) + sizeof(elem_table_generic)*actual_size)) == NULL) return NULL;

  if((ptable_and_elem->plist_of_lists = malloc_list(type_list_dynamic, "PVOID", 1)) == NULL){
    free(ptable_and_elem);
    return 0;
   }

  if((ptable_and_elem->plist_of_occupations = malloc_list(type_list_dynamic, "DOUBLE", 2)) == NULL){
    free(ptable_and_elem->plist_of_lists);
    free(ptable_and_elem);
    return 0;
   }

  ptable_and_elem->n_entries = dim;
  ptable_and_elem->n_occupied = 0;

  ptable = ((pchar) ptable_and_elem) + sizeof(table_info_generic);
  pelem_tmp = (pelem_table_generic) ptable;
  pelem_tmp->idx_next = type_resize;
  pelem_tmp++;

  /* STEP 2 */
  for (i = 2; i < actual_size; i++, pelem_tmp++){
    pelem_tmp->idx_next = i;
   }
  pelem_tmp->idx_next = IDX_FINE_LISTA;

  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG CREATE_TABLE ----\n");
  printf("Creo tabella\n");
  printf("Ecco la tabella:\n");
  printf("idx,  val,  idx_next\n");
  for (i = 0, pelem_tmp = (pelem_table_generic) ptable; i < actual_size; i++) {
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
int change_resize_table_table_generic(pvoid plist, type_resize type_resize){
  ((pelem_table_generic) ptable)->idx_next = type_resize;
  return 1;
 }

int print_list_of_lists(all_type plist, unsi size){
  printf("Idx_start: %u\n", ((plist_table_generic)(plist.pv))->idx_start);
  return 1;
 }

/* resize_table: cambia il numero di elementi complessivi della tabella che contiene plist
 * n_entries:    numero di elementi complessivi della tabella, dopo che è stata ridimensionata;
 *
 * return:       1 se ridimensionata correttamente, 0 altrimenti, ad esempio se
 * n_entries è minore del numero di elementi delle liste contenute.
 */
int resize_table_table_generic(pvoid plist, unsi n_entries){
  ptable_info_generic pmy_info;
  ptable_info_generic ptable_and_elem;
  pelem_table_generic pnew_table_start, pelem_moving;
  int                 i, n_list_to_copy;
  unsi                actual_size = n_entries + 1, new_idx_void;
  plist_table_generic plist_extracted;

  pmy_info = ptable - sizeof(table_info_generic);
  /* CASO 1: dimensione troppo piccola
   * se la dimensione scelta e' minore del numero degli elementi attualmente contenuti
   * da' errore perche' non li puo' scartare */
  if(n_entries < pmy_info->n_occupied) return 0;

  /* CASO 2: ingrandimento
   * posso semplicemente allocare una zona di memoria piu' grande e fare un memcopy.
   * Poi aggiungo tutti gli elementi successivi in cima alla lista dei vuoti */
  if(n_entries > pmy_info->n_entries){
    /* alloco la nuovo table */
    if((ptable_and_elem = malloc(sizeof(table_info_generic) + sizeof(elem_table_generic) * actual_size)) == NULL) return 0;
    /* copio la tabella */
    memcpy(ptable_and_elem, pmy_info, sizeof(table_info_generic) + sizeof(elem_table_generic) * (pmy_info->n_entries + 1));

    /* svuoto la lista contenente le medie dell'occupazione perche' le calcoliamo
     * per ciascuna tabella separatamente */
    while(extract_first(ptable_and_elem->plist_of_occupations, (all_type)NULL, NULL)){}
    ptable_and_elem->n_entries = n_entries;
    ptable_and_elem->n_insert = 0;
    pnew_table_start = (pelem_table_generic)(ptable_and_elem + 1);

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
   * 1) Alloco la nuova table
   * 2) prendo ciascuna lista contenuta nella vecchia table, la ricopio e la reinserisco
   *    nella list_of_lists
   * 3) metto tutti gli elementi successivi in coda alla lista dei vuoti
   * 4) aggiorno info e sostituisco vecchia table
   * */
  else{
    /* print_list(pmy_info->plist_of_lists, print_list_of_lists); */
    /* STEP 1 */
    /* alloco nuova table */
    if((ptable_and_elem = malloc(sizeof(table_info_generic) + sizeof(elem_table_generic) * actual_size)) == NULL) return 0;

    ptable_and_elem->plist_of_lists = pmy_info->plist_of_lists;
    /* STEP 2
     * Per ricopiare sfrutto il fatto che la tabella e' vuota quindi ricopio ogni lista
     * in ordine, aumentando di volta in volta di 1 il new_idx_void */
    new_idx_void = 1;
    pnew_table_start = (pelem_table_generic)(ptable_and_elem + 1);
    n_list_to_copy = get_n_elem(ptable_and_elem->plist_of_lists);
    for (i = 0; i < n_list_to_copy; i++) {
      extract_first(ptable_and_elem->plist_of_lists, (all_type)((pvoid)&plist_extracted), NULL);
      #ifdef DEBUG_LIST_TABLE_GENERIC
      printf("Copio una lista\n");
      #endif
      insert_last(ptable_and_elem->plist_of_lists, (all_type)((pvoid)plist_extracted), 0);
      /* se ha 0 elementi basta aggiungere il posto della lista */
      if(plist_extracted->n_elem == 0){
        plist_extracted->idx_start = plist_extracted->idx_end = new_idx_void;
        new_idx_void++;
       }
      /* se ha piu' elementi li ricopio */
      else{
        copy_list_generic(pnew_table_start, &new_idx_void, ptable, &idx_void_list, ((plist_table_generic)plist_extracted)->idx_start);
        plist_extracted->idx_start = new_idx_void - plist_extracted->n_elem + 1;
        plist_extracted->idx_end = new_idx_void;
        new_idx_void++;
       }
     }
    /* STEP 3 */
    /* metto tutti gli elementi successivi in coda alla lista dei vuoti */
    for (i = new_idx_void, pelem_moving = pnew_table_start + i; i < n_entries; i++) {
      pelem_moving->idx_next = i + 1;
      pelem_moving++;
     }
    pelem_moving->idx_next = IDX_FINE_LISTA;

    /* STEP 4 */
    /* aggiorno info */
    pnew_table_start->idx_next = ((pelem_table_generic) ptable)->idx_next;
    ptable_and_elem->n_entries = n_entries;
    ptable_and_elem->n_occupied = pmy_info->n_occupied;
    ptable_and_elem->n_insert = 0;
    ptable_and_elem->plist_of_occupations = pmy_info->plist_of_occupations;
    /* svuoto la lista contenente le medie dell'occupazione perche' le calcoliamo
     * per ciascuna tabella separatamente */
    while(extract_first(ptable_and_elem->plist_of_occupations, (all_type)NULL, NULL)){}
    /* libero la vecchia tabella */
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
int copy_list_generic(pelem_table_generic ptable_dest, punsi pidx_void_list_dest,
              pelem_table_generic ptable_orig, punsi pidx_void_list_orig,
              unsi idx_start){
  pelem_table_generic pelem_moving_dest, pelem_moving_origin;

  /* sfrutto il fatto che la lista di destinazione è fatta solo dalla
   * lista dei vuoti ordinata.
   * pelem_moving_dest scorre lungo la tabella di destinazione un elemento alla volta
   * mentre pelem_moving_origin salta lungo la tabella di origine passando
   * per tutti gli elementi della lista.
   * Alla fine quindi le liste ricopiate risultano anche ordinate */
  pelem_moving_dest = ptable_dest + *pidx_void_list_dest;
  pelem_moving_origin = ptable_orig + idx_start;
  while(pelem_moving_origin->idx_next != IDX_FINE_LISTA){
    pelem_moving_dest->paddr = pelem_moving_origin->paddr;
    pelem_moving_dest->size = pelem_moving_origin->size;
    pelem_moving_dest->idx_next = ++(*pidx_void_list_dest);
    pelem_moving_dest++;
    pelem_moving_origin = ptable_orig + pelem_moving_origin->idx_next;
   }
  pelem_moving_dest->paddr = pelem_moving_origin->paddr;
  pelem_moving_dest->size = pelem_moving_origin->size;
  pelem_moving_dest->idx_next = IDX_FINE_LISTA;
  return 1;
 }

/* get_info_table: fornisce informazioni sulla tabella che contiene plist
 * pn_entries: indirizzo in cui scrivere il numero di elementi complessivi della tabella;
 * pn_occupied: indirizzo in cui scrivere il numero di elementi occupati della tabella;
 *
 * return: 1 se tutto va bene, 0 altrimenti
 * */
int get_info_table_table_generic(pvoid plist,
                   punsi pn_entries,
                   punsi pn_occupied){
  ptable_info_generic pmy_info;

  pmy_info = ptable - sizeof(table_info_generic);
  *pn_entries = pmy_info->n_entries;
  *pn_occupied = pmy_info->n_occupied;

  return 1;
 }

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list_table_generic(pvoid plist){
  pelem_table_generic pelem_start = (pelem_table_generic) ptable, pelem_tmp;
  plist_table_generic plist_to_free = (plist_table_generic) plist;
  ptable_info_generic pmy_info = ((ptable_info_generic) ptable) - 1;
  pvoid               pextracted;

  /* se la lista ha zero elementi non bisogna scorrere sulla lista ma basta
   * liberare l'elemento che contiene la struttura lista */
  if(plist_to_free->n_elem == 0)
   {
    (pelem_start + plist_to_free->idx_start)->idx_next = idx_void_list;
    idx_void_list = plist_to_free->idx_start;

    /* tolgo la lista liberata dalla list_of_lists */
    while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
      if(pextracted == plist_to_free){
        break;
       }
      insert_last(pmy_info->plist_of_lists, (all_type)pextracted, 0);
     }

    free(plist);
    ((((ptable_info_generic)ptable)-1)->n_occupied)--;
    #ifdef DEBUG_LISTA_VELOCE
    printf("---- DEBUG ----\n");
    printf("Lista liberata. Indirizzo lista dei vuoti: %d\n", idx_void_list);
    printf("---- FINE DEBUG ----\n");
    #endif
    return;
   }

  /* Scorro lungo la lista per liberare gli indirizzi contenuti */
  pelem_tmp = pelem_start + plist_to_free->idx_start;
  while((pelem_tmp->idx_next) != IDX_FINE_LISTA)
   {
    free(pelem_tmp->paddr);
    pelem_tmp = pelem_start + (pelem_tmp->idx_next);
   }
  free(pelem_tmp->paddr);

  /* cambio l'idx_next dell'ultimo elemento della lista in idx_void_list */
  pelem_tmp->idx_next = idx_void_list;
  /* cambio idx_void_list nell'indice del primo elemento della lista.
   * In questo modo ho messo la lista in cima alla void_list */
  idx_void_list = plist_to_free->idx_start;

  /* tolgo la lista liberata dalla list_of_lists */
  while (extract_first(pmy_info->plist_of_lists, (all_type)((pvoid) &pextracted), NULL)) {
    if(pextracted == plist_to_free){
      break;
     }
    insert_last(pmy_info->plist_of_lists, (all_type)pextracted, 0);
   }
  (pmy_info->n_occupied)-=plist_to_free->n_elem;

  /* libero la struttura lista */
  free(plist);

  #ifdef DEBUG_LISTA_VELOCE
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
int insert_first_table_generic(pvoid plist, all_type value, unsi size){
  pelem_table_generic pfirst_free_elem;
  plist_table_generic plist_casted = (plist_table_generic) plist;
  ptable_info_generic pmy_info;
  int                 int_tmp;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0)
   {
    pfirst_free_elem = ((pelem_table_generic) ptable) + plist_casted->idx_start;
    if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
    memcpy(pfirst_free_elem->paddr,value.pv,(pfirst_free_elem->size)=size);
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("Memoria preallocata piena\n");
    #endif
    if((((pelem_table_generic) ptable)->idx_next) == type_resize_default){
      if(!expand_table_fit_generic()) return 0;
      #ifdef DEBUG_LIST_TABLE_GENERIC
      printf("Ridimensionata\n");
      #endif
     }
    else return 0;
   }
  pmy_info  = ((ptable_info_generic) ptable) - 1;
  /* salvo l'indirizzo del nuovo elemento */
  pfirst_free_elem = ((pelem_table_generic) ptable) + idx_void_list;

  /* scrivo il valore del nuovo elemento e scrivo nell'idx_next l'inizio della lista */
  if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
  memcpy(pfirst_free_elem->paddr,value.pv,(pfirst_free_elem->size)=size);
  int_tmp = pfirst_free_elem->idx_next;
  pfirst_free_elem->idx_next = plist_casted->idx_start;

  /* sposto l'inizio della lista al nuovo elemento */
  plist_casted->idx_start = idx_void_list;
  (plist_casted->n_elem)++;

  /* sposto l'inizio della void_list all'idx_next del nuovo elemento */
  idx_void_list = int_tmp;

  (pmy_info->n_occupied)++;
  if(!((++(pmy_info->n_insert)) % OCCUP_FREQ)){
    manage_moving_window_generic(pmy_info);
   }

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
int extract_first_table_generic(pvoid plist, all_type pvalue, punsi psize){
  pelem_table_generic pelem_to_extract;
  plist_table_generic plist_casted = (plist_table_generic) plist;
  ppvoid              ppvalue_input = pvalue.pv;
  int                 tmp_int;

  if(plist_casted->n_elem == 0)
   {
    return 0;
   }

  /* salvo l'indirizzo dell'elemento da rimuovere */
  pelem_to_extract = ((pelem_table_generic) ptable) + plist_casted->idx_start;

  /* restituisco il valore contenuto nell'elemento */
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_to_extract->paddr;
   }
  if(psize != NULL){
    *psize = pelem_to_extract->size;
   }

  /* se la lista ha 1 solo elemento non devo liberare lo spazio (ogni lista
   * ha almeno uno spazio), ma basta decrementare il n_elem,
   * perche' quando aggiungero' l'elemento successivo paddr e size
   * saranno sovrascritti essendo n_elem==0 */
  if(plist_casted->n_elem == 1)
   {
    (plist_casted->n_elem)--;
    return 1;
   }

  /* metto l'elemento da estrarre in cima alla lista dei vuoti */
  tmp_int = pelem_to_extract->idx_next;
  pelem_to_extract->idx_next = idx_void_list;
  idx_void_list = plist_casted->idx_start;

  /* sposto l'inizio della lista */
  plist_casted->idx_start = tmp_int;
  (plist_casted->n_elem)--;

  ((((ptable_info_generic)ptable)-1)->n_occupied)--;

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
int insert_last_table_generic(pvoid plist, all_type value, unsi size){
  pelem_table_generic pfirst_free_elem, ptable_casted = (pelem_table_generic) ptable;
  plist_table_generic plist_casted = (plist_table_generic) plist;
  ptable_info_generic pmy_info;

  /* se la lista ha 0 elementi ha gia' uno spazio nella tabella
   * ma con paddr e size vuoti, quindi e' sufficiente che scriva li' */
  if(plist_casted->n_elem == 0){
    pfirst_free_elem = ((pelem_table_generic) ptable) + plist_casted->idx_start;
    if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
    memcpy(pfirst_free_elem->paddr,value.pv,(pfirst_free_elem->size)=size);
    (plist_casted->n_elem)++;
    return 1;
   }

  if(idx_void_list == IDX_FINE_LISTA){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("Memoria preallocata piena\n");
    #endif
    if((ptable_casted->idx_next) == type_resize_default){
      if(!expand_table_fit_generic()) return 0;
      ptable_casted = (pelem_table_generic) ptable;
      #ifdef DEBUG_LIST_TABLE_GENERIC
      printf("Ridimensionata\n");
      #endif
     }
    else return 0;
   }
  pmy_info  = ((ptable_info_generic) ptable) - 1;
  /* salvo l'indirizzo del primo elemento libero */
  pfirst_free_elem = ptable_casted + idx_void_list;

  /* salvo le informazioni prese in input nell'elemento */
  if((pfirst_free_elem->paddr = malloc(size)) == NULL) return 0;
  memcpy(pfirst_free_elem->paddr,value.pv,(pfirst_free_elem->size)=size);

  /* aggiorno l'idx_next del penultimo elemento nell'indice del nuovo elemento */
  (ptable_casted + (plist_casted->idx_end))->idx_next = idx_void_list;

  /* aggiorno le informazioni sulla lista */
  plist_casted->idx_end = idx_void_list;
  (plist_casted->n_elem)++;

  /* aggiorno l'indice della lista dei vuoti e dell'ultimo elemento della lista */
  idx_void_list = pfirst_free_elem->idx_next;
  pfirst_free_elem->idx_next = IDX_FINE_LISTA;

  (pmy_info->n_occupied)++;
  if(!((++(pmy_info->n_insert)) % OCCUP_FREQ)){
    manage_moving_window_generic(pmy_info);
   }

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
int extract_last_table_generic(pvoid plist, all_type pvalue, punsi psize){
  pelem_table_generic pelem_moving, ptable_casted = (pelem_table_generic) ptable;
  plist_table_generic plist_casted = (plist_table_generic) plist;
  unsi                idx_current, idx_end = plist_casted->idx_end;
  ppvoid              ppvalue_input = pvalue.pv;

  if(plist_casted->n_elem == 0){
    return 0;
   }

  /* se la lista ha 1 solo elemento non devo liberare lo spazio (ogni lista
   * ha almeno uno spazio), ma basta decrementare il n_elem,
   * perche' quando aggiungero' l'elemento successivo il valore contenuto
   * sara' sovrascritti essendo n_elem==0 */
  pelem_moving = ptable_casted + plist_casted->idx_start;
  if(plist_casted->n_elem == 1){
    if(ppvalue_input != NULL){
      *ppvalue_input = pelem_moving->paddr;
     }
    if(psize != NULL){
      *psize = pelem_moving->size;
     }
    (plist_casted->n_elem)--;
    return 1;
   }

  /* raggiungo l'ultimo elemento con pelem_moving e conservo in idx_current l'indice del penultimo */
  idx_current = plist_casted->idx_start;
  while(pelem_moving->idx_next != idx_end){
    pelem_moving = ptable_casted + (idx_current = pelem_moving->idx_next);
   }

  /* scrivo IDX_FINE_LISTA nell'idx_next del penultimo elemento */
  pelem_moving->idx_next = IDX_FINE_LISTA;
  pelem_moving = ptable_casted + idx_end;

  /* restituisco il valore estratto */
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_moving->paddr;
   }
  if(psize != NULL){
    *psize = pelem_moving->size;
   }

  /* metto l'ultimo elemento in cima alla lista dei vuoti */
  (pelem_moving->idx_next) = idx_void_list;
  idx_void_list = plist_casted->idx_end;

  /* aggiorno le informazioni della lista */
  plist_casted->idx_end = idx_current;
  (plist_casted->n_elem)--;

  ((((ptable_info_generic)ptable)-1)->n_occupied)--;

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
int insert_nth_table_generic(pvoid plist, all_type value, unsi size, unsi n){
  plist_table_generic plist_casted = (plist_table_generic) plist;
  pelem_table_generic pnew_elem, pelem_moving, ptable_casted = (pelem_table_generic) ptable;
  int                  int_tmp, i;
  ptable_info_generic pmy_info  = ((ptable_info_generic) ptable) - 1;

  /* gestisco casi particolari */
  if(n > (plist_casted->n_elem + 1)) return 0;
  if(n == (plist_casted->n_elem + 1)) return insert_last(plist, value, size);
  if(n == 1) return insert_first(plist, value, size);

  if(idx_void_list == IDX_FINE_LISTA){
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("Memoria preallocata piena\n");
    #endif
    if((ptable_casted->idx_next) == type_resize_default){
      if(!expand_table_fit_generic()) return 0;
      pmy_info  = ((ptable_info_generic) ptable) - 1;
      ptable_casted = (pelem_table_generic) ptable;
      #ifdef DEBUG_LIST_TABLE_GENERIC
      printf("Ridimensionata\n");
      #endif
     }
    else return 0;
   }

  /* salvo l'indirizzo del nuovo elemento e vi salvo il valore preso in input */
  pnew_elem = ptable_casted + idx_void_list;
  if((pnew_elem->paddr = malloc(size)) == NULL) return 0;
  memcpy(pnew_elem->paddr,value.pv,(pnew_elem->size)=size);

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
  (pmy_info->n_occupied)++;
  if(!((++(pmy_info->n_insert)) % OCCUP_FREQ)){
    manage_moving_window_generic(pmy_info);
   }

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
int extract_nth_table_generic(pvoid plist, all_type pvalue, punsi psize, unsi n){
  plist_table_generic plist_casted = (plist_table_generic) plist;
  pelem_table_generic pelem_to_extract, pelem_moving, ptable_casted = (pelem_table_generic) ptable;
  int                  int_tmp, i;
  ppvoid              ppvalue_input = (ppvoid) (pvalue.pv);

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
  if(ppvalue_input != NULL){
    *ppvalue_input = pelem_to_extract->paddr;
   }
  if(psize != NULL){
    *psize = pelem_to_extract->size;
   }
  int_tmp = pelem_to_extract->idx_next;

  /* cambio l'idx_next di pelem_to_extract in modo che diventi il primo elemento della void_list */
  pelem_to_extract->idx_next = idx_void_list;
  idx_void_list = pelem_moving->idx_next;

  /* aggiorno l'idx_next dell'(n-1)-esimo elemento */
  pelem_moving->idx_next = int_tmp;

  (plist_casted->n_elem)--;
  ((((ptable_info_generic)ptable)-1)->n_occupied)--;

  return 1;
 }

/* get_n_elem: restituisce il numero di elementi della lista
 * plist:      lista di cui si vuole conoscere il numero di elementi
 *
 * return:     numero di elementi della lista
 * */
unsi get_n_elem_table_generic(pvoid plist){
  return ((plist_table_generic) plist)->n_elem;
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
                         all_type value_searched, unsi size_searched,
                         all_type pvalue_found,   punsi psize_found,
                         pcustom_compare pinput_compare){
  plist_table_generic plist_casted = (plist_table_generic) plist;
  pelem_table_generic pelem_moving, ptable_casted = (pelem_table_generic) ptable;

  if (pinput_compare == NULL) return 0;
  pelem_moving = ptable + plist_casted->idx_start;
  while(pelem_moving != NULL){
    if(!pinput_compare(value_searched, size_searched,(all_type)pelem_moving->paddr, pelem_moving->size)){
      *((ppvoid)(pvalue_found.pv)) = pelem_moving->paddr;
      *psize_found = pelem_moving->size;
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
int print_list_table_generic(pvoid plist, pcustom_print pinput_print){
  plist_table_generic plist_casted = (plist_table_generic) plist;
  pelem_table_generic pelem_start = (pelem_table_generic) ptable, pelem_tmp;
  int i=0;

  printf("type_list: type_list_table\n");
  printf("type_data: generic\n");
  printf("Numero di elementi della lista: %u\n", plist_casted->n_elem);
  #ifdef DEBUG_LIST_TABLE_GENERIC
  printf("---- DEBUG PRINT_LIST ----\n");
  printf("Idx_start: %d, idx_end: %d, n_elem: %d, idx_void_list: %d\n", plist_casted->idx_start, plist_casted->idx_end, plist_casted->n_elem, idx_void_list);
  printf("Tipo di resize: %d\n", ((pelem_table_generic)ptable)->idx_next);
  printf("---- FINE DEBUG ----\n\n");
  #endif
  if((plist_casted->n_elem == 0) || (pinput_print == NULL)) return 1;

  pelem_tmp = pelem_start + plist_casted->idx_start;
  while(pelem_tmp->idx_next != IDX_FINE_LISTA)
   {
    i++;
    if(i == 5) break;
    if(!(pinput_print((all_type)(pelem_tmp->paddr), pelem_tmp->size))) return 0;
    /* printf("letto %f\n", *((pfloat)(pelem_tmp->paddr))); */
    printf("->");
    pelem_tmp = pelem_start + pelem_tmp->idx_next;
   }
  if(!(pinput_print((all_type)(pelem_tmp->paddr), pelem_tmp->size))) return 0;
  printf("\n");

  return 1;
 }

void print_table_generic(){
  ptable_info_generic pmy_info;
  int                 i;
  pelem_table_generic pelem_moving;

  printf("Stampo tabella a indirizzo %lu\n", (long) ptable);
  printf("idx_void_list: %u\n", idx_void_list);
  pmy_info = ptable - sizeof(table_info_generic);
  for (i = 0; i < (pmy_info->n_entries + 1); i++) {
    pelem_moving = ((pelem_table_generic) ptable) + i;
    printf("%d -> %d\n", i, pelem_moving->idx_next);
  }
 }

/* manage_moving_window: gestisce la finestra mobile da cui si calcola l'occupazione media */
void manage_moving_window_generic(pvoid pinfo_table){
  ptable_info_generic pmy_info = (ptable_info_generic) pinfo_table;
  pdouble             pmoving_array = pmy_info->pmoving_window;
  int i;
  double sum = 0;
  double pfit_var[2];
  unsi   my_idx = (pmy_info->n_insert / OCCUP_FREQ) - 1;

  /* l'array con l'indice che scorre simula una coda. L'indice aumenta di 1 ogni volta
   * che viene chiamata questa funzione e mi muovo lungo l'array. Non esco dalla zona
   * di memoria perche' prendo il resto */
  pmoving_array[my_idx % DIM_MOVING_WINDOW] = (double) pmy_info->n_occupied;
  /* se l'array e' pieno calcolo la media e la salvo in plist_of_occupations
   * assieme al n_insert attuale */
  if(my_idx>= DIM_MOVING_WINDOW){
    for (i = 0; i < DIM_MOVING_WINDOW; i++) {
      sum += pmoving_array[i];
     }
    pfit_var[0] = pmy_info->n_insert;
    pfit_var[1] = sum / DIM_MOVING_WINDOW;
    insert_last(pmy_info->plist_of_occupations, (all_type) ((pvoid) pfit_var), 0);
    /* printf("Calcolo media mobile %f\n", sum /DIM_MOVING_WINDOW); */
    /* printf("aggiunta media di occupazione: %f\n", sum / DIM_MOVING_WINDOW); */
   }
  return;
 }

/* expand_table_fit: calcola con un fit lineare di quanto dovrebbe crescere la tabella
 *                   e la ridimensiona
 *
 * return: 1 se tutto va bene, 0 altrimenti
 */
int expand_table_fit_generic(){
  ptable_info_generic pmy_info = ((ptable_info_generic) ptable) - 1;
  pvoid               plist_of_occupations = pmy_info->plist_of_occupations;
  int                 n_elem = get_n_elem(plist_of_occupations), i;
  double              x, y, sumx=0, sumy=0, sumxy=0, sumx2=0;
  double              a,b;
  double*             pd;
  double              n_entries_bigger;

  /* se non ho abbastanza dati per fare il fit, aumento la tabella di 3 volte */
  if(n_elem < 5){
    resize_table_table_generic(NULL, 3 * pmy_info->n_entries);
    /* printf("Ridimensiono da %u a dim: %u\n",pmy_info->n_entries, 3*pmy_info->n_entries); */
    return 1;
   }
  /* se ho abbastanza dati faccio fit */
  else{
    for (i = 1; i <= n_elem; i++) {
      extract_first(plist_of_occupations, (all_type)((pvoid)&pd), 0);
      x = pd[0];
      y = pd[1];
      sumx += x;
      sumy += y;
      sumxy += x * y;
      sumx2 += x * x;
    }
    /* a e b sono i coefficienti della retta
     * occupaz% = a * n_insert + b
     * per scegliere l'occupazione futura dobbiamo scegliere il valore di n_insert
     * a cui secondo la legge avvera' il prossimo ridimensionamento.
     * Lo scelgo con una legge esponenziale
     * 1.5 + 2.5*e^(0.5*(1-velocita_occupazione))
     * dove velocita_occupazione = n_insert / n_entries.
     * Se ci sono solo inserimenti (velocita_occupazione = 1)
     * il numero di inserimenti previsti aumenta di 4 volte, mentre quando il numero di
     * inserimenti netti è molto piccolo il numero aumenta di 1.5 volte
     * */
    a = (n_elem*sumxy - sumx*sumy) / (n_elem * sumx2 - sumx * sumx);
    b = (sumy * sumx2 - sumx * sumxy) / (n_elem * sumx2 - sumx * sumx);
    /* printf("Coefficienti a %f b %f\n", a, b); */
    /* printf("Fattore %f\n", tmp1); */
    n_entries_bigger = a * pmy_info->n_insert*(1.5+2.5*exp(0.5*(1-(((double)pmy_info->n_insert) / pmy_info->n_entries)))) + b;
    /* printf("Occupazione futura: %f\n", tmp2); */
    #ifdef DEBUG_LIST_TABLE_GENERIC
    printf("Ridimensiono da %u a dim: %f\n",pmy_info->n_entries, n_entries_bigger);
    #endif
    return resize_table_table_generic(NULL, n_entries_bigger);
   }
  return 0;
 }

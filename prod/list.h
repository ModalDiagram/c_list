#ifndef _LIST
#define _LIST

#include "./defines_typedef.h"

/* Questa libreria fornisce all'utente l'oggetto "list".
 * La complessita' delle funzioni non dipende dalla lunghezza della lista:
 * - nell'inserimento e rimozione in testa
 * - nell'inserimento in coda
 * ma vi dipende:
 * - nella rimozione in coda
 * - nell'inserimento e rimozione all'interno della lista
 * - nella ricerca di un elemento specifico all'interno della lista
 *
 * Sono fornite due versioni:
 * - list_dynamic: utile nel caso in cui il numero di elementi da inserire o estrarre
 *   e' molto variabile
 * - list_table:  piu' veloce della precedente, ma occupa una quantita' fissa di memoria
 *   (specificabile in malloc_list o predefinita in defines_typedef.h), chiamata table nel seguito.
 *   Le liste istanziate successivamente occupano la stessa table.
 *   Quando la table si riempie si ampliera' automaticamente di default, a meno ché
 *   non sia stato specificato type_resize_manual in change_resize_table.
 *   La table puo' essere ampliata o ridotta manualmente attraverso le funzioni
 *   expand_table e shrink_table (contenute in util/friend_functions.h).
 *   Queste operazioni rallentano il programma temporaneamente quindi questo tipo di
 *   lista e' particolarmente utile quando il numero di elementi e' circa noto a priori
 *   o non troppo variabile
 *
 * NOTA BENE:
 * - quando un elemento viene inserito nella lista viene creata una copia dell'elemento,
 *   per cui modificare l'elemento dato in input non modifichera'
 *   l'elemento contenuto nella lista
 */

/* type_list: tipo della lista da istanziare, descritti precedentemente */
typedef enum{
    type_list_dynamic=0,
    type_list_table  =1,
  } type_list;

/* type_resize: tipo di resize della table
 *
 * E' utile solo per le liste di tipo type_list_table.
 * Hanno i seguenti effetti:
 * - type_resize_default: la table viene ampliata automaticamente una volta piena
 * - type_resize_manual:  la table torna errore quando si cerca di inserire elementi
 *                        oltre la sua capienza. Deve essere ampliata manualmente
 *                        attraverso shrink_table
 *
 * type_resize_default e' il tipo di resize delle tabella appena create, ma puo' essere
 * cambiato attraverso la funzione change_resize_table
 * */
typedef enum{
    type_resize_default=0,
    type_resize_manual =1,
  } type_resize;

/* pcustom_print: funzione con cui stampare un elemento della lista
 * paddr:         indirizzo dell'elemento da stampare
 * size:          rappresenza rispettivamente:
 *                - type_data_generic: dimensione dell'elemento da stampare
 *                - type_data_array_*: dimensione dell'array da stampare
 *
 * E' presa in input da print_list.
 * Deve tornare 1 se stampa bene o altrimenti. */
typedef int (*pcustom_print)(pvoid paddr, unsi size);

/* pcustom_compare: funzione con cui comparare due elementi della lista
 * pvalue1:         indirizzo del primo elemento da comparare
 * size1:           rappresenza rispettivamente:
 *                  - type_data_generic: dimensione del primo elemento da comparare
 *                  - type_data_array_*: dimensione del primo array da comparare
 * pvalue2:         indirizzo del secondo elemento da comparare
 * size2:           come size1 ma del secondo elemento da comparare
 * presult:         indirizzo in cui salvare il risultato del compare, che deve essere:
 *                  0  se sono uguali
 *                  >0 se il primo è maggiore
 *                  <0 se il secondo è maggiore
 *
 * E' presa in input ad esempio da sort_list e get_max.
 * Deve tornare 1 se stampa bene o altrimenti. */
typedef int (*pcustom_compare)(pvoid pvalue1, unsi size1,
                               pvoid pvalue2, unsi size2,
                               pint  presult);

/* Sono fornite le seguenti funzioni membro: */

/* malloc_list_single_data: istanzia una nuova lista che puo' contenere dati dei tipi:
 *                          "INT", "FLOAT", "DOUBLE", "ADDRESS", "GENERIC".
 *                          Ciascun elemento della lista contiene un dato.
 * type_list:   tipo di lista da instanziare
 * type_string: stringa che descrive il tipo di dato che la lista contiene. Deve
 *              essere una delle stringhe riportate sopra.
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list_single(type_list type_list, pchar type_string);

/* malloc_list_array_data: istanzia una nuova lista che puo' contenere dati dei tipi:
 *                         "INT", "FLOAT", "DOUBLE".
 *                         Ciascun elemento della lista contiene un array di lunghezza
 *                         n_elem del dato scelto..
 * type_list:   tipo di lista da instanziare
 * type_string: stringa che descrive il tipo di dato che la lista contiene. Deve essere
 *              una delle stringhe riportate sopra
 * n_elem:      numero di elementi di ciascun array.
 *
 * return:      puntatore alla nuova lista, NULL se l'istanziamento non
 *              va a buon fine
 * */
pvoid malloc_list_array(type_list type_list, pchar type_string, unsi n_elem);

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
pvoid change_resize_table(pvoid plist, type_resize type_resize);

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list(pvoid plist);

/* get_type_list: questa funzione permette di scoprire il type_list della lista
 * plist:         lista di cui si vuole conoscere il tipo
 *
 * return:        intero corrispondente al tipo come nell'enum type_list */
unsi get_type_list(pvoid plist);

/* get_type_data: questa funzione permette di scoprire il tipo di
 *                dato contenuto nella lista
 * plist:         lista di cui si vuole conoscere il type_data
 *
 * return:        stringa che indica il tipo di dato. Puo' essere:
 *                "INT", "FLOAT", "DOUBLE",...
 *                "ARRAY_INT", "ARRAY_FLOAT", "ARRAY_DOUBLE", ...
 * */
pchar get_type_data(pvoid plist);

/* INIZIOFUNZ */
/* insert_first_TIPOBASE: inserisce un elemento di tipo TIPOBASE in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * value:        valore dell'elemento da inserire
 * size:         deve essere rispettivamente:
 *               - type_data_generic: dimensione del dato da inserire
 *               - type_data_array_*: numero di elem. dell'array da inserire
 *               - altri:             non ha importanza, si consiglia di inserire 0
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * Se la lista non contiene elementi di tipo TIPOBASE ritorna 0.
 * */
int insert_first_TIPOBASE(pvoid plist, TIPOBASE value);
/* FINEFUNZ */

/* INIZIOFUNZ */
/* insert_first_array_TIPOBASE: inserisce un array di elementi di tipo TIPOBASE in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * paddr:        indirizzo dell'array da inserire
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * Se la lista non contiene array di elementi di tipo TIPOBASE ritorna 0.
 * */
int insert_first_array_TIPOBASE(pvoid plist, pTIPOBASE paddr);
/* FINEFUNZ */

/* insert_first_generico: inserisce un elemento di tipo generico in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * pvalue:       indirizzo dell'elemento da inserire
 * size:         deve essere rispettivamente:
 *               - type_data_generic: dimensione del dato da inserire
 *               - type_data_array_*: numero di elem. dell'array da inserire
 *               - altri:             non ha importanza, si consiglia di inserire 0
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int insert_first_generic(pvoid plist, pvoid pvalue, unsi size);

/* INIZIOFUNZ */
/* extract_first_TIPOBASE: estrae l'elemento di tipo TIPOBASE in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * pvalue:        indirizzo in cui scrivere il valore dell'elemento estratto
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int extract_first_TIPOBASE(pvoid plist, pTIPOBASE pvalue);
/* FINEFUNZ */

/* INIZIOFUNZ */
/* extract_first_array_TIPOBASE: estrae l'array di elementi di tipo TIPOBASE in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * pvalue:        indirizzo in cui scrivere l'indirizzo dell'array estratto
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int extract_first_array_TIPOBASE(pvoid plist, ppTIPOBASE ppaddr);
/* FINEFUNZ */

/* extract_first: estrae l'elemento di tipo generic in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
 * ppaddr:        indirizzo in cui scrivere l'indirizzo dell'elemento estratto
 * psize:         puntatore ad unsigned int in cui verranno scritti rispettivamente:
 *                - type_data_generic: dimensione del dato estratto
 *                - type_data_array_*: numero di elem. dell'array estratto
 *                - altri:             niente, psize puo' anche essere NULL
 *
 * Torna 1 se tutto va bene, 0 altrimenti */
int extract_first_generic(pvoid plist, ppvalue ppaddr, punsi psize);

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
                 pcustom_compare pinput_compare);

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
int get_max(pvoid plist, ppvoid ppaddr_max, punsi psize_max, pcustom_compare pinput_compare);

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
int sort_list(pvoid plist, pcustom_compare pinput_compare);

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
int print_list(pvoid plist, pcustom_print pinput_print);

#endif /* ifndef _LIST */

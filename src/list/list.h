#ifndef _LIST
#define _LIST

#include "../util/defines_typedef.h"

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

/* Di seguito e' riportato un enum con tutti i tipi contenibili dall'oggetto.
 * Siccome C non permette l'overload delle funzioni, tutte prendono in input una variabile
 * di tipo all_type, in modo da poter passare diversi tipi. La variabile data in input
 * allora va castata al tipo appropriato e poi ad all_type.
 * Questo da' un warning se si compila con --pedantic, ma e' inevitabile.
 * Esempi di funzioni che prendono variabili all_type in input:
 * insert_first(plist_float, (all_type)((float)2.5), ...)
 * extract_first(plist_float, (all_type)((pvoid)&f), ...)
 * */
typedef union _all_type
         {int      i;
          long     l;
          float    f;
          double   d;
          char     c;
          void*    pv;
         } all_type;

/* Dimensione della zona di memoria da allocare all'istanziamento della prima
 * lista_veloce e in cui saranno contenute anche le successive lista_veloce */
#define TABLE_DEFAULT_DIM 1000

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
 * cambiato attraverso la funzione change_resize_table o scelto quando si crea la lista con
 * malloc_list_with_resize
 * */
typedef enum{
  type_resize_default=0,
  type_resize_manual =1,
 } type_resize;

/* pcustom_print: funzione con cui stampare un elemento della lista
 * value:         valore dell'elemento da stampare
 * size:          rappresenta rispettivamente:
 *                - dato generic: dimensione dell'elemento da stampare
 *                - dato array:   dimensione dell'array da stampare
 *                - altri dati:   non specificato
 *
 * E' presa in input da print_list.
 * Deve tornare 1 se stampa bene o altrimenti.
 *
 * NB: all_type e' la union definita prima. Per usare value come int, float, pvoid etc.
 * si usa value.i, value.f, value.pv, ...
 * */
typedef int (*pcustom_print)(all_type value, unsi size);

/* pcustom_compare: funzione con cui comparare due elementi della lista
 * value1:          rappresenta rispettivamente:
 *                  - dato array:   indirizzo del primo array da comparare
 *                  - dato generic: indirizzo del primo elemento da comparare
 *                  - altri dati:   valore del primo dato da comparare
 * size1:           rappresenta rispettivamente:
 *                  - dato generico: dimensione del primo elemento da comparare
 *                  - dato array:    dimensione del primo array da comparare
 * value2:          come value1 ma del secondo elemento da comparare
 * size2:           come size1 ma del secondo elemento da comparare
 * return:          0  se sono uguali
 *                  >0 se il primo è maggiore
 *                  <0 se il secondo è maggiore
 *
 * E' presa in input ad esempio da sort_list e get_max.
 * Deve tornare 1 se stampa bene o altrimenti.
 *
 * NB: all_type e' la union definita prima. Per usare value come int, float, pvoid etc.
 * si usa value.i, value.f, value.pv, ...
 * */
typedef int (*pcustom_compare)(all_type value1, unsi size1,
                               all_type value2, unsi size2);

/* Sono fornite le seguenti funzioni membro: */

/* malloc_list: istanzia una nuova lista che puo' contenere dati dei tipi:
 *              "CHAR", "INT", "FLOAT", "DOUBLE", "LONG", "GENERIC",
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
pvoid malloc_list(type_list type_list, pchar type_string, unsi dim_array);

/* malloc_list_specify_table: crea una nuova lista di tipo type_list_table, e specifica il tipo di
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
pvoid malloc_list_specify_table(pchar type_string, unsi dim_array, type_resize type_resize, unsi dim_table);

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
int change_resize_table(pvoid plist, type_resize type_resize);

/* resize_table: cambia il numero di elementi complessivi della tabella che contiene plist
 * n_entries:    numero di elementi complessivi della tabella, dopo che è stata ridimensionata;
 *
 * return:       1 se ridimensionata correttamente, 0 altrimenti, ad esempio se
 * n_entries è minore del numero di elementi delle liste contenute.
 */
int resize_table(pvoid plist, unsi n_entries);

/* get_info_table: fornisce informazioni sulla tabella che contiene plist
 * pn_entries: indirizzo in cui scrivere il numero di elementi complessivi della tabella;
 * pn_occupied: indirizzo in cui scrivere il numero di elementi occupati della tabella;
 *
 * return: 1 se tutto va bene, 0 altrimenti
 * */
int get_info_table(pvoid plist,
                   punsi pn_entries,
                   punsi pn_occupied);

/* free_list: libera la memoria occupata dalla lista
 * plista:    lista da liberare
 *
 * return:    non ritorna niente */
void free_list(pvoid plist);

/* get_type_list: questa funzione permette di scoprire il tipo della lista e
 *                del dato contenuto
 * plist:         lista di cui si vuole conoscere il tipo
 *
 * return:        stringa del tipo:
 *                "DINAMICA INT"
 *                "DINAMICA ARRAY INT"
 *                "TABLE INT"
 * */
pchar get_type_list(pvoid plist);

/* insert_first: inserisce un elemento in cima alla lista.
 * plist:        lista al cui inizio inserire l'elemento
 * value:        elemento da inserire
 * size:         deve essere rispettivamente:
 *               - type_data_generic: dimensione del dato da inserire
 *               - altri:             non ha importanza
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * NB: per evitare errori, value deve essere del tipo appropriato e castato a (all_type), ad esempio
 * - insert_first(mia_lista_double, (all_type)(2.4), 0)
 * - insert_first(mia_lista_generic, (all_type)((pvoid)&var_da_inserire), sizeof(var_da_inserire))
 * */
int insert_first(pvoid plist, all_type value, unsi size);

/* extract_first: estrae l'elemento in cima alla lista
 * plist:         lista dal cui inizio estrarre l'elemento
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
 * */
int extract_first(pvoid plist, all_type pvalue, punsi psize);

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
int insert_last(pvoid plista, all_type value, unsi size);

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
int extract_last(pvoid plist, all_type pvalue, punsi psize);

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
 *                 int (*pcustom_compare)(all_type value1, unsi size1, all_type value2, unsi size2, pint presult);
 *                 Deve scrivere in presult 0 se sono uguali.
 *                 La funzione compara value_searched come value1 e ciascun elemento
 *                 della lista come value2 e si ferma quando i due sono uguali secondo pinput_compare
 *
 * Torna 1 se lo ha trovato, 0 altrimenti
 * */
int search_first(pvoid plist,
                 all_type value_searched, unsi size_searched,
                 all_type pvalue_found,   punsi psize_found,
                 pcustom_compare pinput_compare);

/* get_max:        trova il massimo della lista (cioe' l'elemento che e' piu' grande di
 *                 tutti gli altri secondo pinput_compare)
 * ppaddr_max:     indirizzo in cui scrivere l'indirizzo dell'elemento massimo
 * psize_max:      indirizzo in cui scrivere rispettivamente:
 *                 - type_data_generic: dimensione dell'elemento massimo
 *                 - type_data_array_*: numero di elem. dell'array massimo
 * pinput_compare: funzione con cui comparare due elementi della lista.
 *                 E' una funzione del tipo:
 *                 int (*pcustom_compare)(all_type value1, unsi size1, all_type value2, unsi size2, pint presult);
 *                 Deve scrivere in presult:
 *                 - 0  se sono uguali
 *                 - >0 se il primo è maggiore
 *                 - <0 se il secondo è maggiore
 *
 * Torna 1 se tutto va bene, 0 altrimenti
 * */
int get_max(pvoid plist, ppvoid ppaddr_max, punsi psize_max, pcustom_compare pinput_compare);

/* sort_list:      ordina la lista secondo pinput_compare
 * plist:          lista da ordinare
 * pinput_compare: funzione con cui comparare due elementi della lista.
 *                 E' una funzione del tipo:
 *                 int (*pcustom_compare)(all_type value1, unsi size1, all_type value2, unsi size2, pint presult);
 *                 Deve scrivere in presult:
 *                 - 0  se sono uguali
 *                 - >0 se il primo è maggiore
 *                 - <0 se il secondo è maggiore
 *
 * Torna 1 se tutto va bene, 0 altrimenti
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
 *               int (*pcustom_print)(ALL_TYPE value, unsi size)
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

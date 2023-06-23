#ifndef FRIEND_FUNCTIONS
#define FRIEND_FUNCTIONS

/* create_custom_table: crea una table di dimensioni arbitrarie
 * n_entries:           numero di elementi che puo' contenere la tabella
 *                      tra tutte le sue liste
 * type_resize:         tipo di resize della lista. Puo' essere:
 *                      - type_resize_default: la lista viene ampliata automaticamente
 *                        quando piena
 *                      - type_resize_manual: la lista torna errore quando viene inserito
 *                        un elemento oltre la sua capienza. Deve essere ampliata con
 *                        expand_table
 *
 * NB: in entrambi i casi, l'unico metodo di restringemento della tabella e'
 * attraverso la funzione shrink_table
 *
 * Torna 1 se la tabella viene creata, 0 altrimenti
 *
 * Questa funzione va chiamata prima di allocare la prima lista di tipo
 * type_list_table altrimenti la tabella sara' creata con la dimensione di default
 * definita in defines_typedef.
 * La dimensione di una tabella gia' creata si puo' modificare con
 * expand_table e shrink_table
 * */
int create_custom_table(unsi n_entries, type_resize type_resize);

/* expand_table: ingrandisce la tabella gia' creata
 * n_entries:    numero di posti da aggiungere alla tabella
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * Questa funzione e' molto pesante quindi si consiglia di ingrandire la table
 * di molte entries alla volta
 * */
int expand_table(unsi n_entries);

/* shrink_table: restringe la tabella gia' creata
 * n_entries:    numero di posti da rimuovere dalla tabella
 *
 * Torna 1 se tutto va bene, 0 altrimenti.
 *
 * Questa funzione e' molto pesante quindi si consiglia di restringere la table
 * di molte entries alla volta
 * */
int shrink_table(unsi n_entries);

int pdefault_print_int(pvoid paddr, unsi size);

int pdefault_print_float(pvoid paddr, unsi size);

int pdefault_print_double(pvoid paddr, unsi size);

#endif /* FRIEND_FUNCTIONS_H */

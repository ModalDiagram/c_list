#include <stdio.h>
#include <stdlib.h>
#include "../list/list.h"

int print_sing_float(all_type val, unsi size){
  printf("%f", val.f);
  return 1;
 }

int print_array_float(all_type val, unsi size){
  int i;
  pfloat parray = (pfloat) val.pv;

  for (i = 0; i < size; i++) {
    printf("%f", parray[i]);
    printf("->");
  }
  return 1;
 }
int main(){
  char        str[256]="n", str2[256], str3[256];
  float f;
  float pf[3];
  pfloat pextract;
  unsigned size, tot, occupied;
  int      j=0,i,random,n, scelta, n_elem, pos;
  pvoid       ps, plist;
  int         s=-1;

  printf("---------------------------\n");
  printf("Studiamo i tipi di list!!\n");
  printf("---------------------------\n\n");
  printf("Quale tipo di lista vuoi istanziare? 0) type_list_dynamic; 1) type_list_table\n");
  printf("---------------------------\n\n");
  scanf("%s",str);
  scelta = atoi(str);
  printf("---------------------------\n\n");
  printf("Singolo o array (3 elementi)? 1) singolo 3) array\n");
  printf("---------------------------\n\n");
  scanf("%s",str);
  n_elem = atoi(str);
  plist = malloc_list(scelta, "FLOAT", n_elem);

  j=0;
  for(;;){
    printf("---------------------------\n\n");
    printf("Scelte:\n-i inserisci \n-e: estrai\n-o: ottieni info sulla tabella\n-r: ridimensiona tabella\n");
    printf("---------------------------\n\n");
    scanf("%s",str);
    printf("%c\n", str[0]);
    switch(str[0]){
      case 'i':
        printf("Dove vuoi inserire? 1) in testa, 2) in coda, 3) in mezzo\n");
        scanf("%s", str3);
        if(n_elem == 1){
          printf("Inserisci il valore da inserire.\n");
          scanf("%s", str2);
          f = atof(str2);
         }
        else{
          printf("Inserisci %d valori\n", n_elem);
          for (i = 0; i < 3; i++) {
            scanf("%s", str2);
            pf[i] = atof(str2);
           }
         }
        switch (str3[0]) {
          case '1':
            if(n_elem == 1){
              if((!insert_first(plist, (all_type) f, 0))) printf("Non inserito\n");
             }
            if(n_elem == 3){
              if((!insert_first(plist, (all_type) ((pvoid)pf), 0))) printf("Non inserito\n");
             }
            break;
          case '2':
            if(n_elem == 1){
              if((!insert_last(plist, (all_type) f, 0))) printf("Non inserito\n");
             }
            if(n_elem == 3){
              if((!insert_last(plist, (all_type) ((pvoid)pf), 0))) printf("Non inserito\n");
             }
            break;
          case '3':
            printf("In che posizione lo vuoi inserire?");
            scanf("%s", str2);
            pos = atoi(str2);
            if(n_elem == 1){
              if((!insert_nth(plist, (all_type) f, 0, pos))) printf("Non inserito\n");
             }
            if(n_elem == 3){
              if((!insert_nth(plist, (all_type) ((pvoid)pf), 0, pos))) printf("Non inserito\n");
             }
            break;
         }
        break;
      case 'e':
        printf("Da dove vuoi estrarre? 1) in testa, 2) in coda, 3) in mezzo\n");
        scanf("%s", str3);
        switch (str3[0]) {
          case '1':
            if(n_elem == 1){
              if((!extract_first(plist, (all_type)((pvoid)&f), 0))) printf("Non estratto\n");
              printf("Estratto valore %f\n", f);
             }
            if(n_elem == 3){
              if((!extract_first(plist, (all_type) ((pvoid)&pextract), 0))) printf("Non estratto\n");
              printf("Estratti valori %f,%f,%f\n", pf[0], pf[1], pf[2]);
             }
            break;
          case '2':
            if(n_elem == 1){
              if((!extract_last(plist, (all_type)((pvoid)&f), 0))) printf("Non estratto\n");
              printf("Estratto valore %f\n", f);
             }
            if(n_elem == 3){
              if((!extract_last(plist, (all_type) ((pvoid)&pextract), 0))) printf("Non estratto\n");
              printf("Estratti valori %f,%f,%f\n", pf[0], pf[1], pf[2]);
             }
            break;
          case '3':
            printf("Da che posizione lo vuoi estrarre?");
            scanf("%s", str2);
            pos = atoi(str2);
            if(n_elem == 1){
              if((!extract_nth(plist, (all_type)((pvoid)&f), 0, pos))) printf("Non estratto\n");
              printf("Estratto valore %f\n", f);
             }
            if(n_elem == 3){
              if((!extract_nth(plist, (all_type) ((pvoid)&pextract), 0, pos))) printf("Non estratto\n");
              printf("Estratti valori %f,%f,%f\n", pf[0], pf[1], pf[2]);
             }
            break;
         }
        break;
      case 'r':
        printf("A quanti posti vuoi portare la tabella?\nN. posti: ");
        scanf("%s", str2);
        if(!resize_table(plist, atoi(str2))) printf("Impossibile ridimensionare\n");
      case 'o':
        if(!get_info_table(plist, &tot, &occupied)) printf("Impossibile ottenere info\n");
        else printf("Posti totali %u, occupati %u\n", tot, occupied);
        break;
     }
    printf("Stampo lista->\n");
    if(n_elem == 1){
      print_list(plist, print_sing_float);
     }
    else{
      print_list(plist, print_array_float);
     }
    printf("\"\nScelta:");
    j++;
  }
}

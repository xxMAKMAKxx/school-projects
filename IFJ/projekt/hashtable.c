/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      hashtable.c
 * Datum:      1.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "hashtable.h"

void hashInit(tHashTable *table[]) {
  int i;

  for(i = 0 ; i < HASHTABLESIZE ; i++){
    table[i] = NULL;
  }
}

int hashFunction(char *str) {
  int i;
  unsigned int key = 0;

  for(i = 0; str[i]; i++) {
    key = key + tolower(str[i]) - '0';
  }

  key = key % 256;
  return key % HASHTABLESIZE;
}

void hashInsert(tHashTable *table[], char *str) {
  int i;
  int key;
  tHashTable *novy;

  key = hashFunction(str);
  novy = malloc(sizeof(tHashTable));
  if (novy == NULL) {
      fprintf(stderr, "Chyba alokace!\n");
  }
  novy->str = malloc(strlen(str)+1);
  if (novy->str == NULL) {
      fprintf(stderr, "Chyba alokace!\n");
  }
  novy->ptr = table[key];
  for(i = 0; str[i]; i++) {
    novy->str[i] = tolower(str[i]);
  }
  novy->str[i] = '\0';
  table[key] = novy;
}

int hashSearch(tHashTable *table[], char *str) {
  int j;
  int key;
  tHashTable *i;
  char new_str[128];

  for(j = 0; str[j]; j++) {
    new_str[j] = tolower(str[j]);
  }
  new_str[j] = '\0';

  key = hashFunction(str);
  for(i = table[key] ; i != NULL ; i = i->ptr) {
    if(strcmp(i->str, new_str) == 0) {
      return 1;
    }
  }
  return 0;
}

void hashDispose(tHashTable *table[]) {
  int i;
  tHashTable *prvek;

  for(i = 0 ; i < HASHTABLESIZE ; i++){
    while(table[i] != NULL) {
      prvek = table[i]->ptr;
      free(table[i]->str);
      free(table[i]);
      table[i] = prvek;
    }
  }
}


/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      ValueTable.c
 * Datum:      5.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "int_valuetable.h"

void ValTableInit(tValTable *table) {
  int i;

  for(i = 0 ; i < VALTABLESIZE ; i++){
    table->Table[i] = NULL;
  }
  table->pocet_promennych = 0;
}

int hashFunction2(char *str) {
  int i;
  unsigned int key = 0;

  for(i = 0; str[i]; i++) {
    key = key + tolower(str[i]) - '0';
  }

  key = key % 256;
  return key % VALTABLESIZE;
}

void ValTableInsert(tValTable *table, char *str, char type) {
    int i;
    int key;
    tValTableElem *novy;
    if(str != NULL) {
        key = hashFunction2(str);
        novy = malloc(sizeof(tValTableElem));
        if (novy == NULL) {
            printf("CHYBA ALOKACE");
        }
        novy->str = malloc(strlen(str)+1);
        if (novy->str == NULL) {
            printf("CHYBA ALOKACE");
        }

        novy->ptr = table->Table[key];

        for(i = 0; str[i]; i++) {
            novy->str[i] = str[i];
        }
        novy->str[i] = '\0';
        novy->type = type;
        novy->index = table->pocet_promennych;
        table->pocet_promennych = table->pocet_promennych + 1;
        table->Table[key] = novy;
    }
}

tValTableElem *ValTableInsertFunc(tValTable *table, char *str, char type) {
    int i;
    int key;
    tValTableElem *novy;
    if(str != NULL) {
        key = hashFunction2(str);
        novy = malloc(sizeof(tValTableElem));
        if (novy == NULL) {
            printf("CHYBA ALOKACE");
        }
        novy->str = malloc(strlen(str)+1);
        if (novy->str == NULL) {
            printf("CHYBA ALOKACE");
        }

        novy->ptr = table->Table[key];

        for(i = 0; str[i]; i++) {
            novy->str[i] = str[i];
        }
        novy->str[i] = '\0';
        novy->type = type;
        table->Table[key] = novy;
        return novy;
    }
    return NULL;
}

tValTableElem *ValTableSearch(tValTable *table, char *str) {
  int j;
  int key;
  tValTableElem *i;
  char new_str[128];

  for(j = 0; str[j]; j++) {
    new_str[j] = tolower(str[j]);
  }
  new_str[j] = '\0';

  key = hashFunction2(str);
  for(i = table->Table[key] ; i != NULL ; i = i->ptr) {
    if(strcmp(i->str, new_str) == 0) {
      return i;
    }
  }
  return NULL;
}

void ValTableDispose(tValTable *table) {
    int i;
    tValTableElem *prvek;

    for(i = 0 ; i < VALTABLESIZE ; i++){
        while(table->Table[i] != NULL) {
            prvek = table->Table[i]->ptr;
            free(table->Table[i]->str);
            free(table->Table[i]);
            table->Table[i] = prvek;
        }
    }
}

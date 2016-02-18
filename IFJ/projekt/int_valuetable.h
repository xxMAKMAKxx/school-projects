/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      ValueTable.h
 * Datum:      5.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef VALUETABLE_H
#define VALUETABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "instruction_list.h"

#define VALTABLESIZE 16

#define TYPEINT 'i'
#define TYPEREAL 'r'
#define TYPEBOOL 'b'
#define TYPESTRING 's'
#define TYPEFUNC 'f'

typedef struct sValTableElem{
    struct sValTableElem *ptr;
    char *str;
    int index;

    char type;

    char f_params[128];
    int f_pocet_lok_promennych;
    void **f_zacatek; //ukazatel na ukazatel na instrukci

} tValTableElem;

typedef struct sValTable{
    tValTableElem *Table[VALTABLESIZE];
    int pocet_promennych;
} tValTable;

void ValTableInit(tValTable *);
void ValTableDispose(tValTable *);

void ValTableInsert(tValTable *, char *, char);
tValTableElem *ValTableInsertFunc(tValTable *, char *, char);
tValTableElem *ValTableSearch(tValTable *, char *);

void ValTableDisposeAll(tValTable *);
#endif

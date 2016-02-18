/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      hashtable.h
 * Datum:      1.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHTABLESIZE 128


typedef struct sHashTable{
    struct sHashTable *ptr;
    char *str;
} tHashTable;

void hashInit(tHashTable **);
void hashDispose(tHashTable **);

int hashFunction(char *);

void hashInsert(tHashTable **, char *);
int hashSearch(tHashTable **, char *);

#endif

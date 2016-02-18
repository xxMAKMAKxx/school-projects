
/* *********************Tabulka symbolu********************************
 * Subor:      ial.c
 * Datum:      19.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef IAL_H
#define IAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_HTSIZE 101

//Zde budou deklarace funkci hash tabulky
typedef char* tKey;
typedef struct tHTItem tHTItem;
typedef tHTItem* tHTable[MAX_HTSIZE];

 typedef struct tData{
	int wasWritten;
	char *type;
	char *value;
	tHTable* localTable;
} tData;

struct tHTItem{
	tKey key;				/* klíè  */
	tData data;				/* obsah */
	struct tHTItem* ptrnext;	/* ukazatel na dal¹í synonymum */
};
/* TRP s explicitnì zøetìzenými synonymy. */

/* Hlavièky øe¹ených procedur a funkcí. */

int hashCode ( tKey key );

void htInit ( tHTable* ptrht );

tHTItem* htSearch ( tHTable* ptrht, tKey key );

void htInsert ( tHTable* ptrht, tKey key, tData data );

tData* htRead ( tHTable* ptrht, tKey key );

void htDelete ( tHTable* ptrht, tKey key );

void htClearAll ( tHTable* ptrht );

//Konec deklarace funkci Hash tabulky


void BoyerMoorAlogythm(char *, char *, int *);
void QuickSort(char *, int, int);


#endif

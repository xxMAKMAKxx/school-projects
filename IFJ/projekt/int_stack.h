/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      Stack.h
 * Datum:      5.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "instruction_list.h"


#define TYPEINT 'i'
#define TYPEREAL 'r'
#define TYPEBOOL 'b'
#define TYPESTRING 's'
#define TYPEFUNC 'f'
#define INSADR 'a'
#define ZANORENIBEGIN 'z'

typedef struct sStackElemI{
    struct sStackElemI *prev;
    void *value;
    char type;
} tStackElemI;

typedef struct sStackI{
    tStackElemI *top;
} tStackI;

void StackInitI(tStackI *);
void StackDisposeI(tStackI *);


void StackPushI(tStackI *, char, void *);
void StackTopI(tStackI *, char *, void **);
void StackTopI2(tStackI *, char *, void **);
void StackPopI(tStackI *);
#endif

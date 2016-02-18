/* *********************LEXIKALNY ANALYZATOR****************************
 * Subor:      scanner.c
 * Datum:      12.10.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    S_START = 0, // 0pociatocny stav
    S_FALSE,     // 1mrtvy stavs
    S_CMNT,      // 2komentar {....}
    S_ID,        // 3identifikator
    S_INT,       // 4cele cislo
    S_REAL1,     // 5realne cislo 13.28
    S_REAL2,     // 6realne cislo 13.28E-45
    S_NUMHELP1,  // 7 12.
    S_NUMHELP2,  // 8 12.E
    S_NUMHELP3,  // 9 12.E-
    S_STR,       // 10retazec
    S_STRHELP1,  // 11
    S_STRHELP2,  //12
    S_STRHELP3,  //13
    S_PER,       // .14
    S_COM,       // ,15
    S_SEM,       // ;16
    S_LPAR,      // (17
    S_RPAR,      // )18
    S_ADD,       // +19
    S_SUB,       // -20
    S_MUL,       // *21
    S_DIV,       // /22
    S_EQ ,       // =23
    S_COL ,      // :24
    S_ASGN,      // :=25
    S_LS,        // <26
    S_LSEQ,      // <=27
    S_NOTEQ,     // <>28
    S_GR,        // >29
    S_GREQ,      // >=30
    S_EOF,       // EOF31
} tState;

typedef struct {
    tState state;
    char *name;
} tToken;

tToken getNextToken(FILE*);
void init();
void dataToToken(tState, char*);
void addChar(char);
void addChar2(char);
void allocScanner();
void freeScanner();

#endif

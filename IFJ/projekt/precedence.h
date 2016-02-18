/* ************************PRECEDENCNA ANALYZA*************************
 * Subor:      precedence.h
 * Datum:      12.10.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/

#ifndef PRECEDENCE_H
#define PRECEDENCE_H
#define STACK_MAX 100

#include <stdio.h>
#include "scanner.h"

typedef enum {
    PSMul=0,        // 0 *
    PSDiv,          // 1 /
    PSAdd,          // 2 +
    PSSub,          // 3 -
    PSLess,         // 4 <
    PSGreat,        // 5 >
    PSLessEqual,    // 6 <=
    PSGreatEqual,   // 7 >=
    PSNotEqual,     // 8 <>
    PSEqual,        // 9 =
    PSParenthL,     // 10 (
    PSParenthR,     // 11 )
    PSID,           // 12 i
    PSBeginEOL,     // 13 $
    PSHandle,       // 14 < v zasobniku
    PSNonTerm,      // 15 E
}PSTerm;

typedef enum{
    PTEqual =0, // 0 =
    PTLess,     // 1 <
    PTGreat,    // 2 >
    PTEmpty,    // 3 prazdna bunka
    PTEnd,      // 4 $$
}PTType;

typedef enum{
    H_INT = 0,  // 0 integer
    H_REAL,     // 1 real
    H_STRING,   // 2 string
    H_BOOL,     // 3 boolean
    H_ID,       // 4 identifikator
    H_FALSE,    // 5 ine
}HashType;

typedef struct{
    char *Ins_string;
    int Ins_int;
    int Ins_bool;
    float Ins_float;
    char *Ins_name;
} tIns_var;

typedef struct tStackElem {
    PSTerm type;
    char *IDdata;
    HashType IDtype;
    HashType SEMtype;
    struct tStackElem *lptr;
    struct tStackElem *rptr;
} *tStackElemPtr;

typedef struct{
    tStackElemPtr First;
    tStackElemPtr Top;
}tStack;

void StackDispose(tStack *stack);
void StackPop(tStack *stack);
int StackInit(tStack *stack);
int StackPush(tStack *stack, tToken token);
int StackShift(tStack *stack,tToken token);
int StackPushE(tStack *stack);
int StackPushIDtoE(tStack *stack);
tStackElemPtr StackTopTerm(tStack *stack);

void TokenToTerm(tToken token,PSTerm *term,char **data, HashType *Htype, int control);
int prec_analysis();
void generate_3ac(tStack *stack);
int mulRule(tStack *stack);
int divRule(tStack *stack);
int addRule(tStack *stack);
int subRule(tStack *stack);
int compare1Rule(tStack *stack);
int compare2Rule(tStack *stack);
int parenthRule(tStack *stack);
int IDtoE_Rule(tStack *stack);

#endif

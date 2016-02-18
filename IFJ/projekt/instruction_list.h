/* ***********************Seznam instrukci******************************
 * Subor:      instruction_list.h
 * Datum:      10.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef INSTRUCTIONLIST_H
#define INSTRUCTIONLIST_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum {
    I_INT = 0,  //0id - kazda deklarace ve var sekci
    I_REAL,     //id - kazda deklarace ve var sekci
    I_STRING,   //id - kazda deklarace ve var sekci
    I_BOOL,     //id - kazda deklarace ve var sekci
    I_FUNC_DEK, //name = jmeno, s = navratovy_typ,promenna:typ,promenna:typ\0   i,a:i,b:i,c:f\0
    I_FUNC_ZAC, //5name = jmeno, s = navratovy_typ,promenna:typ,promenna:typ\0   i,a:i,b:i,c:f\0
    I_BEGIN,    //kazdy begin
    I_END,      //kazdy end
    I_PUSH,     //id nebo hodnota
    I_POP,      //id, do id nahraje vrsek zasobniku
    I_CALL,     //10id funkce, return hodnota bude na zasobniku
    I_WRITE,    //parametr i = pocet parametru na zasobniku
    I_READ,     //id do ktereho ulozi hodnotu
    I_COND,     //zacatek podminky(pred if a while)
    I_IF,       //zacatek if (podminka(true/false)) bude na vrcholu zasobniku
    I_ELSE,     //15else sekce
    I_WHILE,    //zacatek while, podminka zase na zasobniku
    I_ADD,      // +    aritmeticke a porovnavaci operace pracuji s vrcholem zásobníku
    I_SUB,      // -
    I_MUL,      // *
    I_DIV,      //20 /
    I_EQ ,      // =
    I_LS,       // <
    I_LSEQ,     // <=
    I_NOTEQ,    // <>
    I_GR,       //25 >
    I_GREQ      // >=
} tInstcruction;
/* Poznamka:    parametry u funkce f(a,b,c,d) pushovat na zasobnik v poradi:
                                            push d
                                            push c
                                            push b
                                            push a
                pocitani vyrazu previst na postfix: id := (a+b)*(c-d)
                                            ab+cd-*
                                            push a
                                            push b
                                            add
                                            push c
                                            push d
                                            sub
                                            mul
                                            pop id
*/


typedef struct tInsElem {
    struct tInsElem *ptr;
    tInstcruction ins;
    char *s;
    int *i;
    int *b;
    float *r;
    char *name;
} *tInsElemPtr;


typedef struct {
    tInsElemPtr First;
    tInsElemPtr Last;
} tIList;


void IListInit (tIList *);
void IListDispose (tIList *);
void IListInsert (tIList *, tInstcruction, int *, float *, int *, char *, char *);
void IListUpdateLast (tIList *, tInstcruction);
void NextInstruction (tInsElemPtr *);
tInsElemPtr FirstInstruction (tIList *);
#endif

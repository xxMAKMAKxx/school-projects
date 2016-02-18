/* ***********************Seznam instrukci******************************
 * Subor:      instruction_list.c
 * Datum:      10.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "instruction_list.h"

void Error() {
    fprintf (stderr, "*ERROR* The program has performed an illegal operation.\n");
}

void IListInit (tIList *L) {
    L->First = NULL;
    L->Last = NULL;
}

void IListDispose (tIList *L) {
    tInsElemPtr X;
    //rusi postupne vsechny prvni prvky
    while (L->First != NULL) {
        X = L->First->ptr;
        if(L->First->i != NULL) free(L->First->i);
        if(L->First->r != NULL) free(L->First->r);
        if(L->First->b != NULL) free(L->First->b);
        if(L->First->s != NULL) free(L->First->s);
        if(L->First->name != NULL) free(L->First->name);
        free(L->First);
        L->First = X;
    }
    //po zruseni vsech prvku, nastavi ukazatele na null
    L->First = NULL;
    L->Last = NULL;
}

void IListInsert (tIList *L,tInstcruction ins, int *i, float *r, int *b, char *s, char *name) {
    int s_size = 0;
    int name_size = 0;
    tInsElemPtr X = malloc(sizeof(struct tInsElem));

    if(s != NULL) s_size = strlen(s) + 1;
    if(name != NULL) name_size = strlen(name) + 1;

    X->i = NULL;
    X->r =  NULL;
    X->s =  NULL;
    X->b =  NULL;
    X->name =  NULL;

    if(i != NULL) X->i = malloc(sizeof(int) * (i != NULL));
    if(r != NULL) X->r = malloc(sizeof(float) * (r != NULL));
    if(b != NULL) X->b = malloc(sizeof(int) * (b != NULL));
    if(s != NULL) X->s = malloc(s_size*sizeof(char)*(s != NULL));
    if(name != NULL) X->name = malloc(name_size*sizeof(char)*(name != NULL));

    X->ptr = NULL;
    X->ins = ins;

    if(i != NULL) *(X->i) = *i;
    if(r != NULL) *(X->r) = *r;
    if(b != NULL) *(X->b) = *b;
    if(s != NULL) strcpy(X->s, s);
    if(name != NULL) strcpy(X->name, name);

    if(L->First == NULL) {
        L->First = X;
        L->Last = X;
    } else {
        L->Last->ptr = X;
        L->Last = X;
    }
}

void IListUpdateLast (tIList *list, tInstcruction i) {
    list->Last->ins = i;
}

void NextInstruction (tInsElemPtr *I) {
    if (*I != NULL) {
        *I = (*I)->ptr;
    }
}

tInsElemPtr FirstInstruction (tIList *list) {
    if (list != NULL) {
        return list->First;
    }
    return NULL;
}

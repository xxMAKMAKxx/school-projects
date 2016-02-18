
/* ******************HASHOVACI TABULKA SYMBOLU**************************
 * Subor:      Stack.c
 * Datum:      5.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "int_stack.h"

void StackInitI(tStackI *stack) {
    stack->top = NULL;
}
void StackDisposeI(tStackI *stack) {
    tStackElemI *i;

    while(stack->top != NULL) {
        i = stack->top->prev;
        free(stack->top->value);
        free(stack->top);
        stack->top = i;
    }
}


void StackPushI(tStackI *stack, char type, void *value) {
    tStackElemI *prvek;
    prvek = malloc(sizeof(struct sStackElemI));
    prvek->prev = stack->top;
    stack->top = prvek;
    prvek->type = type;
    switch (type) {
    case TYPEINT:
        prvek->value = malloc(sizeof(int));
        *(int*)prvek->value = *(int*)value;
        break;
    case TYPEREAL:
        prvek->value = malloc(sizeof(float));
        *(float*)prvek->value = *(float*)value;
        break;
    case TYPEBOOL:
        prvek->value = malloc(sizeof(int));
        *(int*)prvek->value = *(int*)value;
        break;
    case TYPESTRING:
        prvek->value = malloc(sizeof(char)*strlen((char*)value) + 1);
        strcpy((char*)prvek->value, (char*)value);
        break;
    case TYPEFUNC:
        prvek->value = malloc(sizeof(char)*strlen((char*)value) + 1);
        strcpy((char*)prvek->value, (char*)value);
        break;
    case INSADR:
        prvek->value = malloc(sizeof(tInsElemPtr));
        *(tInsElemPtr*)prvek->value = value;
        break;
    case ZANORENIBEGIN:
        prvek->value = malloc(sizeof(int));
        *(int*)prvek->value = *(int*)value;
        break;
    default:
        break;
    }
}

void StackTopI(tStackI *stack, char *type, void **value) {
    tStackElemI *prvek;
    prvek = stack->top;
    if(prvek != NULL) {
        *value = prvek->value;
        *type = prvek->type;
    } else {
        *value = NULL;
        *type = 0;
    }
}

void StackTopI2(tStackI *stack, char *type, void **value) {
    tStackElemI *prvek;
    prvek = stack->top;
    if(prvek != NULL) {
        prvek = prvek->prev;
        if(prvek != NULL) {
            *value = prvek->value;
            *type = prvek->type;
        } else {
            *value = NULL;
            *type = 0;
        }
    } else {
        *value = NULL;
        *type = 0;
    }
}

void StackPopI(tStackI *stack) {
    tStackElemI *prvek;
    if(stack->top != NULL) {
        prvek = stack->top;
        stack->top = stack->top->prev;
        free(prvek->value);
        free(prvek);
    }
}

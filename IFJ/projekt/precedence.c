/* ************************PRECEDENCNA ANALYZA*************************
 * Subor:      precedence.c
 * Datum:      12.10.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/

#include "precedence.h"
#include "parser.h"
#include "scanner.h"
#include "keywords.h"
#include "instruction_list.h"

extern tError errCode;
extern tToken currToken;
extern tToken prevToken;
extern FILE *file;
extern tIList Instr_list;
extern int inFunction;
extern tHTable* globalTable;
extern tHTable* globalLocalTable;
tHTItem* isStored;
tIns_var Instr_var;
tStack stack;
HashType semIDtype;

/*Precedencna tabulka*/
const int pTable [14][14] = {
    // vyplnenie tabulky pre *
    [PSMul][PSMul]               = PTGreat,
    [PSMul][PSDiv]               = PTGreat,
    [PSMul][PSAdd]               = PTGreat,
    [PSMul][PSSub]               = PTGreat,
    [PSMul][PSLess]              = PTGreat,
    [PSMul][PSGreat]             = PTGreat,
    [PSMul][PSLessEqual]         = PTGreat,
    [PSMul][PSGreatEqual]        = PTGreat,
    [PSMul][PSNotEqual]          = PTGreat,
    [PSMul][PSEqual]             = PTGreat,
    [PSMul][PSParenthL]          = PTLess,
    [PSMul][PSParenthR]          = PTGreat,
    [PSMul][PSID]                = PTLess,
    [PSMul][PSBeginEOL]          = PTGreat,
    // vyplnenie tabulky pre /
    [PSDiv][PSMul]               = PTGreat,
    [PSDiv][PSDiv]               = PTGreat,
    [PSDiv][PSAdd]               = PTGreat,
    [PSDiv][PSSub]               = PTGreat,
    [PSDiv][PSLess]              = PTGreat,
    [PSDiv][PSGreat]             = PTGreat,
    [PSDiv][PSLessEqual]         = PTGreat,
    [PSDiv][PSGreatEqual]        = PTGreat,
    [PSDiv][PSNotEqual]          = PTGreat,
    [PSDiv][PSEqual]             = PTGreat,
    [PSDiv][PSParenthL]          = PTLess,
    [PSDiv][PSParenthR]          = PTGreat,
    [PSDiv][PSID]                = PTLess,
    [PSDiv][PSBeginEOL]          = PTGreat,
    // vyplnenie tabulky pre +
    [PSAdd][PSMul]               = PTLess,
    [PSAdd][PSDiv]               = PTLess,
    [PSAdd][PSAdd]               = PTGreat,
    [PSAdd][PSSub]               = PTGreat,
    [PSAdd][PSLess]              = PTGreat,
    [PSAdd][PSGreat]             = PTGreat,
    [PSAdd][PSLessEqual]         = PTGreat,
    [PSAdd][PSGreatEqual]        = PTGreat,
    [PSAdd][PSNotEqual]          = PTGreat,
    [PSAdd][PSEqual]             = PTGreat,
    [PSAdd][PSParenthL]          = PTLess,
    [PSAdd][PSParenthR]          = PTGreat,
    [PSAdd][PSID]                = PTLess,
    [PSAdd][PSBeginEOL]          = PTGreat,
    // vyplnenie tabulky pre -
    [PSSub][PSMul]               = PTLess,
    [PSSub][PSDiv]               = PTLess,
    [PSSub][PSAdd]               = PTGreat,
    [PSSub][PSSub]               = PTGreat,
    [PSSub][PSLess]              = PTGreat,
    [PSSub][PSGreat]             = PTGreat,
    [PSSub][PSLessEqual]         = PTGreat,
    [PSSub][PSGreatEqual]        = PTGreat,
    [PSSub][PSNotEqual]          = PTGreat,
    [PSSub][PSEqual]             = PTGreat,
    [PSSub][PSParenthL]          = PTLess,
    [PSSub][PSParenthR]          = PTGreat,
    [PSSub][PSID]                = PTLess,
    [PSSub][PSBeginEOL]          = PTGreat,
    // vyplnenie tabulky pre <
    [PSLess][PSMul]              = PTLess,
    [PSLess][PSDiv]              = PTLess,
    [PSLess][PSAdd]              = PTLess,
    [PSLess][PSSub]              = PTLess,
    [PSLess][PSLess]             = PTGreat,
    [PSLess][PSGreat]            = PTGreat,
    [PSLess][PSLessEqual]        = PTGreat,
    [PSLess][PSGreatEqual]       = PTGreat,
    [PSLess][PSNotEqual]         = PTGreat,
    [PSLess][PSEqual]            = PTGreat,
    [PSLess][PSParenthL]         = PTLess,
    [PSLess][PSParenthR]         = PTGreat,
    [PSLess][PSID]               = PTLess,
    [PSLess][PSBeginEOL]         = PTGreat,
    // vyplnenie tabulky pre >
    [PSGreat][PSMul]             = PTLess,
    [PSGreat][PSDiv]             = PTLess,
    [PSGreat][PSAdd]             = PTLess,
    [PSGreat][PSSub]             = PTLess,
    [PSGreat][PSLess]            = PTGreat,
    [PSGreat][PSGreat]           = PTGreat,
    [PSGreat][PSLessEqual]       = PTGreat,
    [PSGreat][PSGreatEqual]      = PTGreat,
    [PSGreat][PSNotEqual]        = PTGreat,
    [PSGreat][PSEqual]           = PTGreat,
    [PSGreat][PSParenthL]        = PTLess,
    [PSGreat][PSParenthR]        = PTGreat,
    [PSGreat][PSID]              = PTLess,
    [PSGreat][PSBeginEOL]        = PTGreat,
    // vyplnenie tabulky pre <=
    [PSLessEqual][PSMul]         = PTLess,
    [PSLessEqual][PSDiv]         = PTLess,
    [PSLessEqual][PSAdd]         = PTLess,
    [PSLessEqual][PSSub]         = PTLess,
    [PSLessEqual][PSLess]        = PTGreat,
    [PSLessEqual][PSGreat]       = PTGreat,
    [PSLessEqual][PSLessEqual]   = PTGreat,
    [PSLessEqual][PSGreatEqual]  = PTGreat,
    [PSLessEqual][PSNotEqual]    = PTGreat,
    [PSLessEqual][PSEqual]       = PTGreat,
    [PSLessEqual][PSParenthL]    = PTLess,
    [PSLessEqual][PSParenthR]    = PTGreat,
    [PSLessEqual][PSID]          = PTLess,
    [PSLessEqual][PSBeginEOL]    = PTGreat,
    // vyplnenie tabulky pre >=
    [PSGreatEqual][PSMul]        = PTLess,
    [PSGreatEqual][PSDiv]        = PTLess,
    [PSGreatEqual][PSAdd]        = PTLess,
    [PSGreatEqual][PSSub]        = PTLess,
    [PSGreatEqual][PSLess]       = PTGreat,
    [PSGreatEqual][PSGreat]      = PTGreat,
    [PSGreatEqual][PSLessEqual]  = PTGreat,
    [PSGreatEqual][PSGreatEqual] = PTGreat,
    [PSGreatEqual][PSNotEqual]   = PTGreat,
    [PSGreatEqual][PSEqual]      = PTGreat,
    [PSGreatEqual][PSParenthL]   = PTLess,
    [PSGreatEqual][PSParenthR]   = PTGreat,
    [PSGreatEqual][PSID]         = PTLess,
    [PSGreatEqual][PSBeginEOL]   = PTGreat,
    // vyplnenie tabulky pre <>
    [PSNotEqual][PSMul]          = PTLess,
    [PSNotEqual][PSDiv]          = PTLess,
    [PSNotEqual][PSAdd]          = PTLess,
    [PSNotEqual][PSSub]          = PTLess,
    [PSNotEqual][PSLess]         = PTGreat,
    [PSNotEqual][PSGreat]        = PTGreat,
    [PSNotEqual][PSLessEqual]    = PTGreat,
    [PSNotEqual][PSGreatEqual]   = PTGreat,
    [PSNotEqual][PSNotEqual]     = PTGreat,
    [PSNotEqual][PSEqual]        = PTGreat,
    [PSNotEqual][PSParenthL]     = PTLess,
    [PSNotEqual][PSParenthR]     = PTGreat,
    [PSNotEqual][PSID]           = PTLess,
    [PSNotEqual][PSBeginEOL]     = PTGreat,
    // vyplnenie tabulky pre =
    [PSEqual][PSMul]             = PTLess,
    [PSEqual][PSDiv]             = PTLess,
    [PSEqual][PSAdd]             = PTLess,
    [PSEqual][PSSub]             = PTLess,
    [PSEqual][PSLess]            = PTGreat,
    [PSEqual][PSGreat]           = PTGreat,
    [PSEqual][PSLessEqual]       = PTGreat,
    [PSEqual][PSGreatEqual]      = PTGreat,
    [PSEqual][PSNotEqual]        = PTGreat,
    [PSEqual][PSEqual]           = PTGreat,
    [PSEqual][PSParenthL]        = PTLess,
    [PSEqual][PSParenthR]        = PTGreat,
    [PSEqual][PSID]              = PTLess,
    [PSEqual][PSBeginEOL]        = PTGreat,
    // vyplnenie tabulky pre (
    [PSParenthL][PSMul]          = PTLess,
    [PSParenthL][PSDiv]          = PTLess,
    [PSParenthL][PSAdd]          = PTLess,
    [PSParenthL][PSSub]          = PTLess,
    [PSParenthL][PSLess]         = PTLess,
    [PSParenthL][PSGreat]        = PTLess,
    [PSParenthL][PSLessEqual]    = PTLess,
    [PSParenthL][PSGreatEqual]   = PTLess,
    [PSParenthL][PSNotEqual]     = PTLess,
    [PSParenthL][PSEqual]        = PTLess,
    [PSParenthL][PSParenthL]     = PTLess,
    [PSParenthL][PSParenthR]     = PTEqual,
    [PSParenthL][PSID]           = PTLess,
    [PSParenthL][PSBeginEOL]     = PTEmpty,
    // vyplnenie tabulky pre )
    [PSParenthR][PSMul]          = PTGreat,
    [PSParenthR][PSDiv]          = PTGreat,
    [PSParenthR][PSAdd]          = PTGreat,
    [PSParenthR][PSSub]          = PTGreat,
    [PSParenthR][PSLess]         = PTGreat,
    [PSParenthR][PSGreat]        = PTGreat,
    [PSParenthR][PSLessEqual]    = PTGreat,
    [PSParenthR][PSGreatEqual]   = PTGreat,
    [PSParenthR][PSNotEqual]     = PTGreat,
    [PSParenthR][PSEqual]        = PTGreat,
    [PSParenthR][PSParenthL]     = PTEmpty,
    [PSParenthR][PSParenthR]     = PTGreat,
    [PSParenthR][PSID]           = PTEmpty,
    [PSParenthR][PSBeginEOL]     = PTGreat,
    // vyplnenie tabulky pre i
    [PSID][PSMul]                = PTGreat,
    [PSID][PSDiv]                = PTGreat,
    [PSID][PSAdd]                = PTGreat,
    [PSID][PSSub]                = PTGreat,
    [PSID][PSLess]               = PTGreat,
    [PSID][PSGreat]              = PTGreat,
    [PSID][PSLessEqual]          = PTGreat,
    [PSID][PSGreatEqual]         = PTGreat,
    [PSID][PSNotEqual]           = PTGreat,
    [PSID][PSEqual]              = PTGreat,
    [PSID][PSParenthL]           = PTEmpty,
    [PSID][PSParenthR]           = PTGreat,
    [PSID][PSID]                 = PTEmpty,
    [PSID][PSBeginEOL]           = PTGreat,
    // vyplnenie tabulky pre $
    [PSBeginEOL][PSMul]          = PTLess,
    [PSBeginEOL][PSDiv]          = PTLess,
    [PSBeginEOL][PSAdd]          = PTLess,
    [PSBeginEOL][PSSub]          = PTLess,
    [PSBeginEOL][PSLess]         = PTLess,
    [PSBeginEOL][PSGreat]        = PTLess,
    [PSBeginEOL][PSLessEqual]    = PTLess,
    [PSBeginEOL][PSGreatEqual]   = PTLess,
    [PSBeginEOL][PSNotEqual]     = PTLess,
    [PSBeginEOL][PSEqual]        = PTLess,
    [PSBeginEOL][PSParenthL]     = PTLess,
    [PSBeginEOL][PSParenthR]     = PTEmpty,
    [PSBeginEOL][PSID]           = PTLess,
    [PSBeginEOL][PSBeginEOL]     = PTEnd,
};

/*Funkcia pre inicializacia zasobniku*/
int StackInit(tStack* stack) {
    if (stack == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }

    stack->Top = NULL;
    stack->First = NULL;

    tStackElemPtr TmpPtr1 = NULL;
    TmpPtr1 = malloc(sizeof(struct tStackElem));
    if (TmpPtr1 != NULL) {
        TmpPtr1->type = PSBeginEOL;
        TmpPtr1->IDdata = NULL;
        TmpPtr1->IDtype = H_FALSE;
        TmpPtr1->SEMtype = H_FALSE;
        TmpPtr1->lptr = NULL;
        TmpPtr1->rptr = NULL;
        stack->First = TmpPtr1;
        stack->Top = TmpPtr1;
    }
    else {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Procedura pre rusenie zasobniku*/
void StackDispose(tStack *stack) {
    int sizeofname = 0;
    tStackElemPtr TmpPtr2 = NULL;
    TmpPtr2 = stack->Top;
    if((stack->Top->type == PSNonTerm)&&(stack->Top->IDtype != H_FALSE)){     /*ak bol vyraz zlozeny len z jednej premennej*/
        if (stack->Top->SEMtype == H_INT){
            Instr_var.Ins_int = atoi(stack->Top->IDdata);
            semIDtype = stack->Top->SEMtype;
            IListInsert (&Instr_list,I_PUSH,&Instr_var.Ins_int,NULL,NULL,NULL,NULL);
        }
        if (stack->Top->SEMtype == H_REAL){
            Instr_var.Ins_float = atof(stack->Top->IDdata);
            semIDtype = stack->Top->SEMtype;
            IListInsert (&Instr_list,I_PUSH,NULL,&Instr_var.Ins_float,NULL,NULL,NULL);
        }
        if (stack->Top->SEMtype == H_STRING){
            sizeofname = strlen(stack->Top->IDdata);
            if ( (Instr_var.Ins_string = malloc(sizeofname+1)) == NULL){
                if(errCode == 0){
                    errCode = E_ERR;
                }
            }
            strcpy(Instr_var.Ins_string, stack->Top->IDdata);
            semIDtype = stack->Top->SEMtype;
            IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,Instr_var.Ins_string,NULL);
            free(Instr_var.Ins_string);
        }
        if (stack->Top->SEMtype == H_BOOL){
            if (strcmp(stack->Top->IDdata, "true") == 0){
                Instr_var.Ins_bool = 1;
            }
            else if (strcmp(stack->Top->IDdata, "false") == 0){
                Instr_var.Ins_bool = 0;
            }
            semIDtype = stack->Top->SEMtype;
            IListInsert (&Instr_list,I_PUSH,NULL,NULL,&Instr_var.Ins_bool,NULL,NULL);
        }
        if (stack->Top->SEMtype == H_ID){        /*vyhladanie identifikatoru v tabulke symbolov*/
            if(inFunction == 0){
                isStored = htSearch(globalTable,stack->Top->IDdata);
                if(isStored == NULL){
                    if(errCode == 0){
                        errCode = E_SEMP;
                        return;
                    }
                }
            }
            else{
                if(inFunction == 1){
                    isStored = htSearch(globalLocalTable,stack->Top->IDdata);
                    if(isStored == NULL){
                        isStored = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                }
            }
            if(isStored->data.type == NULL){
                if(errCode == 0){
                    errCode = E_SYN;
                    return;
                }
            }
            if (strcmp(isStored->data.type, "integer") == 0){
                semIDtype = H_INT;
            }
            else{
                if (strcmp(isStored->data.type, "real") == 0){
                    semIDtype = H_REAL;
                }
                else{
                    if (strcmp(isStored->data.type, "string") == 0){
                        semIDtype = H_STRING;
                    }
                    else{
                        if (strcmp(isStored->data.type, "boolean") == 0){
                            semIDtype = H_BOOL;
                        }
                        else{
                            if(errCode == 0){
                                errCode = E_SEMF;
                            }
                        }
                    }
                }
            }
            sizeofname = strlen(stack->Top->IDdata);
            if ((Instr_var.Ins_name = malloc(sizeofname+1)) == NULL){
                if(errCode == 0){
                    errCode = E_ERR;
                }
            }
            strcpy(Instr_var.Ins_name, stack->Top->IDdata);
            IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,NULL,Instr_var.Ins_name);
            free(Instr_var.Ins_name);
        }
    }

    while (stack->Top != NULL) {
        if(stack->Top->IDdata != NULL){
            free (stack->Top->IDdata);
        }
        TmpPtr2 = stack->Top;
        stack->Top = stack->Top->lptr;
        free (TmpPtr2);
    }
    stack->Top = NULL;
    stack->First = NULL;
    prevToken.state = S_FALSE;
    prevToken.name = NULL;
}

/*Funkcia pre zistenie najvrchnejseho terminalu na zasobniku*/
tStackElemPtr StackTopTerm (tStack *stack) {
    tStackElemPtr TmpPtr3 = NULL;
    TmpPtr3 = stack->Top;
    while(TmpPtr3->type > PSBeginEOL) {
        TmpPtr3 = TmpPtr3->lptr;
    }
    return TmpPtr3;
}

/*Procedura pre zrusenie najvrchnejsieho elementu na zasobniku*/
void StackPop(tStack* stack) {
    tStackElemPtr TmpPtr4 = NULL;
    if (stack->Top != NULL) {
        TmpPtr4 = stack->Top;
        if(stack->Top->IDdata != NULL){
            free (stack->Top->IDdata);
        }
        if (stack->Top == stack->First) {
            stack->Top = NULL;
            stack->First = NULL;
        }
        else {
            stack->Top = stack->Top->lptr;
            stack->Top->rptr = NULL;
        }
        free (TmpPtr4);
    }
}

/*Funkcia pre pridanie elementu na zasobnik*/
int StackPush(tStack *stack, tToken token) {
    tStackElemPtr TmpPtr5 = NULL;
    TmpPtr5 = malloc(sizeof(struct tStackElem));
    if (TmpPtr5 == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    TokenToTerm(token,&(TmpPtr5->type),&(TmpPtr5->IDdata),&(TmpPtr5->IDtype),1);
    TmpPtr5->SEMtype = TmpPtr5->IDtype;
    TmpPtr5->lptr = stack->Top;
    TmpPtr5->rptr = NULL;
    stack->Top->rptr = TmpPtr5;
    stack->Top = TmpPtr5;
    return 1;
}

/*Funkcia pre shift*/
int StackShift(tStack *stack,tToken token) {
    tStackElemPtr TmpPtrNew = NULL, PtrTopTerm = NULL;
    int result = 0;

    PtrTopTerm = StackTopTerm(stack);       //ziskava adresu najvrchnejsieho terminalu
    TmpPtrNew = malloc(sizeof(struct tStackElem));

    if (TmpPtrNew == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }

    TmpPtrNew->type = PSHandle;           //vklada Handle
    TmpPtrNew->IDdata = NULL;
    TmpPtrNew->IDtype = H_FALSE;
    TmpPtrNew->SEMtype = H_FALSE;
    TmpPtrNew->lptr = PtrTopTerm;
    TmpPtrNew->rptr = PtrTopTerm->rptr;

    if(PtrTopTerm == stack->Top)
        stack->Top = TmpPtrNew;
    else
        PtrTopTerm->rptr->lptr = TmpPtrNew;

    PtrTopTerm->rptr = TmpPtrNew;

    result = StackPush(stack, token);           //vklada terminal zo vstupu
    if(result != 1)
        return result;
    return 1;
}

/*Funkcia pre vkladanie nonterminalu E na zasobnik*/
int StackPushE(tStack *stack) {
    tStackElemPtr TmpPtr6 = NULL;
    PSTerm tmpIDterm;
    char *tmpIDdata;
    HashType tmpIDtype,tmpSEMtype;

    TmpPtr6 = malloc(sizeof(struct tStackElem));
    if (TmpPtr6 == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }

    tmpIDterm = stack->Top->type;
    if (tmpIDterm == PSNonTerm){
        TmpPtr6->type = PSNonTerm;
        TmpPtr6->IDdata = NULL;
        TmpPtr6->IDtype = H_FALSE;
        TmpPtr6->SEMtype = semIDtype;
        StackPop (stack);                   //rusenie E
        StackPop (stack);                   //rusenie operátoru (+,-,*,/,<,>,<=,>=,<>,=)
        StackPop (stack);                   //rusenie E
        StackPop (stack);                   //rusenie handle <
    }
    else{
        if(tmpIDterm == PSParenthR){
            tmpIDdata = stack->Top->lptr->IDdata;
            tmpIDtype = stack->Top->lptr->IDtype;
            tmpSEMtype = stack->Top->lptr->SEMtype;
            stack->Top->lptr->IDdata = NULL;
            TmpPtr6->type = PSNonTerm;
            TmpPtr6->IDdata = tmpIDdata;
            TmpPtr6->IDtype = tmpIDtype;
            TmpPtr6->SEMtype = tmpSEMtype;
            StackPop (stack);                   //rusenie )
            StackPop (stack);                   //rusenie E
            StackPop (stack);                   //rusenie (
            StackPop (stack);                   //rusenie handle <
        }
    }

    TmpPtr6->lptr = stack->Top;
    TmpPtr6->rptr = NULL;
    stack->Top->rptr = TmpPtr6;
    stack->Top = TmpPtr6;

    return 1;
}

/*Funkcia pre vkladanie nonterminalu E na zasobnik ak je na vrchole zasobniku <i */
int StackPushIDtoE(tStack *stack) {
    tStackElemPtr TmpPtr7 = NULL;
    char *tmpIDdata = stack->Top->IDdata;
    HashType tmpIDtype = stack->Top->SEMtype;
    stack->Top->IDdata = NULL;

    TmpPtr7 = malloc(sizeof(struct tStackElem));
    if (TmpPtr7 == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    TmpPtr7->type = PSNonTerm;
    TmpPtr7->IDdata = tmpIDdata;
    TmpPtr7->IDtype = tmpIDtype;
    TmpPtr7->SEMtype = tmpIDtype;

    StackPop (stack);                   //rusenie i
    StackPop (stack);                   //rusenie handle <

    TmpPtr7->lptr = stack->Top;
    TmpPtr7->rptr = NULL;
    stack->Top->rptr = TmpPtr7;
    stack->Top = TmpPtr7;

    return 1;
}

/*Procedura pre prevod prichadzajucich tokenov na terminaly*/
void TokenToTerm(tToken token,PSTerm *term,char **data, HashType *Htype, int control){
    int sizeofname = 0;
    if (token.name != NULL){
        sizeofname = strlen(token.name);
    }
    (*data) = NULL;
    (*Htype) = H_FALSE;
    switch (token.state) {
    case S_MUL:
        (*term) = PSMul;
        break;
    case S_DIV:
        (*term) = PSDiv;
        break;
    case S_ADD:
        (*term) = PSAdd;
        break;
    case S_SUB:
        (*term) = PSSub;
        break;
    case S_LS:
        (*term) = PSLess;
        break;
    case S_GR:
        (*term) = PSGreat;
        break;
    case S_LSEQ:
        (*term) = PSLessEqual;
        break;
    case S_GREQ:
        (*term) = PSGreatEqual;
        break;
    case S_NOTEQ:
        (*term) = PSNotEqual;
        break;
    case S_EQ:
        (*term) = PSEqual;
        break;
    case S_LPAR:
        (*term) = PSParenthL;
        break;
    case S_RPAR:
        (*term) = PSParenthR;
        break;
    case S_INT:
    case S_REAL1:
    case S_REAL2:
    case S_STR:
        (*term) = PSID;
        if (control == 1){
            if (((*data) = malloc(sizeofname+1)) == NULL){
                if(errCode == 0){
                    errCode = E_ERR;
                }
            }
            strcpy((*data), token.name);
        }
        else {
            (*data) = NULL;
        }
        if (token.state == S_INT) {
            (*Htype) = H_INT;
        }
        else if ((token.state == S_REAL1) || (token.state == S_REAL2)){
            (*Htype) = H_REAL;
        }
        else if (token.state == S_STR) {
            (*Htype) = H_STRING;
        }
        break;
    case S_ID:
        if (isKeyWord(token)) {                    //kontrola klucovych slov
            if((strcmp(token.name, "true") == 0)||(strcmp(token.name, "false") == 0)){
                (*term) = PSID;
                if (control == 1){
                    if (((*data) = malloc(sizeof(char) * (sizeofname+1))) == NULL){
                        if(errCode == 0){
                            errCode = E_ERR;
                        }
                    }
                    strcpy((*data), token.name);
                }
                else {
                    (*data) = NULL;
                }
                (*Htype) = H_BOOL;
            }
            else{
                (*term) = PSBeginEOL;
            }
        }
        else {
            (*term) = PSID;
            if (control == 1){
                if (((*data) = malloc(sizeof(char) * (sizeofname+1))) == NULL){
                    if(errCode == 0){
                        errCode = E_ERR;
                    }
                }
                strcpy((*data), token.name);

            }
            else {
                (*data) = NULL;
            }
            (*Htype) = H_ID;
        }
        break;
    default:
        (*term) = PSBeginEOL;
        break;
    }
}

/*Hlavna funkcia pre precedencnu analyzu*/
int prec_analysis(HashType *finishIDtype) {
    /*pomocne premenne*/
    int result = 0;
    tStackElemPtr TmpPtr8 = NULL;           //ukazatel na terminal najblizsie k vrcholu
    PSTerm tmpTerm;
    char *tmpData = NULL;
    HashType tmpHtype;
    if(StackInit (&stack) != 1) {
        StackDispose(&stack);
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    if (prevToken.state != S_FALSE) {
        if((result = StackShift(&stack,prevToken)) != 1) {
            StackDispose(&stack);
            return result;
        }
    }
    do {
        TmpPtr8 = StackTopTerm (&stack);
        TokenToTerm(currToken,&tmpTerm,&tmpData,&tmpHtype,0);

        switch(pTable[TmpPtr8->type][tmpTerm]) {
        case PTEqual:
            if((result = StackPush(&stack,currToken)) != 1) {
                StackDispose(&stack);
                return result;
            }
            currToken = getNextToken(file);
            break;
        case PTLess:
            if((result = StackShift(&stack,currToken)) != 1) {
                StackDispose(&stack);
                return result;
            }
            currToken = getNextToken(file);
            break;
        case PTGreat:
            switch (TmpPtr8->type) {
            case PSMul:
                if((result = mulRule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSDiv:
                if((result = divRule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSAdd:
                if((result = addRule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSSub:
                if((result = subRule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSLess:
            case PSGreat:
            case PSLessEqual:
            case PSGreatEqual:
                if((result = compare1Rule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSNotEqual:
            case PSEqual:
                if((result = compare2Rule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSParenthR:
                if((result = parenthRule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            case PSID:
                if((result = IDtoE_Rule(&stack)) != 1) {
                    StackDispose(&stack);
                    return result;
                }
                break;
            default:
                break;
            }
            break;
        case PTEmpty:
            StackDispose(&stack);
            if (errCode == 0) {
                errCode = E_SYN;
            }
            return 0;
            break;
        default :
            break;
        }
        TmpPtr8 = StackTopTerm (&stack);
    } while (pTable[TmpPtr8->type][tmpTerm] != PTEnd);
    if(stack.Top == stack.First){
        StackDispose(&stack);
        return 0;
    }
    if(semIDtype == H_FALSE) {
        StackDispose(&stack);
        return 0;
    }
    StackDispose(&stack);
    (*finishIDtype) = semIDtype;    //parseru je poslany konecny typ celeho vyrazu, pre semanticku kontrolu
    return 1;
}

/*Procedura pre generovanie trojadresneho kodu spolu so semantickymi kontrolami*/
void generate_3ac(tStack *stack){
    int sizeofname = 0;
    tHTItem* isStored2;

    switch(stack->Top->lptr->lptr->SEMtype){
        case H_INT:
            switch(stack->Top->SEMtype){
                case H_INT:
                    semIDtype = H_INT;
                    break;
                case H_REAL:
                    semIDtype = H_REAL;
                    break;
                case H_STRING:
                case H_BOOL:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
                case H_ID:
                    if(inFunction == 0){
                        isStored = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                    else{
                        if(inFunction == 1){
                            isStored = htSearch(globalLocalTable,stack->Top->IDdata);
                            if(isStored == NULL){
                                isStored = htSearch(globalTable,stack->Top->IDdata);
                                if(isStored == NULL){
                                    if(errCode == 0){
                                        errCode = E_SEMP;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if(isStored->data.type == NULL){
                        if(errCode == 0){
                            errCode = E_SYN;
                            return;
                        }
                    }
                    if((strcmp(isStored->data.type,"integer")) == 0){
                        semIDtype = H_INT;
                    }
                    else{
                        if((strcmp(isStored->data.type,"real")) == 0){
                            semIDtype = H_REAL;
                        }
                        else{
                            if(errCode == 0){
                                errCode = E_SEMF;
                            }
                        }
                    }
                    break;
                default:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
            }
            break;
        case H_REAL:
            switch(stack->Top->SEMtype){
                case H_INT:
                case H_REAL:
                    semIDtype = H_REAL;
                    break;
                case H_STRING:
                case H_BOOL:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
                case H_ID:
                    if(inFunction == 0){
                        isStored = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                    else{
                        if(inFunction == 1){
                            isStored = htSearch(globalLocalTable,stack->Top->IDdata);
                            if(isStored == NULL){
                                isStored = htSearch(globalTable,stack->Top->IDdata);
                                if(isStored == NULL){
                                    if(errCode == 0){
                                        errCode = E_SEMP;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if(isStored->data.type == NULL){
                        if(errCode == 0){
                            errCode = E_SYN;
                            return;
                        }
                    }
                    if(((strcmp(isStored->data.type,"integer")) == 0)||((strcmp(isStored->data.type,"real")) == 0)){
                        semIDtype = H_REAL;
                    }
                    else{
                        if(errCode == 0){
                            errCode = E_SEMF;
                        }
                    }
                    break;
                default:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
            }
            break;
        case H_STRING:
            switch(stack->Top->SEMtype){
                case H_STRING:
                    semIDtype = H_STRING;
                    break;
                case H_INT:
                case H_REAL:
                case H_BOOL:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
                case H_ID:
                    if(inFunction == 0){
                        isStored = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                    else{
                        if(inFunction == 1){
                            isStored = htSearch(globalLocalTable,stack->Top->IDdata);
                            if(isStored == NULL){
                                isStored = htSearch(globalTable,stack->Top->IDdata);
                                if(isStored == NULL){
                                    if(errCode == 0){
                                        errCode = E_SEMP;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if(isStored->data.type == NULL){
                        if(errCode == 0){
                            errCode = E_SYN;
                            return;
                        }
                    }
                    if((strcmp(isStored->data.type,"string")) == 0){
                        semIDtype = H_STRING;
                    }
                    else{
                        if(errCode == 0){
                            errCode = E_SEMF;
                        }
                    }
                    break;
                default:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
            }
            break;
        case H_BOOL:
            switch(stack->Top->SEMtype){
                case H_BOOL:
                    semIDtype = H_BOOL;
                    break;
                case H_INT:
                case H_REAL:
                case H_STRING:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
                case H_ID:
                    if(inFunction == 0){
                        isStored = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                    else{
                        if(inFunction == 1){
                            isStored = htSearch(globalLocalTable,stack->Top->IDdata);
                            if(isStored == NULL){
                                isStored = htSearch(globalTable,stack->Top->IDdata);
                                if(isStored == NULL){
                                    if(errCode == 0){
                                        errCode = E_SEMP;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if(isStored->data.type == NULL){
                        if(errCode == 0){
                            errCode = E_SYN;
                            return;
                        }
                    }
                    if((strcmp(isStored->data.type,"boolean")) == 0){
                        semIDtype = H_BOOL;
                    }
                    else{
                        if(errCode == 0){
                            errCode = E_SEMF;
                        }
                    }
                    break;
                default:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
            }
            break;
        case H_ID:
            if(inFunction == 0){
                isStored = htSearch(globalTable,stack->Top->lptr->lptr->IDdata);
                if(isStored == NULL){
                    if(errCode == 0){
                        errCode = E_SEMP;
                        return;
                    }
                }
            }
            else{
                if(inFunction == 1){
                    isStored = htSearch(globalLocalTable,stack->Top->lptr->lptr->IDdata);
                    if(isStored == NULL){
                        isStored = htSearch(globalTable,stack->Top->lptr->lptr->IDdata);
                        if(isStored == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                }
            }
            if(isStored->data.type == NULL){
                if(errCode == 0){
                    errCode = E_SYN;
                    return;
                }
            }
            switch(stack->Top->SEMtype){
                case H_INT:
                    if((strcmp(isStored->data.type,"integer")) == 0){
                        semIDtype = H_INT;
                    }
                    if((strcmp(isStored->data.type,"real")) == 0){
                        semIDtype = H_REAL;
                    }
                    break;
                case H_REAL:
                    if (((strcmp(isStored->data.type,"integer")) == 0)||((strcmp(isStored->data.type,"real")) == 0)){
                        semIDtype = H_REAL;
                    }
                    break;
                case H_STRING:
                    if((strcmp(isStored->data.type,"string")) == 0){
                        semIDtype = H_STRING;
                    }
                    break;
                case H_BOOL:
                    if((strcmp(isStored->data.type,"boolean")) == 0){
                        semIDtype = H_BOOL;
                    }
                    break;
                case H_ID:
                    if(inFunction == 0){
                        isStored2 = htSearch(globalTable,stack->Top->IDdata);
                        if(isStored2 == NULL){
                            if(errCode == 0){
                                errCode = E_SEMP;
                                return;
                            }
                        }
                    }
                    else{
                        if(inFunction == 1){
                            isStored2 = htSearch(globalLocalTable,stack->Top->IDdata);
                            if(isStored2 == NULL){
                                isStored2 = htSearch(globalTable,stack->Top->IDdata);
                                if(isStored2 == NULL){
                                    if(errCode == 0){
                                        errCode = E_SEMP;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    if(isStored2->data.type == NULL){
                        if(errCode == 0){
                            errCode = E_SYN;
                            return;
                        }
                    }
                    if(((strcmp(isStored->data.type,"integer")) == 0)&&((strcmp(isStored2->data.type,"integer")) == 0)){
                        semIDtype = H_INT;
                    }
                    if(((strcmp(isStored->data.type,"string")) == 0)&&((strcmp(isStored2->data.type,"string"))== 0)){
                        semIDtype = H_STRING;
                    }
                    if(((strcmp(isStored->data.type,"boolean")) == 0)&&((strcmp(isStored2->data.type,"boolean"))== 0)){
                        semIDtype = H_BOOL;
                    }
                    if((((strcmp(isStored->data.type,"integer")) == 0)&&((strcmp(isStored2->data.type,"real"))== 0))||
                       (((strcmp(isStored->data.type,"real")) == 0)&&((strcmp(isStored2->data.type,"integer"))== 0))||
                        (((strcmp(isStored->data.type,"real")) == 0)&&((strcmp(isStored2->data.type,"real"))== 0))){
                        semIDtype = H_INT;
                    }
                    break;
                default:
                    if(errCode == 0){
                        errCode = E_SEMF;
                    }
                    break;
            }
            break;
        default:
            if(errCode == 0){
                errCode = E_SEMF;
            }
            break;
    }
    if (semIDtype == H_FALSE){
        if(errCode == 0){
            errCode = E_SEMF;
        }
    }

    /*ak semanticke kontroly prebehli uspesne, generuje sa trojadresny kod*/
    if (errCode == 0) {
        if(stack->Top->lptr->lptr->IDdata != NULL){
            if (stack->Top->lptr->lptr->SEMtype == H_INT){
                Instr_var.Ins_int = atoi(stack->Top->lptr->lptr->IDdata);
                IListInsert(&Instr_list,I_PUSH,&Instr_var.Ins_int,NULL,NULL,NULL,NULL);
            }
            if (stack->Top->lptr->lptr->SEMtype == H_REAL){
                Instr_var.Ins_float = atof(stack->Top->lptr->lptr->IDdata);
                IListInsert(&Instr_list,I_PUSH,NULL,&Instr_var.Ins_float,NULL,NULL,NULL);
            }
            if (stack->Top->lptr->lptr->SEMtype == H_STRING){
                sizeofname = strlen(stack->Top->lptr->lptr->IDdata);
                if ((Instr_var.Ins_string = malloc(sizeofname+1)) == NULL){
                    if(errCode == 0){
                        errCode = E_ERR;
                        return;
                    }
                }
                strcpy(Instr_var.Ins_string, stack->Top->lptr->lptr->IDdata);
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,Instr_var.Ins_string,NULL);
                free(Instr_var.Ins_string);
            }
            if (stack->Top->lptr->lptr->SEMtype == H_BOOL){
                if (strcmp(stack->Top->lptr->lptr->IDdata, "true") == 0){
                    Instr_var.Ins_bool = 1;
                }
                else if (strcmp(stack->Top->lptr->lptr->IDdata, "false") == 0){
                    Instr_var.Ins_bool = 0;
                }
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,&Instr_var.Ins_bool,NULL,NULL);
            }
            if (stack->Top->lptr->lptr->SEMtype == H_ID){
                sizeofname = strlen(stack->Top->lptr->lptr->IDdata);
                if ((Instr_var.Ins_name = malloc(sizeofname+1)) == NULL){
                    if(errCode == 0){
                        errCode = E_ERR;
                        return;
                    }
                }
                strcpy(Instr_var.Ins_name, stack->Top->lptr->lptr->IDdata);
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,NULL,Instr_var.Ins_name);
                free(Instr_var.Ins_name);
            }
        }

        if(stack->Top->IDdata != NULL){
            if (stack->Top->SEMtype == H_INT){
                Instr_var.Ins_int = atoi(stack->Top->IDdata);
                IListInsert (&Instr_list,I_PUSH,&Instr_var.Ins_int,NULL,NULL,NULL,NULL);
            }
            if (stack->Top->SEMtype == H_REAL){
                Instr_var.Ins_float = atof(stack->Top->IDdata);
                IListInsert (&Instr_list,I_PUSH,NULL,&Instr_var.Ins_float,NULL,NULL,NULL);
            }
            if (stack->Top->SEMtype == H_STRING){
                sizeofname = strlen(stack->Top->IDdata);
                if ( (Instr_var.Ins_string = malloc(sizeofname+1)) == NULL){
                    if(errCode == 0){
                        errCode = E_ERR;
                        return;
                    }
                }
                strcpy(Instr_var.Ins_string, stack->Top->IDdata);
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,Instr_var.Ins_string,NULL);
                free(Instr_var.Ins_string);
            }
            if (stack->Top->SEMtype == H_BOOL){
                if (strcmp(stack->Top->IDdata, "true") == 0){
                    Instr_var.Ins_bool = 1;
                }
                else if (strcmp(stack->Top->IDdata, "false") == 0){
                    Instr_var.Ins_bool = 0;
                }
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,&Instr_var.Ins_bool,NULL,NULL);
            }
            if (stack->Top->SEMtype == H_ID){
                sizeofname = strlen(stack->Top->IDdata);
                if ((Instr_var.Ins_name = malloc(sizeofname+1)) == NULL){
                    if(errCode == 0){
                        errCode = E_ERR;
                        return;
                    }
                }
                strcpy(Instr_var.Ins_name, stack->Top->IDdata);
                IListInsert (&Instr_list,I_PUSH,NULL,NULL,NULL,NULL,Instr_var.Ins_name);
                free(Instr_var.Ins_name);
            }
        }
    }
}

/*Funkcia pre pravidlo E->E*E */
int mulRule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm)) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if ((semIDtype == H_REAL)||(semIDtype == H_INT)){
        IListInsert (&Instr_list,I_MUL,NULL,NULL,NULL,NULL,NULL);
    }
    else{
        if (errCode == 0) {
            errCode = E_SEMF;
        }
        return 0;
    }

    if(StackPushE (stack) != 1){      //vkladanie nonterminalu E namiesto E*E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidlo E->E/E */
int divRule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm)) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if ((semIDtype == H_REAL)||(semIDtype == H_INT)){
        if (semIDtype == H_INT){
            semIDtype = H_REAL;
        }
        IListInsert (&Instr_list,I_DIV,NULL,NULL,NULL,NULL,NULL);
    }
    else{
        if (errCode == 0) {
            errCode = E_SEMF;
        }
        return 0;
    }

    if(StackPushE (stack) != 1){      //vkladanie nonterminalu E namiesto E/E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidlo E->E+E */
int addRule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm)) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if ((semIDtype == H_REAL)||(semIDtype == H_INT)||(semIDtype == H_STRING)){
        IListInsert (&Instr_list,I_ADD,NULL,NULL,NULL,NULL,NULL);
    }
    else{
        if (errCode == 0) {
            errCode = E_SEMF;
        }
        return 0;
    }

    if(StackPushE (stack) != 1){       //vkladanie nonterminalu E namiesto E+E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidlo E->E-E */
int subRule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm)) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if ((semIDtype == H_REAL)||(semIDtype == H_INT)){
        IListInsert (&Instr_list,I_SUB,NULL,NULL,NULL,NULL,NULL);
    }
    else{
        if (errCode == 0) {
            errCode = E_SEMF;
        }
        return 0;
    }

    if(StackPushE (stack) != 1){       //vkladanie nonterminalu E namiesto E-E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidla E->E<E,E->E>E,E->E<=E,E->E>=E */
int compare1Rule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm))
    {   if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if (stack->Top->lptr->type == PSLess){
        IListInsert (&Instr_list,I_LS,NULL,NULL,NULL,NULL,NULL);
    }
    if (stack->Top->lptr->type == PSGreat){
        IListInsert (&Instr_list,I_GR,NULL,NULL,NULL,NULL,NULL);
    }
    if (stack->Top->lptr->type == PSLessEqual){
        IListInsert (&Instr_list,I_LSEQ,NULL,NULL,NULL,NULL,NULL);
    }
    if (stack->Top->lptr->type == PSGreatEqual){
        IListInsert (&Instr_list,I_GREQ,NULL,NULL,NULL,NULL,NULL);
    }

    semIDtype = H_BOOL;

    if(StackPushE (stack) != 1){       //vkladanie nonterminalu E namiesto E op E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidla E->E<>E a E->E=E */
int compare2Rule(tStack *stack) {
    if((stack->Top->type != PSNonTerm)||(stack->Top->lptr->lptr->type != PSNonTerm)) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }

    generate_3ac(stack);
    if (errCode != 0) {
        return 0;
    }

    if (stack->Top->lptr->type == PSEqual){
        IListInsert (&Instr_list,I_EQ,NULL,NULL,NULL,NULL,NULL);
    }
    if (stack->Top->lptr->type == PSNotEqual){
        IListInsert (&Instr_list,I_NOTEQ,NULL,NULL,NULL,NULL,NULL);
    }

    semIDtype = H_BOOL;

    if(StackPushE (stack) != 1){       //vkladanie nonterminalu E namiesto E op E
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidlo E->(E) */
int parenthRule(tStack *stack) {
    if(stack->Top->lptr->type != PSNonTerm) {
        if (errCode == 0) {
            errCode = E_SYN;
        }
        return 0;
    }
    if(StackPushE (stack) != 1){       //vkladanie nonterminalu E namiesto (E)
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

/*Funkcia pre pravidlo E->i */
int IDtoE_Rule(tStack *stack) {
    if(StackPushIDtoE (stack) != 1){       //vkladanie nonterminalu E namiesto <i
        if (errCode == 0) {
            errCode = E_ERR;
        }
        return 0;
    }
    return 1;
}

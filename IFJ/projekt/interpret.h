#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "instruction_list.h"

#define TYPEINT 'i'
#define TYPEREAL 'r'
#define TYPEBOOL 'b'
#define TYPESTRING 's'
#define TYPEFUNC 'f'
#define TYPEID 'd'




typedef enum {
    X_ZAC_KON,
    X_END_FUNC_I,
    X_END_FUNC_R,
    X_END_FUNC_B,
    X_END_FUNC_S,
    X_PUSH_I,
    X_PUSH_R,
    X_PUSH_B,
    X_PUSH_S,
    X_PUSH_IDL_I,
    X_PUSH_IDL_R,
    X_PUSH_IDL_B,
    X_PUSH_IDL_S,
    X_PUSH_IDG_I,
    X_PUSH_IDG_R,
    X_PUSH_IDG_B,
    X_PUSH_IDG_S,
    X_POP_L_I,
    X_POP_L_R,
    X_POP_L_B,
    X_POP_L_S,
    X_POP_G_I,
    X_POP_G_R,
    X_POP_G_B,
    X_POP_G_S,
    X_CALL,
    X_CALL_Leng,
    X_CALL_Copy,
    X_CALL_Find,
    X_CALL_Sort,
    X_WRITE,
    X_READ_L_I,
    X_READ_L_R,
    X_READ_L_B,
    X_READ_L_S,
    X_READ_G_I,
    X_READ_G_R,
    X_READ_G_B,
    X_READ_G_S,
    X_IF,       //else sekce;
    X_END_IF,
    X_WHILE,    //end while
    X_END_WHILE,//zacatek while

    X_ADD_X_I,
    X_ADD_X_R,
    X_ADD_X_S,
    X_SUB_X_I,
    X_SUB_X_R,
    X_MUL_X_I,
    X_MUL_X_R,
    X_DIV_X_I,
    X_DIV_X_R,

    X_EQ_X_I,
    X_EQ_X_R,
    X_EQ_X_B,
    X_EQ_X_S,
    X_LS_X_I,
    X_LS_X_R,
    X_LS_X_B,
    X_LS_X_S,
    X_LSEQ_X_I,
    X_LSEQ_X_R,
    X_LSEQ_X_B,
    X_LSEQ_X_S,
    X_NOTEQ_X_I,
    X_NOTEQ_X_R,
    X_NOTEQ_X_B,
    X_NOTEQ_X_S,
    X_GR_X_I,
    X_GR_X_R,
    X_GR_X_B,
    X_GR_X_S,
    X_GREQ_X_I,
    X_GREQ_X_R,
    X_GREQ_X_B,
    X_GREQ_X_S,


    X_ADD_IDL_I,
    X_ADD_IDL_R,
    X_ADD_IDL_S,
    X_SUB_IDL_I,
    X_SUB_IDL_R,
    X_MUL_IDL_I,
    X_MUL_IDL_R,
    X_DIV_IDL_I,
    X_DIV_IDL_R,

    X_EQ_IDL_I,
    X_EQ_IDL_R,
    X_EQ_IDL_B,
    X_EQ_IDL_S,
    X_LS_IDL_I,
    X_LS_IDL_R,
    X_LS_IDL_B,
    X_LS_IDL_S,
    X_LSEQ_IDL_I,
    X_LSEQ_IDL_R,
    X_LSEQ_IDL_B,
    X_LSEQ_IDL_S,
    X_NOTEQ_IDL_I,
    X_NOTEQ_IDL_R,
    X_NOTEQ_IDL_B,
    X_NOTEQ_IDL_S,
    X_GR_IDL_I,
    X_GR_IDL_R,
    X_GR_IDL_B,
    X_GR_IDL_S,
    X_GREQ_IDL_I,
    X_GREQ_IDL_R,
    X_GREQ_IDL_B,
    X_GREQ_IDL_S,


    X_ADD_IDG_I,
    X_ADD_IDG_R,
    X_ADD_IDG_S,
    X_SUB_IDG_I,
    X_SUB_IDG_R,
    X_MUL_IDG_I,
    X_MUL_IDG_R,
    X_DIV_IDG_I,
    X_DIV_IDG_R,

    X_EQ_IDG_I,
    X_EQ_IDG_R,
    X_EQ_IDG_B,
    X_EQ_IDG_S,
    X_LS_IDG_I,
    X_LS_IDG_R,
    X_LS_IDG_B,
    X_LS_IDG_S,
    X_LSEQ_IDG_I,
    X_LSEQ_IDG_R,
    X_LSEQ_IDG_B,
    X_LSEQ_IDG_S,
    X_NOTEQ_IDG_I,
    X_NOTEQ_IDG_R,
    X_NOTEQ_IDG_B,
    X_NOTEQ_IDG_S,
    X_GR_IDG_I,
    X_GR_IDG_R,
    X_GR_IDG_B,
    X_GR_IDG_S,
    X_GREQ_IDG_I,
    X_GREQ_IDG_R,
    X_GREQ_IDG_B,
    X_GREQ_IDG_S,

    X_ADD,
    X_SUB,
    X_MUL,
    X_DIV,
    X_EQ ,
    X_LS,
    X_LSEQ,
    X_NOTEQ,
    X_GR,
    X_GREQ

} tUprIns;

typedef struct sUprInsElem {
    struct sUprInsElem *ptr;
    tUprIns ins;

    char type;
    int i;
    float f;
    char str[256];
    struct sUprInsElem **adr;   //ukazatel na prvek který ukazuje na ins
    struct sUprInsElem ***adr2; //ukazatel na ukazatel v tabulce symbolu, ktery ukazuje tam kam **adr
} *tUprInsElemPtr;

typedef struct {
    tUprInsElemPtr First;
    tUprInsElemPtr *begin_main;
    tUprInsElemPtr Last;
} tUprIList;

typedef struct {
    char type;

    int i;
    float f;
    char str[256];
} tUprData;

int interpret(tIList *);

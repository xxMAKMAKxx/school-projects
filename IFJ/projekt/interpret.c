#include "interpret.h"
#include "int_valuetable.h"

 //  /*
#include "parser.h"
#include "ial.h"
extern tError errCode;

void UprInit(tUprIList *list){
    list->begin_main = NULL;
    list->First = NULL;
    list->Last = NULL;
}

void UprDispose(tUprIList *list){
    tUprInsElemPtr elem;

    while(list->First != NULL) {
        elem = list->First->ptr;
        free(list->First);
        list->First = elem;
    }
}

tUprInsElemPtr UprInsert(tUprIList *list,tUprIns ins, char type, const void *value, const char *params){
    tUprInsElemPtr elem;
    if(list->Last == NULL) {
        list->Last = malloc(sizeof(struct sUprInsElem));
        elem = list->Last;
        list->First = elem;
    }
    else {
        list->Last->ptr = malloc(sizeof(struct sUprInsElem));
        elem = list->Last->ptr;
        list->Last = elem;
    }
    elem->ptr = NULL;
    elem->adr = NULL;
    elem->adr2 = NULL;
    elem->ins = ins;
    elem->type = type;
    switch(type) {
    case TYPEINT:
        elem->i = *(int*)value;
        break;
    case TYPEREAL:
        elem->f = *(float*)value;
        break;
    case TYPEBOOL:
        elem->i = *(int*)value;
        break;
    case TYPESTRING:
        strcpy(elem->str, (char*)value);
        break;
    case TYPEFUNC:
        elem->i = *(int*)value;
        strcpy(elem->str, params);
        break;
    case TYPEID:
        elem->i = *(int*)value;
        break;
    default:
        break;
    }
    return elem;
}

void UprUpdateAdr(tUprInsElemPtr elem, tUprInsElemPtr *adr){
    elem->adr = adr;
}

void UprUpdateIns(tUprInsElemPtr elem, tUprIns ins){
    elem->ins = ins;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------
//----------------------------------------
//--------------------




void func_length(char *s, int *ret) {
    int i;
    for(i = 0; s[i] != '\0'; i++);
    *ret = i;
}

void func_copy(char *s, int i, int n, char *ret) {
    int j, k;
    k = 0;
    for(j = 1; s[j-1] != '\0'; j++) {
        if(j >= i && k < n) {
            ret[k] = s[j-1];
            k++;
        }
    }
    ret[k] = '\0';
}

void func_find(char *str1, char *str2, int *ret) {
    BoyerMoorAlogythm(str1, str2, ret);
}

void func_sort(char *string, char *ret) {
    QuickSort(string, 0, strlen(string)-1);
    strcpy(ret, string);
}


int UprInterpret(tUprInsElemPtr instrukce, int pocet_lok_prom, char *params, tUprData *global_prom, tUprData *zasobnik, register int stack_pointer){
    int i, j;
    char c;
    tUprData local_prom[pocet_lok_prom];

    //printf("pocet lok prom: %i\nparametry: %s\n", pocet_lok_prom, params);

    for(i = 0; i < pocet_lok_prom; i++){
        local_prom[i].type = 0;
    }
    for(i = 1; params[i] ; i++){
        local_prom[i].type = params[i];
        switch(local_prom[i].type){
        case TYPEINT:
            local_prom[i].i = zasobnik[stack_pointer].i;
            break;
        case TYPEREAL:
            local_prom[i].f = zasobnik[stack_pointer].f;
            break;
        case TYPEBOOL:
            local_prom[i].i = zasobnik[stack_pointer].i;
            break;
        case TYPESTRING:
            strcpy(local_prom[i].str, zasobnik[stack_pointer].str);
            break;
        }
        stack_pointer--;
    }


    while(instrukce != NULL){
        //printf("Ins: %s, type: %c, zas[%i]: %i, %g", enn[instrukce->ins], instrukce->type,stack_pointer, zasobnik[stack_pointer].i, zasobnik[stack_pointer].f);getchar();
        switch(instrukce->ins){
        case X_ZAC_KON:
            break;
//------------------------------------------------------------
        case X_END_FUNC_I:
            if(local_prom[0].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = local_prom[0].i;
            zasobnik[stack_pointer].type = TYPEINT;
            return stack_pointer;
        case X_END_FUNC_R:
            if(local_prom[0].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].f = local_prom[0].f;
            zasobnik[stack_pointer].type = TYPEREAL;
            return stack_pointer;
        case X_END_FUNC_B:
            if(local_prom[0].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = local_prom[0].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            return stack_pointer;
        case X_END_FUNC_S:
            if(local_prom[0].type == 0) return -E_NIID;
            stack_pointer++;
            strcpy(zasobnik[stack_pointer].str, local_prom[0].str);
            zasobnik[stack_pointer].type = TYPESTRING;
            return stack_pointer;
//------------------------------------------------------------
        case X_PUSH_I:
            stack_pointer++;
            zasobnik[stack_pointer].i = instrukce->i;
            zasobnik[stack_pointer].type = TYPEINT;
            break;
        case X_PUSH_R:
            stack_pointer++;
            zasobnik[stack_pointer].f = instrukce->f;
            zasobnik[stack_pointer].type = TYPEREAL;
            break;
        case X_PUSH_B:
            stack_pointer++;
            zasobnik[stack_pointer].i = instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_PUSH_S:
            stack_pointer++;
            strcpy(zasobnik[stack_pointer].str, instrukce->str);
            zasobnik[stack_pointer].type = TYPESTRING;
            break;
//------------------------------------------------------------
        case X_PUSH_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEINT;
            break;
        case X_PUSH_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].f = local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEREAL;
            break;
        case X_PUSH_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_PUSH_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            strcpy(zasobnik[stack_pointer].str, local_prom[instrukce->i].str);
            zasobnik[stack_pointer].type = TYPESTRING;
            break;
//------------------------------------------------------------
        case X_PUSH_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEINT;
            break;
        case X_PUSH_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].f = global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEREAL;
            break;
        case X_PUSH_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            zasobnik[stack_pointer].i = global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_PUSH_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            stack_pointer++;
            strcpy(zasobnik[stack_pointer].str, global_prom[instrukce->i].str);
            zasobnik[stack_pointer].type = TYPESTRING;
            break;
//------------------------------------------------------------
        case X_POP_L_I:
            local_prom[instrukce->i].i = zasobnik[stack_pointer].i;
            local_prom[instrukce->i].type = TYPEINT;
            stack_pointer--;
            break;
        case X_POP_L_R:
            local_prom[instrukce->i].f = zasobnik[stack_pointer].f;
            local_prom[instrukce->i].type = TYPEREAL;
            stack_pointer--;
            break;
        case X_POP_L_B:
            local_prom[instrukce->i].i = zasobnik[stack_pointer].i;
            local_prom[instrukce->i].type = TYPEBOOL;
            stack_pointer--;
            break;
        case X_POP_L_S:
            strcpy(local_prom[instrukce->i].str, zasobnik[stack_pointer].str);
            local_prom[instrukce->i].type = TYPESTRING;
            stack_pointer--;
            break;
//------------------------------------------------------------
        case X_POP_G_I:
            global_prom[instrukce->i].i = zasobnik[stack_pointer].i;
            global_prom[instrukce->i].type = TYPEINT;
            stack_pointer--;
            break;
        case X_POP_G_R:
            global_prom[instrukce->i].f = zasobnik[stack_pointer].f;
            global_prom[instrukce->i].type = TYPEREAL;
            stack_pointer--;
            break;
        case X_POP_G_B:
            global_prom[instrukce->i].i = zasobnik[stack_pointer].i;
            global_prom[instrukce->i].type = TYPEBOOL;
            stack_pointer--;
            break;
        case X_POP_G_S:
            strcpy(global_prom[instrukce->i].str, zasobnik[stack_pointer].str);
            global_prom[instrukce->i].type = TYPESTRING;
            stack_pointer--;
            break;
//------------------------------------------------------------
        case X_CALL:
            stack_pointer = UprInterpret(**(instrukce->adr2), instrukce->i, instrukce->str, global_prom, zasobnik, stack_pointer);
            if(stack_pointer < -1) return stack_pointer;
            break;
        case X_CALL_Leng:
            func_length(zasobnik[stack_pointer].str, &(zasobnik[stack_pointer].i));
            zasobnik[stack_pointer].type = TYPEINT;
            break;
        case X_CALL_Copy:
            func_copy(zasobnik[stack_pointer].str, zasobnik[stack_pointer-1].i, zasobnik[stack_pointer-2].i, zasobnik[stack_pointer-2].str);
            stack_pointer--;
            stack_pointer--;
            zasobnik[stack_pointer].type = TYPESTRING;
            break;
        case X_CALL_Find:
            func_find(zasobnik[stack_pointer].str, zasobnik[stack_pointer-1].str, &(zasobnik[stack_pointer-1].i));
            stack_pointer--;
            zasobnik[stack_pointer].type = TYPEINT;
            break;
        case X_CALL_Sort:
            func_sort(zasobnik[stack_pointer].str, zasobnik[stack_pointer].str);
            break;
//------------------------------------------------------------
        case X_WRITE:
            for(i = instrukce->i; i > 0; i--){
                switch(zasobnik[stack_pointer].type){
                case TYPEINT:
                    printf("%i", zasobnik[stack_pointer].i);
                    break;
                case TYPEREAL:
                    printf("%g", zasobnik[stack_pointer].f);
                    break;
                case TYPEBOOL:
                    if(zasobnik[stack_pointer].i != 0) printf("TRUE");
                    else printf("FALSE");
                    break;
                case TYPESTRING:
                    printf("%s", zasobnik[stack_pointer].str);
                    break;
                }
                stack_pointer--;
            }
            break;
        case X_READ_L_I:
            if(scanf("%i", &(local_prom[instrukce->i].i)) != 1) return -E_READ;
            while((c = getchar()) != '\n' && c != EOF);
            local_prom[instrukce->i].type = TYPEINT;
            break;
        case X_READ_L_R:
            if(scanf("%g", &(local_prom[instrukce->i].f)) != 1) return -E_READ;
            while((c = getchar()) != '\n' && c != EOF);
            local_prom[instrukce->i].type = TYPEREAL;
            break;
        case X_READ_L_B:
            return -E_SEMF;
            break;
        case X_READ_L_S:
            i = 0;
            while((c = getchar()) != '\n' && c != EOF) {
                local_prom[instrukce->i].str[i] = c;
                i++;
            }
            local_prom[instrukce->i].str[i] = '\0';
            local_prom[instrukce->i].type = TYPESTRING;
            break;
        case X_READ_G_I:
            if(scanf("%i", &(global_prom[instrukce->i].i)) != 1) return -E_READ;
            while((c = getchar()) != '\n' && c != EOF);
            global_prom[instrukce->i].type = TYPEINT;
            break;
        case X_READ_G_R:
            if(scanf("%g", &(global_prom[instrukce->i].f)) != 1) return -E_READ;
            while((c = getchar()) != '\n' && c != EOF);
            global_prom[instrukce->i].type = TYPEREAL;
            break;
        case X_READ_G_B:
            return -E_SEMF;
            break;
        case X_READ_G_S:
            i = 0;
            while((c = getchar()) != '\n' && c != EOF) {
                global_prom[instrukce->i].str[i] = c;
                i++;
            }
            global_prom[instrukce->i].str[i] = '\0';
            global_prom[instrukce->i].type = TYPESTRING;
            break;
//------------------------------------------------------------
        case X_IF:
            if(zasobnik[stack_pointer].i == 0) {
               instrukce = *(instrukce->adr);
               stack_pointer--;
               continue;
            }
            stack_pointer--;
            break;
        case X_END_IF:
            instrukce = *(instrukce->adr);
            continue;
        case X_WHILE:
            if(zasobnik[stack_pointer].i == 0) {
               instrukce = *(instrukce->adr);
               stack_pointer--;
               continue;
            }
            stack_pointer--;
            break;
        case X_END_WHILE:
            instrukce = *(instrukce->adr);
            continue;
//------------------------------------------------------------
        case X_ADD_X_I:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i + instrukce->i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + (float)instrukce->i;
                break;
            }
            break;
        case X_ADD_X_R:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i + instrukce->f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + instrukce->f;
                break;
            }
            break;
        case X_ADD_X_S:
            j = 0;
            for(i = 0; zasobnik[stack_pointer].str[i]; i++);
            for(j = 0; instrukce->str[j]; j++) {
                zasobnik[stack_pointer].str[i] = instrukce->str[j];
                i++;
            }
            zasobnik[stack_pointer].str[i] = '\0';
            break;
        case X_SUB_X_I:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i - instrukce->i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - (float)instrukce->i;
                break;
            }
            break;
        case X_SUB_X_R:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i - instrukce->f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - instrukce->f;
                break;
            }
            break;
        case X_MUL_X_I:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i * instrukce->i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * (float)instrukce->i;
                break;
            }
            break;
        case X_MUL_X_R:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i * instrukce->f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * instrukce->f;
                break;
            }
            break;
        case X_DIV_X_I:
            if(instrukce->i == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / (float)instrukce->i;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / (float)instrukce->i;
                break;
            }
            break;
        case X_DIV_X_R:
            if(instrukce->f == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / instrukce->f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / instrukce->f;
                break;
            }
            break;
//------------------------------------------------------------
        case X_EQ_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f == instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) == 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f < instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) < 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f <= instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) <= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f != instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) != 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f > instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) > 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_X_I:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_X_R:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f >= instrukce->f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_X_B:
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= instrukce->i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_X_S:
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, instrukce->str) >= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
//------------------------------------------------------------
//------------------------------------------------------------
        case X_ADD_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i + local_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + (float)local_prom[instrukce->i].i;
                break;
            }
            break;
        case X_ADD_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i + local_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + local_prom[instrukce->i].f;
                break;
            }
            break;
        case X_ADD_IDL_S:
            j = 0;
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            for(i = 0; zasobnik[stack_pointer].str[i]; i++);
            for(j = 0; instrukce->str[j]; j++) {
                zasobnik[stack_pointer].str[i] = local_prom[instrukce->i].str[j];
                i++;
            }
            zasobnik[stack_pointer].str[i] = '\0';
            break;
        case X_SUB_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i - local_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - (float)local_prom[instrukce->i].i;
                break;
            }
            break;
        case X_SUB_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i - local_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - local_prom[instrukce->i].f;
                break;
            }
            break;
        case X_MUL_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i * local_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * (float)local_prom[instrukce->i].i;
                break;
            }
            break;
        case X_MUL_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i * local_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * local_prom[instrukce->i].f;
                break;
            }
            break;
        case X_DIV_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            if(local_prom[instrukce->i].i == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / (float)local_prom[instrukce->i].i;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / (float)local_prom[instrukce->i].i;
                break;
            }
            break;
        case X_DIV_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            if(local_prom[instrukce->i].f == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / local_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / local_prom[instrukce->i].f;
                break;
            }
            break;
//------------------------------------------------------------
        case X_EQ_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f == local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) == 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f < local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) < 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f <= local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) <= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f != local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) != 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f > local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) > 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDL_I:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDL_R:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f >= local_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDL_B:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= local_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDL_S:
            if(local_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, local_prom[instrukce->i].str) >= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
//------------------------------------------------------------
//------------------------------------------------------------
        case X_ADD_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i + global_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + (float)global_prom[instrukce->i].i;
                break;
            }
            break;
        case X_ADD_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i + global_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f + global_prom[instrukce->i].f;
                break;
            }
            break;
        case X_ADD_IDG_S:
            j = 0;
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            for(i = 0; zasobnik[stack_pointer].str[i]; i++);
            for(j = 0; instrukce->str[j]; j++) {
                zasobnik[stack_pointer].str[i] = global_prom[instrukce->i].str[j];
                i++;
            }
            zasobnik[stack_pointer].str[i] = '\0';
            break;
        case X_SUB_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i - global_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - (float)global_prom[instrukce->i].i;
                break;
            }
            break;
        case X_SUB_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i - global_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f - global_prom[instrukce->i].f;
                break;
            }
            break;
        case X_MUL_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].i = zasobnik[stack_pointer].i * global_prom[instrukce->i].i;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * (float)global_prom[instrukce->i].i;
                break;
            }
            break;
        case X_MUL_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i * global_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f * global_prom[instrukce->i].f;
                break;
            }
            break;
        case X_DIV_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            if(global_prom[instrukce->i].i == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / (float)global_prom[instrukce->i].i;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / (float)global_prom[instrukce->i].i;
                break;
            }
            break;
        case X_DIV_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            if(global_prom[instrukce->i].f == 0) return -E_NUL;
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer].f = (float)zasobnik[stack_pointer].i / global_prom[instrukce->i].f;
                zasobnik[stack_pointer].type = TYPEREAL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer].f = zasobnik[stack_pointer].f / global_prom[instrukce->i].f;
                break;
            }
            break;
//------------------------------------------------------------
        case X_EQ_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f == global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i == global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_EQ_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) == 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f < global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i < global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LS_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) < 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f <= global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i <= global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_LSEQ_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) <= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f != global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i != global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_NOTEQ_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) != 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f > global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i > global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GR_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) > 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDG_I:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDG_R:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].f >= global_prom[instrukce->i].f;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDG_B:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = zasobnik[stack_pointer].i >= global_prom[instrukce->i].i;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
        case X_GREQ_IDG_S:
            if(global_prom[instrukce->i].type == 0) return -E_NIID;
            zasobnik[stack_pointer].i = strcmp(zasobnik[stack_pointer].str, global_prom[instrukce->i].str) >= 0;
            zasobnik[stack_pointer].type = TYPEBOOL;
            break;
//------------------------------------------------------------
//------------------------------------------------------------
        case X_ADD:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i + zasobnik[stack_pointer].i;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f + (float)zasobnik[stack_pointer].i;
                    break;
                }
                break;
            case TYPEREAL:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].f = (float)zasobnik[stack_pointer-1].i + zasobnik[stack_pointer].f;
                    zasobnik[stack_pointer-1].type = TYPEREAL;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f + zasobnik[stack_pointer].f;
                    break;
                }
                break;
            case TYPESTRING:
                j = 0;
                for(i = 0; zasobnik[stack_pointer-1].str[i]; i++);
                for(j = 0; zasobnik[stack_pointer].str[j]; j++) {
                    zasobnik[stack_pointer-1].str[i] = zasobnik[stack_pointer].str[j];
                    i++;
                }
                zasobnik[stack_pointer-1].str[i] = '\0';
                break;
            }
            stack_pointer--;
            break;
        case X_SUB:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i - zasobnik[stack_pointer].i;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f - (float)zasobnik[stack_pointer].i;
                    break;
                }
                break;
            case TYPEREAL:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].f = (float)zasobnik[stack_pointer-1].i - zasobnik[stack_pointer].f;
                    zasobnik[stack_pointer-1].type = TYPEREAL;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f - zasobnik[stack_pointer].f;
                    break;
                }
                break;
            }
            stack_pointer--;
            break;
        case X_MUL:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i * zasobnik[stack_pointer].i;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f * (float)zasobnik[stack_pointer].i;
                    break;
                }
                break;
            case TYPEREAL:
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].f = (float)zasobnik[stack_pointer-1].i * zasobnik[stack_pointer].f;
                    zasobnik[stack_pointer-1].type = TYPEREAL;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f * zasobnik[stack_pointer].f;
                    break;
                }
                break;
            }
            stack_pointer--;
            break;
        case X_DIV:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                if(zasobnik[stack_pointer].i == 0) return -E_NUL;
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].f = (float)zasobnik[stack_pointer-1].i / (float)zasobnik[stack_pointer].i;
                    zasobnik[stack_pointer-1].type = TYPEREAL;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f / (float)zasobnik[stack_pointer].i;
                    break;
                }
                break;
            case TYPEREAL:
                if(zasobnik[stack_pointer].f == 0) return -E_NUL;
                switch(zasobnik[stack_pointer-1].type){
                case TYPEINT:
                    zasobnik[stack_pointer-1].f = (float)zasobnik[stack_pointer-1].i / zasobnik[stack_pointer].f;
                    zasobnik[stack_pointer-1].type = TYPEREAL;
                    break;
                case TYPEREAL:
                    zasobnik[stack_pointer-1].f = zasobnik[stack_pointer-1].f / zasobnik[stack_pointer].f;
                    break;
                }
                break;
            }
            stack_pointer--;
            break;

        case X_EQ :
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i == zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f == zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i == zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) == 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        case X_LS:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i < zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f < zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i < zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) < 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        case X_LSEQ:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i <= zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f <= zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i <= zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) <= 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        case X_NOTEQ:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i != zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f != zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i != zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) != 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        case X_GR:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i > zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f > zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i > zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) > 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        case X_GREQ:
            switch(zasobnik[stack_pointer].type){
            case TYPEINT:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i >= zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEREAL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].f >= zasobnik[stack_pointer].f;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPEBOOL:
                zasobnik[stack_pointer-1].i = zasobnik[stack_pointer-1].i >= zasobnik[stack_pointer].i;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            case TYPESTRING:
                zasobnik[stack_pointer-1].i = strcmp(zasobnik[stack_pointer-1].str, zasobnik[stack_pointer].str) >= 0;
                zasobnik[stack_pointer-1].type = TYPEBOOL;
                break;
            }
            stack_pointer--;
            break;
        default:
            printf("NEZNAMA INSTRUKCE!!!");
            break;
//------------------------------------------------------------
        }
        instrukce = instrukce->ptr;
    }
    return 0;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


tValTable pomGlobTable;
tValTable pomLocTable;
int ve_funkci;





tInsElemPtr UprFromIns_begin(tInsElemPtr old_ins, tUprIList *new_list);

tInsElemPtr UprFromIns_begin_whileif(tInsElemPtr old_ins, tUprIList *new_list){
    tUprInsElemPtr *adr_podminka;
    tUprInsElemPtr adr_ifwhile;
    tUprInsElemPtr adr_else;

    adr_podminka = &(new_list->Last->ptr);
    old_ins = UprFromIns_begin(old_ins, new_list);
    //instr if / while
    if(old_ins->ins == I_IF) {
        UprInsert(new_list, X_IF, 0, NULL, NULL);
        adr_ifwhile = new_list->Last;
        //dalsi instukce (begin)
        old_ins = old_ins->ptr;

        old_ins = UprFromIns_begin(old_ins, new_list);
        //instrukce end
        UprInsert(new_list, X_END_IF, 0, NULL, NULL);
        adr_else = new_list->Last;
        adr_ifwhile->adr = &(new_list->Last->ptr);
        //dalsi instukce (mozna else)
        if(old_ins->ptr->ins == I_ELSE) {
            //dalsi instukce (else)
            old_ins = old_ins->ptr;
            //dalsi instukce (begin)
            old_ins = old_ins->ptr;
            old_ins = UprFromIns_begin(old_ins, new_list);
            //instrukce end
            adr_else->adr = &(new_list->Last->ptr);

        }
        else{
            adr_else->adr = &(new_list->Last->ptr);
        }

    }
    else if(old_ins->ins == I_WHILE) {
        UprInsert(new_list, X_WHILE, 0, NULL, NULL);
        adr_ifwhile = new_list->Last;
        //dalsi instukce (begin)
        old_ins = old_ins->ptr;
        old_ins = UprFromIns_begin(old_ins, new_list);
        UprInsert(new_list, X_END_WHILE, 0, NULL, NULL);
        new_list->Last->adr = adr_podminka;
        adr_ifwhile->adr = &(new_list->Last->ptr);
        //instrukce end
    }
    return old_ins;
}



tInsElemPtr UprFromIns_begin(tInsElemPtr old_ins, tUprIList *new_list){
    tValTableElem *elem;
    while(old_ins != NULL) {
        old_ins = old_ins->ptr;
        switch (old_ins->ins) {
        case I_BEGIN:
            old_ins = UprFromIns_begin(old_ins, new_list);
            break;
        case I_END:
            return old_ins;
            break;
        case I_PUSH:
            if(old_ins->i != NULL) {
                UprInsert(new_list, X_PUSH_I, TYPEINT, old_ins->i,NULL);
            }
            if(old_ins->r != NULL) {
                UprInsert(new_list, X_PUSH_R, TYPEREAL, old_ins->r,NULL);
            }
            if(old_ins->b != NULL) {
                UprInsert(new_list, X_PUSH_B, TYPEBOOL, old_ins->b,NULL);
            }
            if(old_ins->s != NULL) {
                UprInsert(new_list, X_PUSH_S, TYPESTRING, old_ins->s,NULL);
            }
            if(old_ins->name != NULL) {
                if(ve_funkci) {
                    elem = ValTableSearch(&pomLocTable, old_ins->name);
                    if (elem != NULL){
                        if(elem->type == TYPEINT) {
                            UprInsert(new_list, X_PUSH_IDL_I, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEREAL) {
                            UprInsert(new_list, X_PUSH_IDL_R, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEBOOL) {
                            UprInsert(new_list, X_PUSH_IDL_B, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPESTRING) {
                            UprInsert(new_list, X_PUSH_IDL_S, TYPEID, &(elem->index),NULL);
                        }
                    break;
                    }
                }
                else{
                    elem = ValTableSearch(&pomGlobTable, old_ins->name);
                    if(elem->type == TYPEINT) {
                        UprInsert(new_list, X_PUSH_IDG_I, TYPEID, &(elem->index),NULL);
                    }
                    else if(elem->type == TYPEREAL) {
                        UprInsert(new_list, X_PUSH_IDG_R, TYPEID, &(elem->index),NULL);
                    }
                    else if(elem->type == TYPEBOOL) {
                        UprInsert(new_list, X_PUSH_IDG_B, TYPEID, &(elem->index),NULL);
                    }
                    else if(elem->type == TYPESTRING) {
                        UprInsert(new_list, X_PUSH_IDG_S, TYPEID, &(elem->index),NULL);
                    }
                }
            }
            break;
        case I_POP:
            if(ve_funkci) {
                    elem = ValTableSearch(&pomLocTable, old_ins->name);
                    if (elem != NULL){
                        if(elem->type == TYPEINT) {
                            UprInsert(new_list, X_POP_L_I, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEREAL) {
                            UprInsert(new_list, X_POP_L_R, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEBOOL) {
                            UprInsert(new_list, X_POP_L_B, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPESTRING) {
                            UprInsert(new_list, X_POP_L_S, TYPEID, &(elem->index),NULL);
                        }
                        break;
                    }
                }
                else{
                    elem = ValTableSearch(&pomGlobTable, old_ins->name);
                    if(elem->type == TYPEINT) {
                            UprInsert(new_list, X_POP_G_I, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEREAL) {
                            UprInsert(new_list, X_POP_G_R, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEBOOL) {
                            UprInsert(new_list, X_POP_G_B, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPESTRING) {
                            UprInsert(new_list, X_POP_G_S, TYPEID, &(elem->index),NULL);
                        }
                    break;
                }
            break;
        case I_CALL:
            if(strcmp(old_ins->name, "sort") == 0) {
                UprInsert(new_list, X_CALL_Sort, 0, NULL, NULL);
                break;
            }
            if(strcmp(old_ins->name, "copy") == 0) {
                UprInsert(new_list, X_CALL_Copy, 0, NULL, NULL);
                break;
            }
            if(strcmp(old_ins->name, "find") == 0) {
                UprInsert(new_list, X_CALL_Find, 0, NULL, NULL);
                break;
            }
            if(strcmp(old_ins->name, "length") == 0) {
                UprInsert(new_list, X_CALL_Leng, 0, NULL, NULL);
                break;
            }
            elem = ValTableSearch(&pomGlobTable, old_ins->name);
            UprInsert(new_list, X_CALL, TYPEFUNC, &(elem->f_pocet_lok_promennych), elem->f_params);
            new_list->Last->adr2 = (void*)&(elem->f_zacatek);
            break;
        case I_WRITE:
            UprInsert(new_list, X_WRITE, TYPEINT, old_ins->i, NULL);
            break;
        case I_READ:
            if(ve_funkci) {
                    elem = ValTableSearch(&pomLocTable, old_ins->name);
                    if (elem != NULL){
                        if(elem->type == TYPEINT) {
                            UprInsert(new_list, X_READ_L_I, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEREAL) {
                            UprInsert(new_list, X_READ_L_R, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEBOOL) {
                            UprInsert(new_list, X_READ_L_B, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPESTRING) {
                            UprInsert(new_list, X_READ_L_S, TYPEID, &(elem->index),NULL);
                        }
                        break;
                    }
                }
                else{
                    elem = ValTableSearch(&pomGlobTable, old_ins->name);
                    if(elem->type == TYPEINT) {
                            UprInsert(new_list, X_READ_G_I, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEREAL) {
                            UprInsert(new_list, X_READ_G_R, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPEBOOL) {
                            UprInsert(new_list, X_READ_G_B, TYPEID, &(elem->index),NULL);
                        }
                        else if(elem->type == TYPESTRING) {
                            UprInsert(new_list, X_READ_G_S, TYPEID, &(elem->index),NULL);
                        }
                    break;
                }
            break;
        case I_COND:
            old_ins = UprFromIns_begin_whileif(old_ins, new_list);
            break;
        case I_IF:
            return old_ins;
            break;
        case I_ELSE:
            return old_ins;
            break;
        case I_WHILE:
            return old_ins;
            break;
        case I_ADD:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_ADD_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_ADD_X_R;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_ADD_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_ADD_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_ADD_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_ADD_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_ADD_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_ADD_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_ADD_IDL_S;
            else new_list->Last->ins = X_ADD;
            break;
        case I_SUB:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_SUB_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_SUB_X_R;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_SUB_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_SUB_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_SUB_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_SUB_IDL_R;
            else new_list->Last->ins = X_SUB;
            break;
        case I_MUL:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_MUL_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_MUL_X_R;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_MUL_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_MUL_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_MUL_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_MUL_IDL_R;
            else new_list->Last->ins = X_MUL;
            break;
            break;
        case I_DIV:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_DIV_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_DIV_X_R;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_DIV_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_DIV_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_DIV_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_DIV_IDL_R;
            else new_list->Last->ins = X_DIV;
            break;
            break;
        case I_EQ:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_EQ_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_EQ_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_EQ_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_EQ_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_EQ_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_EQ_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_EQ_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_EQ_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_EQ_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_EQ_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_EQ_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_EQ_IDL_S;
            else new_list->Last->ins = X_EQ;
            break;
        case I_LS:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_LS_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_LS_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_LS_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_LS_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_LS_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_LS_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_LS_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_LS_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_LS_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_LS_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_LS_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_LS_IDL_S;
            else new_list->Last->ins = X_LS;
            break;
            break;
        case I_LSEQ:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_LSEQ_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_LSEQ_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_LSEQ_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_LSEQ_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_LSEQ_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_LSEQ_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_LSEQ_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_LSEQ_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_LSEQ_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_LSEQ_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_LSEQ_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_LSEQ_IDL_S;
            else new_list->Last->ins = X_LSEQ;
            break;
            break;
        case I_NOTEQ:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_NOTEQ_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_NOTEQ_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_NOTEQ_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_NOTEQ_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_NOTEQ_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_NOTEQ_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_NOTEQ_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_NOTEQ_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_NOTEQ_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_NOTEQ_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_NOTEQ_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_NOTEQ_IDL_S;
            else new_list->Last->ins = X_NOTEQ;
            break;
            break;
        case I_GR:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_GR_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_GR_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_GR_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_GR_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_GR_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_GR_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_GR_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_GR_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_GR_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_GR_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_GR_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_GR_IDL_S;
            else new_list->Last->ins = X_GR;
            break;
            break;
        case I_GREQ:
            if(new_list->Last->ins == X_PUSH_I)          new_list->Last->ins = X_GREQ_X_I;
            else if(new_list->Last->ins == X_PUSH_R)     new_list->Last->ins = X_GREQ_X_R;
            else if(new_list->Last->ins == X_PUSH_B)     new_list->Last->ins = X_GREQ_X_B;
            else if(new_list->Last->ins == X_PUSH_S)     new_list->Last->ins = X_GREQ_X_S;
            else if(new_list->Last->ins == X_PUSH_IDG_I) new_list->Last->ins = X_GREQ_IDG_I;
            else if(new_list->Last->ins == X_PUSH_IDG_R) new_list->Last->ins = X_GREQ_IDG_R;
            else if(new_list->Last->ins == X_PUSH_IDG_B) new_list->Last->ins = X_GREQ_IDG_B;
            else if(new_list->Last->ins == X_PUSH_IDG_S) new_list->Last->ins = X_GREQ_IDG_S;
            else if(new_list->Last->ins == X_PUSH_IDL_I) new_list->Last->ins = X_GREQ_IDL_I;
            else if(new_list->Last->ins == X_PUSH_IDL_R) new_list->Last->ins = X_GREQ_IDL_R;
            else if(new_list->Last->ins == X_PUSH_IDL_B) new_list->Last->ins = X_GREQ_IDL_B;
            else if(new_list->Last->ins == X_PUSH_IDL_S) new_list->Last->ins = X_GREQ_IDL_S;
            else new_list->Last->ins = X_GREQ;
            break;
            break;
        default:
            break;
        }
    }
    return NULL;
}

tInsElemPtr UprFromIns_function_locvar(tInsElemPtr old_ins){
    while(old_ins != NULL) {
        if(old_ins->ins == I_INT) {
            ValTableInsert(&pomLocTable, old_ins->name, TYPEINT);
        }
        else if(old_ins->ins == I_REAL) {
            ValTableInsert(&pomLocTable, old_ins->name, TYPEREAL);
        }
        else if(old_ins->ins == I_BOOL) {
            ValTableInsert(&pomLocTable, old_ins->name, TYPEBOOL);
        }
        else if(old_ins->ins == I_STRING) {
            ValTableInsert(&pomLocTable, old_ins->name, TYPESTRING);
        }
        else break;
        old_ins = old_ins->ptr;
    }
    return old_ins;
}

int UprFromIns_function_varcount(tInsElemPtr old_ins){
    int i = 0;
    tInsElemPtr pom_ins;

    pom_ins = old_ins;
    while(pom_ins != NULL) {
        if(pom_ins->ins == I_FUNC_ZAC){
            if(strcmp(pom_ins->name, old_ins->name) == 0) {
                pom_ins = pom_ins->ptr;
                while(pom_ins != NULL) {
                    if(pom_ins->ins == I_INT) {
                        i++;
                    }
                    else if(pom_ins->ins == I_REAL) {
                        i++;
                    }
                    else if(pom_ins->ins == I_BOOL) {
                        i++;
                    }
                    else if(pom_ins->ins == I_STRING) {
                        i++;
                    }
                    else return i;
                    pom_ins = pom_ins->ptr;
                }
                return i;
            }
        }
        pom_ins = pom_ins->ptr;
    }
    return i;
}


tInsElemPtr UprFromIns_function(tInsElemPtr old_ins, tUprIList *new_list){
    tValTableElem *elem_func;
    int i;
    char param[128];
    char pom_string[128];
    int j;
    int k;
    int l;

    while(old_ins != NULL) {
        if(old_ins->ins == I_FUNC_DEK) {
            param[0] = old_ins->s[0];
            j = 1;
            for(i = 1; old_ins->s[i]; i++){
                if(old_ins->s[i] == ':') {
                    i++;
                    param[j] = old_ins->s[i];
                    j++;        //v j je ulozeny pocet parametru + return value
                }
            }
            param[j] = '\0';
            elem_func = ValTableInsertFunc(&pomGlobTable, old_ins->name, old_ins->s[0]);
            k = UprFromIns_function_varcount(old_ins); //pocet lok prom;
            strcpy(elem_func->f_params, param);
            elem_func->f_pocet_lok_promennych = j + k;
        }
        else if(old_ins->ins == I_FUNC_ZAC) {
            ValTableInit(&pomLocTable);
            ve_funkci = 1;

            ValTableInsert(&pomLocTable, old_ins->name, old_ins->s[0]);
            elem_func = ValTableSearch(&pomGlobTable, old_ins->name);
            if(elem_func == NULL){
                param[0] = old_ins->s[0];
                j = 1;

                for(i = 1; old_ins->s[i]; i++){
                    l = 0;
                    if(old_ins->s[i] != ':' && old_ins->s[i] != ','){
                        for(; old_ins->s[i] != ':'; i++){
                            pom_string[l] = old_ins->s[i];
                            l++;
                        }
                        pom_string[l] = '\0';
                        i++;
                        param[j] = old_ins->s[i];
                        ValTableInsert(&pomLocTable, pom_string, param[j]);
                        j++;
                    }
                }
                param[j] = '\0';
                k = UprFromIns_function_varcount(old_ins); //pocet lok prom;
                elem_func = ValTableInsertFunc(&pomGlobTable, old_ins->name, old_ins->s[0]);
                strcpy(elem_func->f_params, param);
                elem_func->f_pocet_lok_promennych = j + k;
            }
            elem_func->f_zacatek = (void*)&(new_list->Last->ptr);
            old_ins = old_ins->ptr;
            old_ins = UprFromIns_function_locvar(old_ins);
            if(old_ins->ins == I_BEGIN) {
                old_ins = UprFromIns_begin(old_ins, new_list);
            }
            ve_funkci = 0;
            if(param[0] == TYPEINT) UprInsert(new_list, X_END_FUNC_I, 0, NULL, NULL);
            if(param[0] == TYPEREAL) UprInsert(new_list, X_END_FUNC_R, 0, NULL, NULL);
            if(param[0] == TYPEBOOL) UprInsert(new_list, X_END_FUNC_B, 0, NULL, NULL);
            if(param[0] == TYPESTRING) UprInsert(new_list, X_END_FUNC_S, 0, NULL, NULL);
            ValTableDispose(&pomLocTable);
        }
        else break;
        old_ins = old_ins->ptr;
    }
    return old_ins;
}




tInsElemPtr UprFromIns_globalvars(tInsElemPtr old_ins, int *vars){
    while(old_ins != NULL) {
        if(old_ins->ins == I_INT) {
            (*vars)++;
            ValTableInsert(&pomGlobTable, old_ins->name, TYPEINT);
        }
        else if(old_ins->ins == I_REAL) {
            (*vars)++;
            ValTableInsert(&pomGlobTable, old_ins->name, TYPEREAL);
        }
        else if(old_ins->ins == I_BOOL) {
            (*vars)++;
            ValTableInsert(&pomGlobTable, old_ins->name, TYPEBOOL);
        }
        else if(old_ins->ins == I_STRING) {
            (*vars)++;
            ValTableInsert(&pomGlobTable, old_ins->name, TYPESTRING);
        }
        else break;
        old_ins = old_ins->ptr;
    }
    return old_ins;
}


int UprFromIns(tIList *old_list, tUprIList *new_list){
    int i = 0;
    tInsElemPtr old_ins;

    ve_funkci = 0;
    old_ins = old_list->First;
    UprInsert(new_list, X_ZAC_KON, 0,NULL,NULL);

    old_ins = UprFromIns_globalvars(old_ins, &i);
    old_ins = UprFromIns_function(old_ins, new_list);
    ve_funkci = 0;
    new_list->begin_main = &(new_list->Last->ptr);
    old_ins = UprFromIns_begin(old_ins, new_list);
    UprInsert(new_list, X_ZAC_KON, 0,NULL,NULL);
    return i;
}
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


void run(tUprIList *new_list, int pocet_glob_prom){
    int i;
    int e;
    tUprData global_data[pocet_glob_prom];
    tUprData zasobnik[64];

    for(i = 0; i < pocet_glob_prom; i++){
        global_data[i].type = 0;
    }
    e = UprInterpret(*(new_list->begin_main), 0, "X", global_data, zasobnik, -1);
    if(errCode == E_OK) {
        errCode = -e;
    }
}

int interpret(tIList *old_list){
    int pocet_glob_promennych;
    tUprIList new_list;

    UprInit(&new_list);
    ValTableInit(&pomGlobTable);

    pocet_glob_promennych = UprFromIns(old_list, &new_list);

    {
       /*
    tUprInsElemPtr ins;
    for(ins = new_list.First; ins != NULL; ins = ins->ptr){
        printf("Instukce: %s\n", enn[ins->ins]);
        switch(ins->type){
        case TYPEINT:
            printf("  int: %i\n", ins->i);
            break;
        case TYPEREAL:
            printf("  fl: %g\n", ins->f);
            break;
        case TYPEBOOL:
            break;
        case TYPESTRING:
            printf("  str: %s\n", ins->str);
            break;
        case TYPEFUNC:
            printf("  str: %s\n", ins->str);
            printf("  par: %i\n", ins->i);
            break;
        case TYPEID:
            printf("  id:  %i\n", ins->i);
            break;
        }
        if(ins->adr != NULL)printf("  ins: %s\n", enn[(*(ins->adr))->ins]);
        if(ins->adr2 != NULL)printf("  in2: %s\n", enn[(**(ins->adr2))->ins]);
    } // */
    }

    run(&new_list, pocet_glob_promennych);

    ValTableDispose(&pomGlobTable);
    UprDispose(&new_list);
    return 0;
}

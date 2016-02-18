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
#include "ial.h"

int HTSIZE = MAX_HTSIZE;
int solved;
int hashCode ( tKey key ) {
	int i;
	int retval = 1;
	int keylen = strlen(key); //zde je delka klice
	for ( i=0; i<keylen; i++ )
		retval += tolower(key[i]);
	return ( retval % HTSIZE );
}

void htInit ( tHTable* ptrht ) {
    int i;
    if((*ptrht) == NULL)
    {
        return;
    }
    else
    {
        for (i = 0; i < HTSIZE; i++)
        {
            (*ptrht)[i] = NULL;
        }
    }
}

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

    if ((*ptrht) == NULL)
        return NULL;
    else
    {
        int index = hashCode(key);
        tHTItem *tmp = (*ptrht)[index];

        while(tmp != NULL)
        {
            if(strcmp(tmp->key, key) == 0)
            {
                return tmp;
            }

            tmp = tmp->ptrnext;
        }
        return NULL;
    }
}
void htInsert ( tHTable* ptrht, tKey key, tData data ) {

    if ((*ptrht) == NULL)
        return;
    int index = hashCode(key);
    tHTItem *tmp = htSearch(ptrht, key);
    if(tmp != NULL) //aktualizace
    {
        tmp->key = key;
        tmp->data = data;
    }
    else //vlozeni
    {
        if ((tmp = malloc (sizeof(struct tHTItem))) == NULL) //alokuju misto na heapu
        {
            return;
        }
        tmp->key = key; //vlozim do tmp key klic co mi byl zadan
        tmp->data = data; //a data
        tmp->ptrnext = (*ptrht)[index]; //prehodim ukazatel
        (*ptrht)[index] = tmp; //nahraju do pole prvek
    }
}


tData* htRead ( tHTable* ptrht, tKey key ) {
    if ((*ptrht) == NULL)
        return NULL;
    tHTItem *tmp = htSearch(ptrht, key);
    if(tmp != NULL)
        return &(tmp->data);
    else
        return NULL;

}

void htDelete ( tHTable* ptrht, tKey key ) {
    if ((*ptrht) == NULL) // kdyz tabulka neexistuje skonci
        return;
    int index = hashCode(key); //ziskej index pole
    tHTItem *tmp = (*ptrht)[index]; // ulozim si prvek do tmp
    tHTItem *tmp2 = NULL; //tmp2

    while (tmp != NULL) //pokud neni NULL (tzn existuje)
    {
        if(tmp->key == key) //a pokud klice se shoduji
        {
            if(tmp == ((*ptrht)[index])) //pokud je prvni
            {
                (*ptrht)[index] = tmp->ptrnext; //tak prvek pole ukazuje na nasledovnika
            }
            else
            {
                tmp2->ptrnext = tmp->ptrnext; //tmp2 ukazuje nyni na nasledovnika tmp
            }
            free(tmp); //uvolni tmp
            return;
        }
        tmp2 = tmp;
        tmp = tmp ->ptrnext;
    }
    return;

}

void htClearAll ( tHTable* ptrht ) {
    if ((*ptrht) == NULL)
        return;
    tHTItem *tmp = NULL;
    tHTItem *tmp2 = NULL;
    int index;
    for (index = 0; index < HTSIZE; index++)
    {
        tmp = (*ptrht)[index];
        (*ptrht)[index] = NULL;

        while (tmp != NULL)
        {
            tmp2 = tmp;
            tmp = tmp->ptrnext;
            if(strcmp("var", tmp2->data.value) == 0)
            {
                free(tmp2->data.localTable);
                free(tmp2->key);
                free(tmp2->data.type);
                free(tmp2);
            }
            else
            {
                free(tmp2->key);
                htClearAll(tmp2->data.localTable);
                free(tmp2->data.localTable);
                free(tmp2->data.type);
                free(tmp2);

            }
        }
    }
}


//------------------------------------------------------------------------------


void ComputeJumps(char *P, int CharJump[256]){
    unsigned char ch;
    int k;

    for(ch = 0 ; ch < 255 ; ch++){
        CharJump[ch] = strlen(P);
    }
    for(k = 0 ; k < strlen(P) ; k++){
        CharJump[(int)P[k]] = strlen(P) - k - 1;
    }
}

int BMAFind(char *P,char *T, int CharJump[256])
{
    int i;
    int j;
    int P_size = strlen(P);
    int T_sizej = strlen(T) - 1;

    if(T_sizej == -1) return 0;

    j = T_sizej;
    i = T_sizej;
    while(i < P_size)
    {
        while(P[i] == T[j]) {
            if(j == 0) {
                return i + 1;
            }
            j--;
            i--;
        }
        j = T_sizej;
        i = i + CharJump[(int)P[i]];
    }
    return 0;
}


void BoyerMoorAlogythm(char *str1, char *str2, int *ret) {
    int CharJump[256];
    ComputeJumps(str2, CharJump);
    *ret = BMAFind(str1, str2, CharJump);
}




void parririon(char *A, int left, int right, int *i, int *j) {
    int PM;
    *i = left;
    *j = right;
    PM = A[(*i+*j) / 2];
    do {
        while(A[*i] < PM) (*i)++;
        while(A[*j] > PM) (*j)--;
        if(*i <= *j) {
            if(*i != *j){
                A[*i] = A[*j] - A[*i];
                A[*j] = A[*j] - A[*i];
                A[*i] = A[*j] + A[*i];
            }
            (*i)++;
            (*j)--;
        }
    } while(*i <= *j);
}

void QuickSort(char *A, int left, int right){
    int i;
    int j;
    parririon(A, left, right, &i, &j);
    if(left < j)  QuickSort(A, left, j);
    if(i < right) QuickSort(A, i, right);
}








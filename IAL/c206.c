
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu,
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek,
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku,
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu,
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem,
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu,
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free.
**/
    tDLElemPtr pomUk;
    while(L->First != NULL)
    {
        pomUk = L->First;
        L->First = L->First->rptr;
        free(pomUk);
    }
    L->Act = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
    tDLElemPtr newElement;
    if((newElement = malloc(sizeof(struct tDLElem))) == NULL)
    {
        DLError();
    }
    newElement->data = val;
    newElement->lptr = NULL;
    newElement->rptr = L->First;
    if(L->First != NULL)
    {
        L->First->lptr = newElement;
    }
    else
    {
        L->Last = newElement;
    }
    L->First = newElement;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
    tDLElemPtr newElement;
    if((newElement = malloc(sizeof(struct tDLElem))) == NULL)
    {
        DLError();
    }
    newElement->data = val;
    newElement->rptr = NULL;
    newElement->lptr = L->Last;
    if(L->Last != NULL)
    {
        L->Last->rptr = newElement;
    }
    else
    {
        L->First = newElement;
    }
    L->Last = newElement;
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
    if(L->First == NULL) //kdyz je first null tak je seznam pradzdny
    {
        DLError();
    }
    else
    {
        *val = L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
    if(L->First == NULL) //kdyz je first null tak je seznam pradzdny
    {
        DLError();
    }
    else
    {
        *val = L->Last->data;
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
    tDLElemPtr pomUk;
    if(L->First == NULL) //kdyz je first null tak je seznam pradzdny
    {
        return;
    }
    else
    {
        pomUk = L->First;
        if(L->Act == L->First)
        {
            L->Act = NULL;
        }
        if(L->First == L->Last)
        {
            L->First = NULL;
            L->Last = NULL;
        }
        else
        {
            L->First = pomUk->rptr;
            L->First->lptr = NULL;
        }
        free(pomUk);
    }
}

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
    tDLElemPtr pomUk;
    if(L->First == NULL) //kdyz je first null tak je seznam pradzdny
    {
        return;
    }
    else
    {
        pomUk = L->Last;
        if(L->Act == L->Last)
        {
            L->Act = NULL;
        }
        if(L->First == L->Last)
        {
            L->First = NULL;
            L->Last = NULL;
        }
        else
        {
            L->Last = pomUk->lptr;
            L->Last->rptr = NULL;
        }
        free(pomUk);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
    if((L->Act == NULL) || (L->Act == L->Last) )
    {
        return;
    }
    else
    {
        tDLElemPtr pomUk;
        pomUk = L->Act->rptr;
        L->Act->rptr = L->Act->rptr->rptr;
        if(L->Act->rptr == NULL)
        {
            L->Last = L->Act;
        }
        free(pomUk);
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
    if((L->Act == NULL) || (L->Act == L->First) )
    {
        return;
    }
    else
    {
        tDLElemPtr pomUk;
        pomUk = L->Act->lptr;
        L->Act->lptr = L->Act->lptr->lptr;
        if(L->Act->lptr == NULL)
        {
            L->First = L->Act;
        }
        free(pomUk);
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
    if(L->Act == NULL)
    {
        return;
    }
    else
    {
        tDLElemPtr newElement;
        if((newElement = malloc(sizeof(struct tDLElem))) == NULL)
        {
            DLError();
        }
        newElement->data = val;
        if(L->Act->rptr == NULL) //act je last prvek
        {
            newElement->rptr = NULL;
            newElement->lptr = L->Act;
            L->Act->rptr = newElement;
            L->Last = newElement;
        }
        else
        {
            newElement->rptr = L->Act->rptr;
            newElement->lptr = L->Act;
            L->Act->rptr = newElement;
            newElement->rptr->lptr = newElement;
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
    if(L->Act == NULL)
    {
        return;
    }
    else
    {
        tDLElemPtr newElement;
        if((newElement = malloc(sizeof(struct tDLElem))) == NULL)
        {
            DLError();
        }
        newElement->data = val;
        if(L->Act->lptr == NULL) //act je first prvek
        {
            newElement->lptr = NULL;
            newElement->rptr = L->Act;
            L->Act->lptr = newElement;
            L->First = newElement;
        }
        else
        {
            newElement->lptr = L->Act->lptr;
            newElement->rptr = L->Act;
            L->Act->lptr = newElement;
            newElement->lptr->rptr = newElement;
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
    if(L->Act == NULL)
    {
        DLError();
    }
    else
    {
        *val = L->Act->data;
    }
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
    if(L->Act == NULL)
    {
        return;
    }
    else
    {
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
    if(L->Act == NULL)
    {
        return;
    }
    else
    {
        L->Act = L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
    if(L->Act == NULL)
    {
        return;
    }
    else
    {
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
    return (L->Act != NULL);
}

/* Konec c206.c*/

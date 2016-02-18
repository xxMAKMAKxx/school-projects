
/* ******************************* c203.c *********************************** */
/*  Pedmt: Algoritmy (IAL) - FIT VUT v Brn                                 */
/*  kol: c203 - Fronta znak v poli                                          */
/*  Referenn implementace: Petr Pikryl, 1994                               */
/*  Pepis do jazyka C: Vclav Topinka, z 2005                             */
/*  pravy: Bohuslav Kena, jen 2014                                        */
/* ************************************************************************** */
/*
** Implementujte frontu znak v poli. Pesnou definici typ naleznete
** v hlavikovm souboru c203.h (ADT fronta je reprezentovna strukturou tQueue,
** kter obsahuje pole 'arr' pro uloen hodnot ve front a indexy f_index
** a b_index. Vechny implementovan funkce mus pedpokldat velikost pole
** QUEUE_SIZE, i kdy ve skutenosti jsou rozmry statickho pole definovny
** MAX_QUEUE. Hodnota QUEUE_SIZE slou k simulaci fronty v rzn velkm poli
** a nastavuje se v testovacm skriptu c203-test.c ped testovnm
** implementovanch funkc. Hodnota QUEUE_SIZE me nabvat hodnot v rozsahu
** 1 a MAX_QUEUE.
**
** Index f_index ukazuje vdy na prvn prvek ve front. Index b_index
** ukazuje na prvn voln prvek ve front. Pokud je fronta przdn, ukazuj
** oba indexy na stejn msto. Po inicializaci ukazuj na prvn prvek pole,
** obsahuj tedy hodnotu 0. Z uvedench pravidel vyplv, e v poli je vdy
** minimln jeden prvek nevyuit.
**
** Pi libovoln operaci se dn z index (f_index i b_index) nesniuje
** vyjma ppadu, kdy index peshne hranici QUEUE_SIZE. V tom ppad
** se "posunuje" znovu na zatek pole. Za tmto elem budete deklarovat
** pomocnou funkci NextIndex, kter pro kruhov pohyb pes indexy pole
** vyuv operaci "modulo".
**
** Implementujte nsledujc funkce:
**
**    nextIndex ..... pomocn funkce - viz popis ve
**    queueInit ..... inicializace fronty
**    queueEmpty .... test na przdnost fronty
**    queueFull ..... test, zda je fronta zaplnna (vyerpna kapacita)
**    queueFront .... pete hodnoty prvnho prvku z fronty
**    queueRemove ... odstran prvn prvek fronty
**    queueGet ...... pete a odstran prvn prvek fronty
**    queueUp ....... zaazen prvku na konec fronty
**
** Sv een eln komentujte!
**
** Terminologick poznmka: Jazyk C nepouv pojem procedura.
** Proto zde pouvme pojem funkce i pro operace, kter by byly
** v algoritmickm jazyce Pascalovskho typu implemenovny jako
** procedury (v jazyce C procedurm odpovdaj funkce vracejc typ void).
**
**/

#include "c203.h"

void queueError (int error_code) {
/*
** Vytiskne upozornn na to, e dolo k chyb.
** Tato funkce bude volna z nkterch dle implementovanch operac.
**
** TUTO FUNKCI, PROSME, NEUPRAVUJTE!
*/
	static const char* QERR_STRINGS[MAX_QERR+1] = {"Unknown error","Queue error: UP","Queue error: FRONT","Queue error: REMOVE","Queue error: GET","Queue error: INIT"};
	if ( error_code <= 0 || error_code > MAX_QERR )
		error_code = 0;
	printf ( "%s\n", QERR_STRINGS[error_code] );
	err_flag = 1;
}

void queueInit (tQueue* q) {
/*
** Inicializujte frontu nsledujcm zpsobem:
** - vechny hodnoty v poli q->arr nastavte na '*',
** - index na zatek fronty nastavte na 0,
** - index prvnho volnho msta nastavte tak na 0.
**
** V ppad, e funkce dostane jako parametr q == NULL, volejte funkci
** queueError(QERR_INIT).
*/
    if(q == NULL)
    {
        queueError(QERR_INIT);
    }
    else
    {
        int i;
        q->f_index = 0;
        q->b_index = 0;
        for (i = 0; i < (QUEUE_SIZE) ; i++)
        {
            q->arr[i] = '*';
        }
    }
}

int nextIndex (int index) {
/*
** Pomocn funkce, kter vrac index nsledujcho prvku v poli.
** Funkci implementujte jako jedin prikaz vyuvajc operace '%'.
** Funkci nextIndex budete vyuvat v dalch implementovanch funkcch.
*/
    return ((index+1)%(QUEUE_SIZE));
}

int queueEmpty (const tQueue* q) {
/*
** Vrac nenulovou hodnotu, pokud je frona przdn, jinak vrac hodnotu 0.
** Funkci implementujte jako jedin pkaz.
*/
    return (q->f_index == q->b_index);
}

int queueFull (const tQueue* q) {
/*
** Vrac nenulovou hodnotu, je-li fronra pln, jinak vrac hodnotu 0.
** Funkci implementujte jako jedin pkaz s vyuitm pomocn funkce nextIndex.
*/
    return (nextIndex(q->b_index) == q->f_index);
}

void queueFront (const tQueue* q, char* c) {
/*
** Prostednictvm parametru c crt znak ze zatku fronty q.
** Pokud je fronta przdn, oetete to volnm funkce queueError(QERR_FRONT).
** Voln tto funkce pi przdn front je vdy nutn povaovat za nekorektn.
** Bv to toti dsledek patnho nvrhu algoritmu, ve kterm je fronta
** pouita. O takov situaci se proto mus programtor-vvoj dozvdt.
** V opanm ppad je ladn program obtnj!
**
** Pi implementaci vyuijte dve definovan funkce queueEmpty.
*/
    if(queueEmpty(q) == 0)
    {
        *c = q->arr[q->f_index];
    }
    else
    {
        queueError(QERR_FRONT);
    }
}

void queueRemove (tQueue* q) {
/*
** Odstran znak ze zatku fronty q. Pokud je fronta przdn, oetete
** vzniklou chybu volnm funkce queueError(QERR_REMOVE).
** Hodnotu na uvolnn pozici ve front nijak neoetujte (nepepisujte).
** Pi implementaci vyuijte dve definovan funkce queueEmpty a nextIndex.
*/
    if(queueEmpty(q) == 0)
    {
        q->f_index = nextIndex(q->f_index);
    }
    else
    {
        queueError(QERR_REMOVE);
    }
}

void queueGet (tQueue* q, char* c) {
/*
** Odstran znak ze zatku fronty a vrt ho prostednictvm parametru c.
** Pokud je fronta przdn, oetete to volnm funkce queueError(QERR_GET).
**
** Pi implementaci vyuijte dve definovanch funkc queueEmpty,
** queueFront a queueRemove.
*/
    if(queueEmpty(q) == 0)
    {
        queueFront(q, c);
        queueRemove(q);
    }
    else
    {
        queueError(QERR_GET);
    }
}

void queueUp (tQueue* q, char c) {
/*
** Vlo znak c do fronty. Pokud je fronta pln, oetete chybu volnm
** funkce queueError(QERR_UP). Vkldn do pln fronty se povauje za
** nekorektn operaci. Situace by mohla bt eena i tak, e by operace
** neprovdla nic, ale v ppad pouit takto definovan abstrakce by se
** obtn odhalovaly chyby v algoritmech, kter by abstrakci vyuvaly.
**
** Pi implementaci vyuijte dve definovanch funkc queueFull a nextIndex.
*/
    if(queueFull(q) == 0)
    {
        q->arr[q->b_index] = c;
        q->b_index = nextIndex(q->b_index);
    }
    else
    {
        queueError(QERR_UP);
    }

}
/* Konec pkladu c203.c */

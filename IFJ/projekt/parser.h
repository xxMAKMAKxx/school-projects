/* *********************Syntakticky analyzator****************************
 * Subor:      parser.c
 * Datum:      19.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"



typedef enum {
  E_OK = 0,     //OK
  E_LEX,        //1chyba v ramci lexikalnej analyzy
  E_SYN,        //2chyba v ramci syntaktickej analyzy
  E_SEMP,       //3semanticka chyba - nedefinovana premenna/funkcia
  E_SEMF,       //4semanticka chyba - typova kompatibilita, zly pocet parametrov
  E_SEM,        //5ostatne semanticke chyby
  E_READ,       //6behova chyba pri nacitani zo vstupu
  E_NIID,       //7behova chyba pri praci s neinicializovanou premennnou
  E_NUL,        //8behova chyba pri deleni nulou
  E_BEH,        //9ostatne behove chyby
  E_ERR = 99,   //99chyba interpretu
}tError;

/*typedef struct
{
    char *Ins_string;
    int Ins_int;
    int Ins_bool;
    float Ins_folat;
    char *Ins_name;
} tIns_var;*/

tError control();
int zacatek();
int var_def(int from, tHTable* localTable2);
int func_def();
int after_var();
int isStrednik();
int type(char *localString, int fromParams,char *nazevFunkce);
int term(int *localState,char *localString);
int var_func(tHTable* localTable2);
int after_stat();
int after_end();
int st_list();
int par_list(char *localString);
int param_n(char *localString);
int after_func(tHTable* localTable2,int *fromForward);
int next_func();
int stat();
int after_if();
int expresion();
int term_list();
int term_n();
int isBegin();
int isTF();
int isDo();
int isEnd();
int isThen();
int prec_analyza();
int func_prikazy();
void addCharParser(char *char1);
void addLocalCharParser(char *char1, char *localString);
void vypisCelouTabulku();
#endif

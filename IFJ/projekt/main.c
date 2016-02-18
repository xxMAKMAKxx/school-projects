/* *********************Main funkce****************************
 * Subor:      main.c
 * Datum:      19.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/

#include "parser.h"
#include "scanner.h"

tError mainErrCode;

int main (int argc, char **argv)
{
    char *cesta;
    if(argc != 2)
    {
        mainErrCode = E_ERR;
    }
    else
    {
        cesta = argv[1];
        mainErrCode = control(cesta);
    }
    return mainErrCode;
}

/* *************************KLUCOVE SLOVA******************************
 * Subor:      keyWords.c
 * Datum:      12.10.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "keywords.h"

const char* keyWords[KEY_WORDS_COUNT] = {
    "begin", "boolean", "do", "else", "end", "false", "find", "forward",
    "function", "if", "integer", "readln", "real", "sort", "string",
    "then", "true", "var", "while", "write", "length", "copy"
};

int isKeyWord(tToken token) {
    int count = 0;

    while (count != KEY_WORDS_COUNT) {
        if (strcmp(keyWords[count], token.name) == 0) {
            return 1;
        }
        count++;
    }
    return 0;
}

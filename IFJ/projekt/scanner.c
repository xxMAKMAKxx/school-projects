/* *********************LEXIKALNY ANALYZATOR****************************
 * Subor:      scanner.c
 * Datum:      12.10.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
#include "scanner.h"
#include "parser.h"

tToken token;
char *string;
int count1, count2, count3, escNum, flag;
char esc[3];
extern tError errCode;

// pridanie dat do tokenu
void dataToToken(tState state, char *name) {
    token.state = state;
    token.name = name;
}

/* alokacia 127 znakov pre premennu/cislo - volana v parseri pred
 * prvym volanim funkcie getNextToken */
void allocScanner() {
    if ((string = malloc(sizeof(char) * 128)) == NULL) {
        if (errCode == 0) {
            errCode = E_ERR;
        }
    }
    flag = 0;
}

/* uvolnenie pamati - volane v parseri po poslednom volani
 * funkcie getNextToken */
void freeScanner() {
    free(string);
}

// inicializacia tokenu, pomocnych premennych
void init() {
    if (flag != 0) {
        free(string);
        if ((string = malloc(sizeof(char) * 128)) == NULL) {
            if (errCode == 0) {
                errCode = E_ERR;
            }
        }
        flag = 0;
    }
    dataToToken(S_START, NULL);
    count1 = 0, count2 = 0, count3 = 0, escNum = 0;
    string[count1] = '\0';
}

// pridanie znaku zo suboru do premennej/cisla
void addChar(char char1) {
    // max dlzka premennej je 127 platnych znakov
    if (count1 != 127) {
        string[count1] = tolower(char1);
        count1++;
        string[count1] = '\0';
    }
}

// pridanie znaku zo suboru do retazca
void addChar2(char char1) {
    // 128 charov je alokovanych
    if (count1 < 127) {
        string[count1] = (char1);
        count1++;
        string[count1] = '\0';
        flag = 0;
    }
    // ak je retazec dlhsi ako 127 znakov, realloc
    else {
        count2 = strlen(string);
        if ((string = realloc(string, (count2 + 2) * sizeof(char))) == NULL) {
            if (errCode == 0) {
                errCode = E_ERR;
            }
            return;
        }
        else {
            string[count2] = char1;
            count2++;
            string[count2] = '\0';
            if (flag == 0) {
                flag = 1;
            }
        }
    }
}

// pridanie znaku do escape sekvencie
void addEsc(char char1) {
    if (count3 == 3) {
        if (errCode == 0) {
            errCode = E_LEX;
        }
        return;
    }
    if ((count3 == 0) && (char1 == '0')) {
        return;
    }
    esc[count3] = char1;
    count3++;
}

// hlavna funkcia
tToken getNextToken(FILE *fp) {
    init();
    if (errCode == E_ERR) {
        dataToToken(S_FALSE, NULL);
        return token;
    }
    char fileChar;
    while (1) {
        fileChar = fgetc(fp);
        switch (token.state) {

        // pociatocny stav
        case S_START:
            // test EOF
            if (fileChar == EOF) {
                token.state = S_EOF;
                break;
            }
            // test na biele znaky
            if (isspace(fileChar)) {
                token.state = S_START;
                break;
            }
            // jednoznakove tokeny
            if (fileChar == '.') {
                dataToToken(S_PER, NULL);
                return token;
            }
            if (fileChar == ',') {
                dataToToken(S_COM, NULL);
                return token;
            }
            if (fileChar == ';') {
                dataToToken(S_SEM, NULL);
                return token;
            }
            if (fileChar == '(') {
                dataToToken(S_LPAR, NULL);
                return token;
            }
            if (fileChar == ')') {
                dataToToken(S_RPAR, NULL);
                return token;
            }
            if (fileChar == '+') {
                dataToToken(S_ADD, NULL);
                return token;
            }
            if (fileChar == '-') {
                dataToToken(S_SUB, NULL);
                return token;
            }
            if (fileChar == '*') {
                dataToToken(S_MUL, NULL);
                return token;
            }
            if (fileChar == '/') {
                dataToToken(S_DIV, NULL);
                return token;
            }
            if (fileChar == '=') {
                dataToToken(S_EQ, NULL);
                return token;
            }
            // test na priradenie, operatory
            if (fileChar == ':') {
                token.state = S_ASGN;
                break;
            }
            if (fileChar == '<') {
                token.state = S_NOTEQ;
                break;
            }
            if (fileChar == '>') {
                token.state = S_GREQ;
                break;
            }
            // test na identifikatory
            if (isalpha(fileChar) || (fileChar == '_')) {
                addChar(fileChar);
                token.state = S_ID;
                break;
            }
            // test na cisla
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_INT;
                break;
            }
            // test na retazec
            if (fileChar == '\'') {
                token.state = S_STRHELP1;
                break;
            }
            // test na komentar
            if (fileChar == '{') {
                token.state = S_CMNT;
                break;
            }
            // vsetky ostatne znaky su chybne
            dataToToken(S_FALSE, NULL);
            if (errCode == 0) {
                errCode = E_LEX;
            }
            return token;

        // priradenie, dvojbodka
        case S_ASGN:
            if (fileChar == '=') {
                dataToToken(S_ASGN, NULL);
                return token;
            }
            else {
                ungetc(fileChar, fp);
                dataToToken(S_COL, NULL);
                return token;
            }

        // operatory <, <>, <=
        case S_NOTEQ:
            if (fileChar == '>') {
                dataToToken(S_NOTEQ, NULL);
                return token;
            }
            else {
                if (fileChar == '=') {
                    dataToToken(S_LSEQ, NULL);
                    return token;
                }
                else {
                    ungetc(fileChar, fp);
                    dataToToken(S_LS, NULL);
                    return token;
                }
            }

        // operatory >, >=
        case S_GREQ:
            if (fileChar == '=') {
                dataToToken(S_GREQ, NULL);
                return token;
            }
            else {
                ungetc(fileChar, fp);
                dataToToken(S_GR, NULL);
                return token;
            }

        // identifikatory
        case S_ID:
            if (isalnum(fileChar) || (fileChar == '_')) {
                addChar(fileChar);
                token.state = S_ID;
                break;
            }
            else {
                ungetc(fileChar, fp);
                dataToToken(S_ID, string);
                return token;
            }

        // cele cisla
        case S_INT:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_INT;
                break;
            }
            else {
                if (fileChar == '.') {
                    addChar(fileChar);
                    token.state = S_NUMHELP1; // 121.
                    break;
                }
                else {
                    if ((fileChar == 'e') || (fileChar == 'E')) {
                        addChar(fileChar);
                        token.state = S_NUMHELP2; // 121E
                        break;
                    }
                    else {
                        ungetc(fileChar, fp);
                        dataToToken(S_INT, string);
                        return token;
                    }
                }
            }

        // 121.
        case S_NUMHELP1:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_REAL1;
                break;
            }
            else {
                dataToToken(S_FALSE, NULL);
                if (errCode == 0) {
                    errCode = E_LEX;
                }
                return token;
            }

        // 121.2
        case S_REAL1:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_REAL1;
                break;
            }
            else {
                if ((fileChar == 'e') || (fileChar == 'E')) {
                    addChar(fileChar);
                    token.state = S_NUMHELP2; // 123.2e
                    break;
                }
                else {
                    ungetc(fileChar, fp);
                    dataToToken(S_REAL1, string);
                    return token;
                }
            }

        // 123.2e, 122e
        case S_NUMHELP2:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_REAL2; // 123.2e5, 10e4
                break;
            }
            else {
                if ((fileChar == '+') || (fileChar == '-')) {
                    addChar(fileChar);
                    token.state = S_NUMHELP3; // 123.2e-, 54.1e+
                    break;
                }
                else {
                    dataToToken(S_FALSE, NULL);
                    if (errCode == 0) {
                        errCode = E_LEX;
                    }
                    return token;
                }
            }

        // 121.2e-, 13.24e+
        case S_NUMHELP3:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_REAL2;
                break;
            }
            else {
                dataToToken(S_FALSE, NULL);
                if (errCode == 0) {
                    errCode = E_LEX;
                }
                return token;
            }

        // 121.2e-4, 125e41
        case S_REAL2:
            if (isdigit(fileChar)) {
                addChar(fileChar);
                token.state = S_REAL2;
                break;
            }
            else {
                ungetc(fileChar, fp);
                dataToToken(S_REAL2, string);
                return token;
            }

        // retazec
        case S_STRHELP1:
            if (fileChar == '\'') {
                token.state = S_STR;
                break;
            }
            else {
                if ((fileChar == 10) || (fileChar == EOF)) {
                    dataToToken(S_FALSE, NULL);
                    if (errCode == 0) {
                        errCode = E_LEX;
                    }
                    return token;
                }
                else {
                    token.state = S_STRHELP1;
                    addChar2(fileChar);
                    if (errCode == E_ERR) {
                        dataToToken(S_FALSE, NULL);
                        return token;
                    }
                    break;
                }
            }

        // retazec
        case S_STR:
            if (fileChar == '#') {
                token.state = S_STRHELP2;
                break;
            }
            else {
                if (fileChar == '\'') {
                    token.state = S_STRHELP1;
                    addChar2(fileChar);
                    if (errCode == E_ERR) {
                        dataToToken(S_FALSE, NULL);
                        return token;
                    }
                    break;
                }
                else {
                    ungetc(fileChar, fp);
                    dataToToken(S_STR, string);
                    return token;
                }
            }

        // retazec
        case S_STRHELP2:
            if (isdigit(fileChar)) {
                token.state = S_STRHELP3;
                addEsc(fileChar);
                break;
            }
            else {
                dataToToken(S_FALSE, NULL);
                if (errCode == 0) {
                    errCode = E_LEX;
                }
                return token;
            }

        // retazec
        case S_STRHELP3:
            if (isdigit(fileChar)) {
                token.state = S_STRHELP3;
                addEsc(fileChar);
                if (errCode != 0) {
                    dataToToken(S_FALSE, NULL);
                    return token;
                }
                break;
            }
            else {
                if (fileChar == '\'') {
                    token.state = S_STRHELP1;
                    escNum = atoi(esc);
                    count3 = 0;
                    if (escNum <= 255) {
                        fileChar = escNum;
                        addChar2(fileChar);
                        if (errCode == E_ERR) {
                            dataToToken(S_FALSE, NULL);
                            return token;
                        }
                    }
                    else {
                        dataToToken(S_FALSE, NULL);
                        if (errCode == 0) {
                            errCode = E_LEX;
                        }
                        return token;
                    }
                    break;
                }
                else {
                    dataToToken(S_FALSE, NULL);
                    if (errCode == 0) {
                        errCode = E_LEX;
                    }
                    return token;
                }
            }

        // komentar - ignoruje znaky medzi {} a nastavi stav na start
        case S_CMNT:
            if (fileChar == '}') {
                token.state = S_START;
                break;
            }
            if (fileChar == EOF) {
                dataToToken(S_FALSE, NULL);
                if (errCode == 0) {
                    errCode = E_LEX;
                }
                return token;
            }
            break;

        // koniec suboru
        case S_EOF:
            dataToToken(S_EOF, NULL);
            return token;

        default:
            break;
        } // switch
    } // while
}

/* *********************Syntakticky analyzator***************************
 * Subor:      parser.c
 * Datum:      19.11.2014
 * Predmet:    Formalne jazyky a prekladace (IFJ)
 * Projekt:    Implementacia interpretu jazyka IFJ14
 * Autori:     Martin Kacmarcik xkacma00
 *             Roman Nedelka    xnedel07
 *             Natalia Jurdova  xjurdo00
 *             Marek Skalnik    xskaln03
 * ********************************************************************/
/* ***********************!!! VZOR !!! *********************************
 int zacatek ()
 {
 int zacatek ()
 {
    int boolean = 0;
    if (strcmp (currToken.name, "var") == 0)
    {
        currToken = getNextToken(file);
        boolean = ( var_def() * after_var() );
    }
    else if(strcmp(currToken.name, "function") == 0)
    {
        currToken = getNextToken(file);
        boolean = func_def
    }
    else if(strcmp(currToken.name, "begin") == 0)
    {
        currToken = getNextToken(file);
        boolean = st_list();
    }
    return boolean;
 }
 }

 printf("Token (mel by byt id token): %s %d \n", currToken.name, currToken.state);

 * ********************************************************************/
 #include "parser.h"
 #include "scanner.h"
 #include "keywords.h"
 #include "precedence.h"
 #include "ial.h"
 #include "instruction_list.h"
 #include "interpret.h"

 tToken currToken; //dulezita promena obsahujici aktualni token
 tToken prevToken; //prevToken je pomocna promena pro prec. analyzu ve chvili kdy nevim jestli zpracovavam ja ci ona
 FILE *file; //promena pro ulozeni souboru se zdrojakem
 tError errCode; //promena obsahujici 1. chybovy stav
 tData data; //data pro tabulku symbolu
 tHTable* globalTable; //tabulka symbolu
 tHTable* globalLocalTable = NULL;
 tHTItem* stored = NULL; //pomocna promena pro testovani tabulky symbolu
 char stringos[256]; //string pro tabulku symbolu v pripade vlozeni funkce
 char instructionString[128];
 int count123 = 3; //pomocny counter pro addchar
 int fromParams = 0;
 int pushCount = 0;
 char globalFunctionName[128];
 char globalString [256];
 char prevTokenString[128];
 char *globalControlFunctionName = NULL;
 int parCount = 0;
 int inFunction = 0;
 HashType finish; //&finish
 //tIns_var TInstr_var;
 tIList Instr_list;
 tError control(char *nazevSouboru)
 {
    //Inicializace mych globalnich promennych
    errCode = E_OK;
    if ((file = fopen(nazevSouboru, "r")) == NULL) //otevri file, ktery uzivatel zadal pres argument
    {
        if(errCode == 0) //kdyz neotevres a errCode je nula hod chybu 99
        {
            errCode = E_ERR;
            return errCode;
        }
    }
    //Inicializace tabulky
    if ( (globalTable = (tHTable*) malloc ( sizeof(tHTable) )) == NULL)
    {
        fprintf(stderr, "Chyba alokace!\n");
        if(errCode == 0)
            errCode = E_ERR;
    }

    //ZACATEK INIT SEKCE
    allocScanner();
    htInit(globalTable);
    IListInit (&Instr_list);
    prevToken.name = NULL;
    prevToken.state = S_FALSE;
    //KONEC INIT SEKCE

    //Konec inicialiazce tabulky
    currToken = getNextToken(file);
    if(currToken.state == S_FALSE && errCode == E_OK) //errCode od lexera
    {
        errCode = E_LEX;
        freeScanner();
        return errCode;
    }
    prevToken.state = S_FALSE; //inicializace pomocneho tokenu
    prevToken.name = NULL; //inicializace pomocneho tokenu

    //Konec inicializace globalnich promenych
    int boolean = zacatek();



    //printf("Obsah souboru je syntakticky: %d\n", boolean);


    //vypisCelouTabulku(globalTable);


    /*stored = htSearch(globalTable, "funkce");

    if(stored->data.localTable == NULL)
    {
        printf("Tabulka je NULL\n");
    }
    else
    {
        vypisCelouTabulku(stored->data.localTable);
    }

    stored = htSearch(globalTable, "funkce2");

    vypisCelouTabulku(stored->data.localTable);*/



    if( boolean == 0 && errCode == E_OK) //nastav syntaktickou chybu jen pokud predtim nedosla zadna jina
    {
        errCode = E_SYN; //nastaveni indikatoru ze sme nasli syntakt. chybu
        //sekce vycisteni pameti, volani free
        htClearAll(globalTable);
        free(globalTable);
        fclose(file);
        IListDispose (&Instr_list);
        freeScanner();
        //konec sekce vycisteni pameti
        return errCode;
    }
    else if(errCode != E_OK)
    {
        //sekce vycisteni pameti, volani free
        htClearAll(globalTable);
        free(globalTable);
        fclose(file);
        IListDispose (&Instr_list);
        freeScanner();
        //konec sekce vycisteni pameti
        return errCode;
    }

    /*TESTOVANI INTERPRETU
    tInsElemPtr ins;
    for(ins = FirstInstruction(&Instr_list);ins != NULL; NextInstruction(&ins)) {
        printf("Instrukce: %i,", ins->ins);
        if (ins->i != NULL) printf(" i: %i", *(ins->i));
        else printf(" i: --");
        if (ins->r != NULL) printf(" f: %f", *(ins->r));
        else printf(" r: --");
        if (ins->s != NULL) printf(" s: %s", (ins->s));
        else printf(" s: --");
        if (ins->name != NULL) printf(" id: %s\n", (ins->name));
        else printf(" s: --\n");
    }
    //KONEC TESTOVANI INTERPRETU*/

    interpret(&Instr_list);
    //printf("Returned value je: %d\n", returnedValue);
    //printf("\n");

    //sekce vycisteni pameti, volani free
    htClearAll(globalTable);
    free(globalTable);
    fclose(file);
    IListDispose (&Instr_list);
    freeScanner();
    //konec sekce vycisteni pameti

    return errCode;
 }

 int isStrednik()
 {
    if( currToken.state == S_SEM)
    {
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }

 int zacatek ()
 {
    int boolean = 0;
    if ((currToken.name != NULL && strcmp (currToken.name, "var") == 0))
    {
        currToken = getNextToken(file);
        boolean = ( var_def(1, NULL) * after_var() );
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "function") == 0))
    {
        currToken = getNextToken(file);
        boolean = func_def();
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        if((currToken.name != NULL && strcmp(currToken.name, "end") == 0))
        {
            IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
            currToken = getNextToken(file);
            if(currToken.state == S_PER)
            {
                currToken = getNextToken(file);
                if(currToken.state == S_EOF)
                    boolean = 1;
            }

        }
        else
            boolean = st_list();
    }
    return boolean;
 }

 int var_def(int from, tHTable* localTable2) //1 je z varu a 2 je z function varu
 {
    int boolean = 0;
    char *variableName;
    char *variableType;
    if (currToken.state == S_ID && isKeyWord(currToken) == 0)
    {
        //alokace nove pameti pro jmeno promene do taublky sym
        int velikost = strlen(currToken.name);
        if ( (variableName = malloc(velikost+1)) == NULL )
        {
            if(errCode == 0)
            {
                fprintf(stderr, "Chyba alokace!\n");
                errCode = E_ERR;
            }
        }
        strcpy(variableName, currToken.name);
        //KONEC alokace nove pameti pro jmeno promene do taublky sym
        currToken = getNextToken(file);
        if (currToken.state == S_COL)
        {
            currToken = getNextToken(file);
            //alokace nove pameti pro jmeno promene do taublky sym
            velikost = strlen(currToken.name);
            if ( (variableType = malloc(velikost+1)) == NULL)
            {
                fprintf(stderr, "Chyba alokace!\n");
                if(errCode == 0)
                    errCode = E_ERR;
            }
            strcpy(variableType, currToken.name);

            //KONEC alokace nove pameti pro jmeno promene do taublky sym
            boolean = ( type(NULL, 0, NULL) * isStrednik() * var_def(from, localTable2) );
            if(boolean == 1 && from == 1) //Z VAR SEKCE GLOB
            {
                //Nyni pripravuju novou promenou na vlozeni do tabulky symbolu
                data.wasWritten = 0;
                data.value = "var";
                data.type = variableType;
                data.localTable = NULL; //tabulka symbolu pro funkci o velikosti 101
                //KONEC Nyni pripravuju novou promenou na vlozeni do tabulky symbolu

                //GENEROVANI KODU!!!
                if(strcmp(variableType, "integer") == 0)
                {
                    IListInsert (&Instr_list, I_INT, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "boolean") == 0)
                {
                    IListInsert (&Instr_list, I_BOOL, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "real") == 0)
                {
                    IListInsert (&Instr_list, I_REAL, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "string") == 0)
                {
                    IListInsert (&Instr_list, I_STRING, NULL, NULL, NULL, NULL, variableName);
                }
                //KONEC GENEROVANI KODU!!!
                //SEMANTICKA KONTROLA
                stored = htSearch(globalTable, variableName);
                if (stored != NULL)
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
                //KONEC SEMANTICKE KONTROLY
                htInsert(globalTable, variableName, data); //vkladam
            }
            else if(boolean == 1 && from == 2) //Z FUNKCE
            {
                data.wasWritten = 0;
                data.value = "var";
                data.type = variableType;
                data.localTable = NULL; //tabulka symbolu pro funkci o velikosti 101

                if(strcmp(variableType, "integer") == 0)
                {
                    IListInsert (&Instr_list, I_INT, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "boolean") == 0)
                {
                    IListInsert (&Instr_list, I_BOOL, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "real") == 0)
                {
                    IListInsert (&Instr_list, I_REAL, NULL, NULL, NULL, NULL, variableName);
                }
                else if(strcmp(variableType, "string") == 0)
                {
                    IListInsert (&Instr_list, I_STRING, NULL, NULL, NULL, NULL, variableName);
                }
                //SEMANTICKA KONTROLA
                stored = htSearch(localTable2, variableName);
                if (stored != NULL)
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
                //KONEC SEMANTICKE KONTROLY
                if (strcmp(globalControlFunctionName, variableName) == 0)
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
                htInsert(localTable2, variableName, data); //vkladam
                stored = htSearch(localTable2, variableName);

            }
        }
        //uvolneni pameti a redefinovani promenych
        //KONEC uvolneni pameti
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "function") == 0 )|| (currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        boolean = 1;
    }
    return boolean;
 }

  int var_func(tHTable* localTable2)
 {
    int boolean = 0;
    char *variableName;
    char *variableType;
    //printf("Local function name je: %s\n", localFunctionName);
    if((currToken.name != NULL && strcmp(currToken.name, "var") == 0 ))
    {
        currToken = getNextToken(file);
        if (currToken.state == S_ID && isKeyWord(currToken) == 0)
        {
            //alokace nove pameti pro jmeno promene do taublky sym
            int velikost = strlen(currToken.name);
            if ( (variableName = malloc(velikost+1)) == NULL )
            {
                if(errCode == 0)
                {
                    fprintf(stderr, "Chyba alokace!\n");
                    errCode = E_ERR;
                }
            }
            strcpy(variableName, currToken.name);
            //KONEC alokace nove pameti pro jmeno promene do taublky sym
            currToken = getNextToken(file);
            if (currToken.state == S_COL)
            {
                currToken = getNextToken(file);
                //alokace nove pameti pro jmeno promene do taublky sym
                velikost = strlen(currToken.name);
                if ( (variableType = malloc(velikost+1)) == NULL)
                {
                    fprintf(stderr,"Chyba alokace!\n" );
                    if(errCode == 0)
                        errCode = E_ERR;
                }
                strcpy(variableType, currToken.name);
                //KONEC alokace nove pameti pro jmeno promene do taublky sym
                boolean = ( type(NULL, 0, NULL) * isStrednik() * var_def(2, localTable2) );

                if(boolean == 1)
                {
                    //Nyni pripravuju novou promenou na vlozeni do tabulky symbolu

                    data.wasWritten = 0;
                    data.value = "var";
                    data.type = variableType;
                    data.localTable = NULL;
                    //KONEC Nyni pripravuju novou promenou na vlozeni do tabulky symbolu

                    if(strcmp(variableType, "integer") == 0)
                    {
                        IListInsert (&Instr_list, I_INT, NULL, NULL, NULL, NULL, variableName);
                    }
                    else if(strcmp(variableType, "boolean") == 0)
                    {
                        IListInsert (&Instr_list, I_BOOL, NULL, NULL, NULL, NULL, variableName);
                    }
                    else if(strcmp(variableType, "real") == 0)
                    {
                        IListInsert (&Instr_list, I_REAL, NULL, NULL, NULL, NULL, variableName);
                    }
                    else if(strcmp(variableType, "string") == 0)
                    {
                        IListInsert (&Instr_list, I_STRING, NULL, NULL, NULL, NULL, variableName);
                    }
                    //SEMANTICKA KONTROLA
                    stored = htSearch(localTable2, variableName);
                    if (stored != NULL)
                    {
                        if(errCode == 0)
                            errCode = E_SEMP;
                    }
                    //KONEC SEMANTICKE KONTROLY
                    if (strcmp(globalControlFunctionName, variableName) == 0)
                    {
                        if(errCode == 0)
                            errCode = E_SEMP;
                    }
                    htInsert(localTable2, variableName, data);
                    //vypisCelouTabulku(stored->data.localTable);

                }
            }
        }
        //vypisCelouTabulku(data.localTable);
        //free(variableName);
        //free(variableType);
        //vypisCelouTabulku(data.localTable);
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        boolean = 1;
    }
    return boolean;
 }

 int after_var()
 {
    int boolean = 0;
    if ((currToken.name != NULL && strcmp (currToken.name, "function") == 0))
    {
        currToken = getNextToken(file);
        boolean = ( func_def() );
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        if((currToken.name != NULL && strcmp(currToken.name, "end") == 0))
        {
            IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
            currToken = getNextToken(file);
            if(currToken.state == S_PER)
            {
                currToken = getNextToken(file);
                if(currToken.state == S_EOF)
                    boolean = 1;
            }
        }
        else
            boolean = st_list();
    }
    return boolean;
 }

 int isPZavorka()
 {
     if(currToken.state == S_RPAR)
     {
        currToken = getNextToken(file);
        return 1;
     }
    else
        return 0;
 }

 int isDvojtecka()
 {
     if(currToken.state == S_COL)
     {
        currToken = getNextToken(file);
        return 1;
     }
    else
        return 0;
 }
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU

 int func_def()
 {
    int boolean = 0;
    int fromForward = 0;
    parCount = 0;
    //int localCouner = 1;
    if (currToken.state == S_ID)
    {
        if(currToken.name != NULL)
        {
            stored = htSearch(globalTable, currToken.name);
            if (stored != NULL && strcmp("var", stored->data.value) == 0)
            {
                if(errCode == 0)
                    errCode = E_SEMP;
            }
            if(isKeyWord(currToken) == 1 )
            {
                if(errCode == 0)
                    errCode = E_SEMP;
            }
        }

        //KOPIROVANI NAZVU FUNKCE DO LOCAL FUNCTION NAME
        char *localFunctionName;
        int velikost = strlen(currToken.name);
        if ( (localFunctionName = malloc(velikost+1)) == NULL)
        {
            fprintf(stderr, "Chyba alokace!\n");
            if(errCode == 0)
                errCode = E_ERR;
        }
        strcpy(localFunctionName, currToken.name);
        //KONEC KOPIROVANI NAZVU FUNKCE DO LOCAL FUNCTION NAME

        //vytvoreni promene localString, do ktere se budou zapisovat parametry a typ funkce
        char localString[256];
        memset(localString, 0, 255);
        //KONEC vytvoreni promene localString, do ktere se budou zapisovat parametry a typ funkce


        //alokace a inicializace lokalni tabulky symbolu
        currToken = getNextToken(file);
        tHTable* localTable2 = (tHTable*) malloc ( sizeof(tHTable) );
        htInit(localTable2);
        globalLocalTable = localTable2;
        //alokace a inicializace lokalni tabulky symbolu

        globalControlFunctionName = localFunctionName;

        //vlozeni samotne funkce do jeji tabulky symbolu
        char *localFunctionName2;
        velikost = strlen(localFunctionName);
        if ( (localFunctionName2 = malloc(velikost+1)) == NULL)
        {
            fprintf(stderr, "Chyba alokace!\n");
            if(errCode == 0)
                errCode = E_ERR;
        }
        strcpy(localFunctionName2, localFunctionName);
        data.localTable = NULL;
        data.type = NULL;
        data.wasWritten = 0;
        data.value = "var";
        htInsert(localTable2, localFunctionName2, data);
        //konec vlozeni samotne funkce

        //vlozeni samotne funkce do globalni tabulky symbolu
        data.localTable = NULL;
        data.type = NULL;
        data.wasWritten = 0;
        data.value = "function";
        htInsert(globalTable, localFunctionName, data);
        //konec vlozeni samotne funkce do globalni tabulky*/

        if (currToken.state == S_LPAR)
        {
            currToken = getNextToken(file);

            boolean = par_list(localString) * isPZavorka() * isDvojtecka() * type(localString, 2, localFunctionName) * isStrednik();
            strcpy(globalFunctionName, localFunctionName);
            //strcpy(globalString, localString);
            memcpy(globalString,localString,sizeof(localString));
            //printf("Global string je: %s\n", globalString);
            IListInsert (&Instr_list, I_FUNC_ZAC, NULL, NULL, NULL, globalString, globalFunctionName);
            boolean = boolean * after_func(localTable2, &fromForward);
        }
        //zapisovani funkce do Globalni Tabulky Symbolu


        if(fromForward == 0)
        {

            //malloc pro localFunctionString
            char *localFunctionType;
            int velikost2 = 255;
            if ( (localFunctionType = malloc(velikost2+1)) == NULL)
            {
                fprintf(stderr, "Chyba alokace!\n");
                if(errCode == 0)
                    errCode = E_ERR;
            }
            memcpy(localFunctionType,localString,sizeof(localString));
            //konec mallocu

            //inicializace datove casti
            data.localTable = localTable2;
            data.type = localFunctionType;
            data.wasWritten = 2;
            data.value = "function";
            //KONEC inicializace datove casti


            //SEMANTICKA KONTROLA
            stored = htSearch(globalTable, localFunctionName); //VZDY NAJDE
            if (stored != NULL) //VZDY NAJDE
            {
                if(stored->data.wasWritten == 2) //UZ TU BYLA FUNKCE
                {
                    htClearAll(localTable2);
                    free(localTable2);
                    free(localFunctionName);
                    free(localFunctionType);
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
                else if(stored->data.wasWritten == 0) //NIC PREDTIM NEBYLO
                {
                    //vypisCelouTabulku(data.localTable);
                    htInsert(globalTable, localFunctionName, data);
                }
                else if(stored->data.wasWritten == 1) //BYL TAM FORWARD
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                    htClearAll(localTable2);
                    free(localTable2);
                    free(localFunctionName);
                    free(localFunctionType);
                }


                /*if(errCode == 0)
                    errCode = E_SEMP;*/
            }


            //KONEC SEMANTICKE KONTROLY

            //KONEC zapisovani do globalni tabulky symbolu
            //printf("Local string je: %s\n", localString);
        }
        else if (fromForward == 1)
        {
            //vycisteni pameti, forward nema svoji lokalni tabulku
            htClearAll(localTable2);
            free(localTable2);
            //konec vycisteni pameti

            //malloc pro localFunctionString
            char *localFunctionType;
            int velikost2 = 255;
            if ( (localFunctionType = malloc(velikost2+1)) == NULL)
            {
                fprintf(stderr, "Chyba alokace!\n");
                if(errCode == 0)
                    errCode = E_ERR;
            }
            memcpy(localFunctionType,localString,sizeof(localString));
            //konec mallocu

            //inicializace dat
            data.type = localFunctionType;
            data.wasWritten = 1;
            data.localTable = NULL;
            data.value = "function";
            //KONEC inicializace dat

            //forward
            stored = htSearch(globalTable, localFunctionName); //VZDY BY MEL NAJIT
            if(stored != NULL)
            {
                if(stored->data.wasWritten == 1) //TZN UZ BYL FORWARD
                {
                    if (errCode == 0)
                        errCode = E_SEMP;
                    free(localFunctionName);
                    free(localFunctionType);
                }
                else if (stored->data.wasWritten == 0) //POPRVE VE FORWARDU TZN NEBYLA PROVEDENA DEKLARACE FUNKCE
                {
                    if (errCode == 0)
                        errCode = E_SEMP;
                    stored->data.wasWritten = 1; //POZNAMKA ZE JSEM BYL VE FORWARDU
                    htInsert(globalTable, localFunctionName, data);
                }
                else if (stored->data.wasWritten == 2) //UZ BYLA FUNKCE
                {
                    if(strcmp(stored->data.type, localString) != 0)
                    {
                        if (errCode == 0)
                            errCode = E_SEMP; //TOTO JE HROZNE NAHODNE NEKDE JE CHYBICKA!!!
                    }
                    stored->data.wasWritten = 1;
                    free(localFunctionName);
                    free(localFunctionType);
                }
                /*if(strcmp(stored->data.type, localString) != 0)
                {
                    if (errCode == 0)
                        errCode = E_SEMP; //TOTO JE HROZNE NAHODNE NEKDE JE CHYBICKA!!!
                }*/
                /*free(localFunctionName);
                free(localFunctionType);*/
            }
            /*else
            {
                if (errCode == 0)
                    errCode = E_SEMP;
                htInsert(globalTable, localFunctionName, data);
            }*/
            //END forward



            //KONEC zapisovani do globalni tabulky symbolu
            //printf("Local string je: %s\n", localString);
        }
    }
    return boolean;
 }

//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU
//KONEC DEFINICE FUNKCE ZACATEK DULEZITE NEJVIC PROBLEMU JJ PLS CAU

int par_list(char *localString)
 {
    int boolean = 0;
    char *variableType;
    if (currToken.state == S_ID)
    {

        //vlozeni parametru do tabulky symbolu
        char *paramName;
        int velikost = strlen(currToken.name);
        if ( (paramName = malloc(velikost+1)) == NULL)
        {
            fprintf(stderr, "Chyba alokace!\n");
            if(errCode == 0)
                errCode = E_ERR;
        }
        strcpy(paramName, currToken.name);
        data.localTable = NULL;
        data.type = NULL;
        data.wasWritten = 0;
        data.value = "var";

        //konec vlozeni parametru do tabulky symbolu

        if(  strcmp(currToken.name, globalControlFunctionName) == 0  )
            if(errCode == 0)
                errCode = E_SEMP;
        parCount++;
        addLocalCharParser(currToken.name, localString);
        addLocalCharParser(":", localString);
        currToken = getNextToken(file);
        if (currToken.state == S_COL)
        {
            currToken = getNextToken(file);
            velikost = strlen(currToken.name);
            if ( (variableType = malloc(velikost+1)) == NULL)
            {
                fprintf(stderr,"Chyba alokace!\n" );
                if(errCode == 0)
                    errCode = E_ERR;
            }
            strcpy(variableType, currToken.name);
            data.type = variableType;
            htInsert(globalLocalTable, paramName, data);
            boolean = ( type(localString, 1, NULL) * param_n(localString) );
        }
    }
    else if(currToken.state == S_RPAR)
        boolean = 1;
    return boolean;
 }

int param_n(char *localString)
 {
    int boolean = 0;
    char *variableType;
    if(currToken.state == S_SEM)
    {
        currToken = getNextToken(file);
        if (currToken.state == S_ID)
        {
            //vlozeni parametru do tabulky symbolu
            char *paramName;
            int velikost = strlen(currToken.name);
            if ( (paramName = malloc(velikost+1)) == NULL)
            {
                fprintf(stderr, "Chyba alokace!\n");
                if(errCode == 0)
                    errCode = E_ERR;
            }
            strcpy(paramName, currToken.name);
            data.localTable = NULL;
            data.type = NULL;
            data.wasWritten = 0;
            data.value = "var";
            //konec vlozeni parametru do tabulky symbolu
            if(  strcmp(currToken.name, globalControlFunctionName) == 0  )
                if(errCode == 0)
                    errCode = E_SEMP;
            parCount++;
            addLocalCharParser(currToken.name, localString);
            addLocalCharParser(":", localString);
            currToken = getNextToken(file);
            if (currToken.state == S_COL)
            {
                currToken = getNextToken(file);
                velikost = strlen(currToken.name);
                if ( (variableType = malloc(velikost+1)) == NULL)
                {
                    fprintf(stderr,"Chyba alokace!\n" );
                    if(errCode == 0)
                        errCode = E_ERR;
                }
                strcpy(variableType, currToken.name);
                data.type = variableType;
                htInsert(globalLocalTable, paramName, data);
                boolean = ( type(localString, 1, NULL) * param_n(localString) );
            }
        }
    }
    else if(currToken.state == S_RPAR)
        boolean = 1;
    return boolean;
 }

int type(char *localString, int fromParams,char* nazevFunkce) //localString je lokalni string a from zjistuje odkud se jde. 0 jedno, 1 z params, 2 z func_def
 {
    int boolean = 0;
    if ((currToken.name != NULL && strcmp(currToken.name, "integer") == 0) || (currToken.name != NULL && strcmp(currToken.name, "real") == 0) || (currToken.name != NULL && strcmp(currToken.name, "string") == 0) || (currToken.name != NULL && strcmp(currToken.name, "boolean") == 0))
    {
        if(fromParams == 1)
        {
            char pomoc[2];
            pomoc[0] = currToken.name[0];
            pomoc[1] = '\0';
            addLocalCharParser(pomoc, localString);
            addLocalCharParser(",", localString);


        }
        else if(fromParams == 2)
        {
            /*char pomoc[2];
            pomoc[0] = currToken.name[0];
            pomoc[1] = '\0';
            addLocalCharParser(pomoc, localString);*/
            localString[0] = currToken.name[0];
            localString[1] = ',';
            stored = htSearch(globalTable, nazevFunkce);
            if(stored != NULL)
            {
                int velikostFunkce = strlen(currToken.name);
                if((stored->data.type = malloc(velikostFunkce+1)) == NULL)
                {
                    if(errCode == 0)
                        errCode = E_ERR;
                }
                strcpy(stored->data.type, currToken.name);
                stored = htSearch(globalLocalTable, nazevFunkce);
                if(stored != NULL)
                {
                    if((stored->data.type = malloc(velikostFunkce+1)) == NULL)
                    {
                        if(errCode == 0)
                            errCode = E_ERR;
                    }
                    strcpy(stored->data.type, currToken.name);
                }
            }
        }
        currToken = getNextToken(file);
        boolean = 1;
    }
    return boolean;
 }

int term(int *localState,char *localString)
 {
    int boolean = 0;
    if(currToken.state == S_ID)
    {
        strcpy(localString, currToken.name);
        (*localState) = currToken.state;
        if (currToken.name != NULL)
        {
            if(inFunction == 0)
            {
                stored = htSearch(globalTable, currToken.name);
                if(stored == NULL)
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
            }
            else if(inFunction == 1)
            {
                stored = htSearch(globalLocalTable, currToken.name);
                if(stored == NULL)
                {
                    stored = htSearch(globalTable, currToken.name);
                    if(stored == NULL)
                        if(errCode == 0)
                            errCode = E_SEMP;
                }
            }
        }
        currToken = getNextToken(file);
        boolean = 1;
    }
    else if (currToken.state == S_INT || currToken.state == S_REAL1 || currToken.state == S_REAL2 || currToken.state == S_STR || isTF() == 1)
    {
        strcpy(localString, currToken.name);
        (*localState) = currToken.state;
        currToken = getNextToken(file);
        boolean = 1;
    }
    return boolean;
 }

 int after_stat()
 {
    int boolean = 0;
    if(currToken.state == S_SEM)
    {
        currToken = getNextToken(file);
        boolean = st_list();
    }
    else if (currToken.name != NULL && strcmp(currToken.name, "end") == 0)
    {
        IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        boolean = after_end();
    }
    return boolean;
 }

  int after_end()
 {
    int boolean = 0;
    if(currToken.state == S_PER)
    {
        currToken = getNextToken(file);
        if(currToken.state == S_EOF)
        {
            boolean = 1;
        }
    }
    else if ((currToken.name != NULL && strcmp(currToken.name, "else") == 0) || (currToken.name != NULL && strcmp(currToken.name, "end") == 0) || currToken.state == S_SEM)
    {
        boolean = 1;
    }
    return boolean;
 }

 int st_list()
 {
    int boolean = 0;
    if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0 )|| currToken.state == S_ID || (currToken.name != NULL && strcmp(currToken.name, "if") == 0) || (currToken.name != NULL && strcmp(currToken.name, "while") == 0) || (currToken.name != NULL && strcmp(currToken.name, "readln") == 0) || (currToken.name != NULL && strcmp(currToken.name, "write") == 0))
    {
        boolean =  stat() * after_stat();
        //printf("Boolean stat je: %d\n", stat());
        //printf("Aktualni token je: %d %s\n", currToken.state, currToken.name);
    }
    return boolean;
 }

int after_func(tHTable* localTable2, int *fromForward)
{
    int boolean = 0;
    count123 = 3;
    if((currToken.name != NULL && strcmp(currToken.name, "var") == 0) || (currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        boolean = (var_func(localTable2) * isBegin() * func_prikazy());
    }
    else if( (currToken.name != NULL && strcmp(currToken.name, "forward")== 0))
    {
        IListUpdateLast(&Instr_list, I_FUNC_DEK);
        //IListInsert (&Instr_list, I_FUNC_DEK, NULL, NULL, NULL, globalString, globalFunctionName);
        currToken = getNextToken(file);
        (*fromForward) = 1;
        if(currToken.state == S_SEM)
        {
            currToken = getNextToken(file);
            if( (currToken.name != NULL && strcmp(currToken.name, "function")== 0))
            {
                currToken = getNextToken(file);
                boolean = func_def();
            }
        }
    }
    return boolean;
}

int func_prikazy()
{
    int boolean = 0;
    if(currToken.name != NULL && strcmp(currToken.name, "end") == 0)
    {
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        boolean = isStrednik() * next_func();
    }
    else
    {
        //IListInsert (&Instr_list, I_FUNC_ZAC, NULL, NULL, NULL, globalString, globalFunctionName);
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        inFunction = 1; // ted vim, ze nasledujici prikazy budou provadeny ve funkci
        boolean = st_list() * isStrednik();
        globalLocalTable = NULL;
        inFunction = 0;
        boolean = boolean * next_func();
    }
    return boolean;
}

 int next_func()
 {
    int boolean = 0;
    if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        if((currToken.name != NULL && strcmp(currToken.name, "end") == 0))
        {
            IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
            currToken = getNextToken(file);
            if(currToken.state == S_PER)
            {
                currToken = getNextToken(file);
                if(currToken.state == S_EOF)
                {
                    boolean = 1;
                }
            }
        }
        else
        {
            boolean = st_list();
        }
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "function") == 0))
    {
        currToken = getNextToken(file);
        boolean =  func_def();
    }
    return boolean;
 }

 int stat()
 {
    int boolean = 0;
    if((currToken.name != NULL && strcmp(currToken.name, "if") == 0))
    {
        currToken = getNextToken(file);
        IListInsert (&Instr_list, I_COND, NULL, NULL, NULL, NULL, NULL);
        boolean =  prec_analysis(&finish);
        //printf("Boolean je: %d\n", boolean);
        IListInsert (&Instr_list, I_IF, NULL, NULL, NULL, NULL, NULL);
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        boolean =  boolean * isThen() * isBegin();
        if(currToken.name != NULL)
        {
            if(strcmp(currToken.name, "end") == 0)
            {
                IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
                currToken = getNextToken(file);
                boolean = boolean * after_if();
            }
            else
            {

                boolean = boolean * st_list() * after_if();
            }
        }
        return boolean;
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "while") == 0))
    {
        currToken = getNextToken(file);
        IListInsert (&Instr_list, I_COND, NULL, NULL, NULL, NULL, NULL);
        boolean =  prec_analysis(&finish) * isDo();
        IListInsert (&Instr_list, I_WHILE, NULL, NULL, NULL, NULL, NULL);
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        boolean = boolean * isBegin();
        if(currToken.name != NULL)
        {
            if(strcmp(currToken.name, "end") == 0)
            {
                IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
                currToken = getNextToken(file);
                boolean = 1;
            }
            else
            {

                boolean = boolean * st_list();
            }
        }

        return boolean;
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "readln") == 0))
    {
        currToken = getNextToken(file);
        if(currToken.state == S_LPAR)
        {
            currToken = getNextToken(file);
            if(currToken.state == S_ID)
            {
                //ZACATEK SEMANTICKE KONTROLY
                if(currToken.name != NULL)
                {
                    if(inFunction == 0)
                    {
                        stored = htSearch(globalTable, currToken.name);
                        if(stored == NULL)
                        {
                            if(errCode == 0)
                                errCode = E_SEMP;
                        }
                    }
                    else if(inFunction == 1)
                    {
                        stored = htSearch(globalLocalTable, currToken.name);
                        if(stored == NULL)
                        {
                            stored = htSearch(globalTable, currToken.name);
                            if(stored == NULL)
                                if(errCode == 0)
                                    errCode = E_SEMP;
                        }
                    }
                }
                //KONEC SEMANTICKE KONTROLY
                strcpy(instructionString, currToken.name);
                currToken = getNextToken(file);
                if(currToken.state == S_RPAR)
                {
                    currToken = getNextToken(file);
                    IListInsert (&Instr_list, I_READ, NULL, NULL, NULL, NULL, instructionString);
                    boolean = 1;
                    return boolean;
                }
            }
        }
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        if((currToken.name != NULL && strcmp(currToken.name, "end") == 0))
        {
            IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
            currToken = getNextToken(file);
            boolean = 1;
        }
        else
            boolean =  st_list();
        return boolean;
    }
    else if((currToken.name != NULL && strcmp(currToken.name, "write") == 0))
    {
        pushCount = 0;
        currToken = getNextToken(file);
        if(currToken.state == S_LPAR)
        {
            currToken = getNextToken(file);
            //printf("Curr token state je: %d name je: %s\n", currToken.state, currToken.name);
            boolean = term_list() * isPZavorka();
            //printf("Pushcount je:%d\n", pushCount);
            IListInsert (&Instr_list, I_WRITE, &pushCount, NULL, NULL, NULL, NULL);
            return boolean;
        }
    }
    else if(currToken.state == S_ID && isKeyWord(currToken) == 0) // id := 5 napr
    {
        strcpy(instructionString, currToken.name);

        //printf("Curr token state je: %d name je: %s\n", currToken.state, currToken.name);
        currToken = getNextToken(file);
        //printf("Instr string name je: %s\n", instructionString);
        if(currToken.state == S_ASGN)
        {
            currToken = getNextToken(file);

            //semanticka kontrola
            if(instructionString != NULL)
            {
                if(inFunction == 0)
                {
                    stored = htSearch(globalTable, instructionString);
                    if(stored == NULL)
                    {
                        if(errCode == 0)
                            errCode = E_SEMP;
                    }
                }
                else if(inFunction == 1)
                {
                    stored = htSearch(globalLocalTable, instructionString);
                    if(stored == NULL)
                    {
                        stored = htSearch(globalTable, instructionString);
                        if(stored == NULL)
                            if(errCode == 0)
                                errCode = E_SEMP;
                    }
                }
            }
            //konec semanticke kontroly

            boolean = expresion();
            if(boolean == 1)
            {
                if(inFunction == 0)
                {
                    stored = htSearch(globalTable, instructionString);
                }
                else if(inFunction == 1)
                {
                    stored = htSearch(globalLocalTable, instructionString);
                    if(stored == NULL)
                    {
                        stored = htSearch(globalTable, instructionString);
                    }
                }
                if(stored != NULL)
                {
                    //printf("finish %d \n",finish);
                    if((strcmp(stored->data.type, "integer") == 0) && (finish != H_INT))
                    {
                        //printf("tu1\n");
                        if(errCode == 0)
                            errCode = E_SEMF;
                    }
                    else if((strcmp(stored->data.type, "real") == 0) && (finish != H_REAL) && (finish != H_INT))
                    {
                        //printf("tu2\n");
                        if(errCode == 0)
                            errCode = E_SEMF;
                    }
                    else if((strcmp(stored->data.type, "string") == 0) && (finish != H_STRING))
                    {
                        //printf("tu3\n");
                        if(errCode == 0)
                            errCode = E_SEMF;
                    }
                    else if((strcmp(stored->data.type, "boolean") == 0) && (finish != H_BOOL))
                    {
                        //printf("tu4\n");
                        if(errCode == 0)
                            errCode = E_SEMF;
                    }

                }
                IListInsert (&Instr_list, I_POP, NULL, NULL, NULL, NULL, instructionString);
            }
            return boolean;
        }
    }


    return boolean;
 }

  int after_if()
 {
    int boolean = 0;
    if((currToken.name != NULL && strcmp(currToken.name, "else") == 0))
    {
        IListInsert (&Instr_list, I_ELSE, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
        {
            IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
            currToken = getNextToken(file);
            if(currToken.name != NULL)
            {
                if(strcmp(currToken.name, "end") == 0)
                {
                    IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
                    currToken = getNextToken(file);
                    boolean =  1;
                }
                else
                {
                    boolean =  st_list();
                }
            }
        }

    }
    else if(currToken.name != NULL)
    {
        if(strcmp(currToken.name, "end") == 0)
        {
            boolean =  1;
        }
    }
    else if(currToken.state == S_SEM)
    {
        boolean = 1;
    }
    return boolean;
 }

 int expresion()
 {
    int boolean = 0;
    parCount = 0;
    char localString[128];
    if(currToken.state == S_ID)
    {
        strcpy(localString, currToken.name);
        strcpy(prevTokenString, currToken.name);
        prevToken.name = prevTokenString;
        prevToken.state = currToken.state; //NAT DOSTANE SEGFAULT SPRAVIT
        //printf("prev token name je : %s %d\n", prevToken.name, prevToken.state);
        if(currToken.name != NULL)
        {
            if (strcmp(currToken.name, "length") == 0)
            {
                finish = H_INT;
            }
            else if (strcmp(currToken.name, "copy") == 0)
            {
                finish = H_STRING;
            }
            else if (strcmp(currToken.name, "sort") == 0)
            {
                finish = H_STRING;
            }
            else if (strcmp(currToken.name, "find") == 0)
            {
                finish = H_INT;
            }
        }
        currToken = getNextToken(file);
        if(currToken.state == S_LPAR) //volani funkce
        {
            //SEMANTICKA KONTROLA
            if(prevToken.name != NULL && strcmp(prevToken.name, "length") != 0 && strcmp(prevToken.name, "sort") != 0 && strcmp(prevToken.name, "copy") != 0 && strcmp(prevToken.name, "find") != 0)
            {
                stored = htSearch(globalTable, prevToken.name);
                if(stored == NULL)
                {
                    if(errCode == 0)
                        errCode = E_SEMP;
                }
                else
                {
                    if(strcmp (stored->data.type, "integer") == 0)
                    {
                        finish = H_INT;
                    }
                    else if(strcmp (stored->data.type, "real") == 0)
                    {
                        finish = H_REAL;
                    }
                    else if(strcmp (stored->data.type, "boolean") == 0)
                    {
                        finish = H_BOOL;
                    }
                    else if(strcmp (stored->data.type, "string") == 0)
                    {
                        finish = H_STRING;
                    }
                }
            }
            //KONEC SEMANTICKE KONTROLY

            prevToken.name = NULL;
            prevToken.state = S_FALSE;
            currToken = getNextToken(file);
            boolean = term_list() * isPZavorka();
            //v local string nazev fce


            //generovani instrukce
            IListInsert (&Instr_list, I_CALL, NULL, NULL, NULL, NULL, localString);
            //konec gen. instrukce

        }
        else //ostatni vyrazy
        {
            boolean = prec_analysis(&finish);
        }
    }
    else
    {
        boolean =  prec_analysis(&finish);
    }
    return boolean;
 }

 int term_list()
 {
    int boolean = 0;
    int localState = 0; //localState obsahuje ID
    char localString[128];
    int localStringPom = 0; //int pro prevedeni stringu na int
    float localFloat = 0;
    if (currToken.state == S_ID || currToken.state == S_INT || currToken.state == S_REAL1 || currToken.state == S_REAL2 || currToken.state == S_STR || isTF() == 1)
    {
        parCount++;


        boolean =  term(&localState, localString) * term_n();
        //generovani instrukci
        if (localState == S_INT)
        {
            localStringPom = atoi(localString);
            IListInsert (&Instr_list, I_PUSH, &localStringPom, NULL, NULL, NULL, NULL);
            pushCount++;
        }
        if (localState == S_STR)
        {
            IListInsert (&Instr_list, I_PUSH, NULL, NULL, NULL, localString, NULL);
            pushCount++;
        }
        if (localState == S_REAL1)
        {
            localFloat = atof(localString);
            IListInsert (&Instr_list, I_PUSH, NULL, &localFloat, NULL, NULL, NULL);
            pushCount++;
        }
        if (localState == S_REAL2)
        {
            localFloat = atof(localString);
            IListInsert (&Instr_list, I_PUSH, NULL, &localFloat, NULL, NULL, NULL);
            pushCount++;
        }
        if (localState == S_ID)
        {
            IListInsert (&Instr_list, I_PUSH, NULL, NULL, NULL, NULL, localString);
            pushCount++;
        }
        //KONEC generovani instrukci
    }
    else if(currToken.state == S_RPAR)
    {
        boolean = 1; // pravidlo term_list -> epsilon
    }
    return boolean;
 }

int term_n()
 {
    int boolean = 0;
    int localState = 0;
    char localString[128];
    int localStringPom = 0; //int pro prevedeni stringu na int
    float localFloat = 0;
    if(currToken.state == S_COM)
    {
        currToken = getNextToken(file);
        if (currToken.state == S_ID || currToken.state == S_INT || currToken.state == S_REAL1 || currToken.state == S_REAL2 || currToken.state == S_STR || isTF() == 1)
        {
            parCount++;
            boolean =  term(&localState, localString) * term_n();
            if (localState == S_INT)
            {
                localStringPom = atoi(localString);
                IListInsert (&Instr_list, I_PUSH, &localStringPom, NULL, NULL, NULL, NULL);
                pushCount++;
            }
            if (localState == S_STR)
            {
                IListInsert (&Instr_list, I_PUSH, NULL, NULL, NULL, localString, NULL);
                pushCount++;
            }
            if (localState == S_REAL1)
            {
                localFloat = atof(localString);
                IListInsert (&Instr_list, I_PUSH, NULL, &localFloat, NULL, NULL, NULL);
                pushCount++;
            }
            if (localState == S_REAL2)
            {
                localFloat = atof(localString);
                IListInsert (&Instr_list, I_PUSH, NULL, &localFloat, NULL, NULL, NULL);
                pushCount++;
            }
            if (localState == S_ID)
            {
                if (currToken.name != NULL)
                {
                    if(inFunction == 0)
                    {
                        stored = htSearch(globalTable, currToken.name);
                        if(stored == NULL)
                        {
                            if(errCode == 0)
                                errCode = E_SEMP;
                        }
                    }
                    else if(inFunction == 1)
                    {
                        stored = htSearch(globalLocalTable, currToken.name);
                        if(stored == NULL)
                        {
                            stored = htSearch(globalTable, currToken.name);
                            if(stored == NULL)
                                if(errCode == 0)
                                    errCode = E_SEMP;
                        }
                    }
                }


                IListInsert (&Instr_list, I_PUSH, NULL, NULL, NULL, NULL, localString);
                pushCount++;
            }
        }
    }
    else if(currToken.state == S_RPAR)
    {
        boolean = 1;
    }
    return boolean;
 }

 int isBegin()
 {
    if((currToken.name != NULL && strcmp(currToken.name, "begin") == 0))
    {
        //IListInsert (&Instr_list, I_BEGIN, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }

 int isTF()
 {
    if((currToken.name != NULL && strcmp(currToken.name, "true") == 0) || (currToken.name != NULL && strcmp(currToken.name, "false")))
    {
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }
int isDo()
 {
    if((currToken.name != NULL && strcmp(currToken.name, "do") == 0))
    {
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }
int isEnd()
 {
    if((currToken.name != NULL && strcmp(currToken.name, "end") == 0))
    {
        IListInsert (&Instr_list, I_END, NULL, NULL, NULL, NULL, NULL);
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }
 int isThen()
 {
    if((currToken.name != NULL && strcmp(currToken.name, "then") == 0))
    {
        currToken = getNextToken(file);
        return 1;
    }
    else
        return 0;
 }

void addCharParser(char *char1) {
    // max dlzka premennej je 127 platnych znakov
    int delka = strlen(char1);
    for (int i = 0; i < (delka+1); i++)
    {
        if (count123 != 255) {
        stringos[count123] = char1[i];
        count123++;
        stringos[count123] = '\0';
    }
    }
}

void addLocalCharParser(char *char1, char *localString) {
    // max dlzka premennej je 127 platnych znakov
    int delka = strlen(char1);
    count123--;
    for (int i = 0; i < (delka+1); i++)
    {
        if (count123 != 255) {
        localString[count123] = char1[i];
        count123++;
        localString[count123] = '\0';
        }
    }
}

void vypisCelouTabulku(tHTable* ptrht)
{
	printf ("------------HASH TABLE--------------\n");
    for (int i = 0; i < 101; i++)
    {
        printf ("%i:",i);
		tHTItem* ptr = (*ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s, %s,%s, WAS WRITTEN JE: %d)",ptr->data.value, ptr->key, ptr->data.type,ptr->data.wasWritten);
			ptr = ptr->ptrnext;
		}
		printf ("\n");
    }
	printf ("------------------------------------\n");
}



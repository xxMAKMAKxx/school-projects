#include <stdio.h>
#include <string.h>

/* Program pro spocitani vyskytu pismen ve slove */
/* Autor: Martin Kacmarcik */
/* Vsechna prava vzhrazena */



/* funkce pro kontrolu pozice znaku (pokud je zadana),zjistuje jestli danne pismeno uz nahodou nebylo objeveno a take inkrementuje vysledek pokud vse proslo podminkama ve funkci znakCheck*/
void poziceCheckPlusVysledekInc(int *poziceZnaku, int *ukazatelNaPozici, int *slovoNaplneno, int *vysledek)
{
        if ( (*poziceZnaku) > 0 ) // zadal uzivatel pozici znaku?
        {
            if ( (*ukazatelNaPozici) == (*poziceZnaku) ) // pokud ano, je aktualni pozice rovna pozici znau kterou zadal uzivatel?
            {
                (*slovoNaplneno) ++; // pokud ano program si poznaci ze uz pismeno nasel
                (*vysledek) ++; // zvetsi vysledek
            }
        }
        else // pokud nebyla zadana poznaci si ze nasel pismeno a inkrementuje vysledek
        {
            (*slovoNaplneno) ++; 
            (*vysledek) ++;
        }
}

/* funkce pro kontrolu spravnosti hledaneho znaku*/

int znakCheck(char znak)
{
     if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak<= 'Z') || (znak >= '0' && znak <= '9') || (znak == '-') || (znak == '_'))
        return 1;
     else
        return 0;
}

/* srdce celeho programu, funkce ktera kontroluje dle zadaneho znaku (mysleno znaku zadaneho parametrem)
 vyskyt toho znaku ve slovech zadanych uzivatelem (i toto tato funkce zajistuje), pocita mezery, v pripade zadaneho debugModu vypisuje slova*/
int slovoCheck (char hledanyZnak, int debugMode, int poziceZnaku)
{
    int znak;
    int vysledek = 0;
    int slovoNaplneno = 0;
    int ukazatelNaPozici = 0;
    int counter = 0;
    int pocetMezer = 1;
    while ((znak = getchar()) != EOF) // nacitani dat od uzivatele
            {
                if (znakCheck(znak) == 1) // kontrola jestli zadany znak je opravdu znak nebo oddelovac
                {
                    if (debugMode == 1) //kontrola pro zapnuty debugmode aby mohl vypsat dany znak
                        {
                            if (counter != 80) // kontrola jestli nahodou se uz nevypsalo 80 znaku
                            {
                                putchar(znak);
                                counter ++;
                            }
                        }
                    if (hledanyZnak == ':'){ // pokud hledanyZnak je dvojtecka porovna aktualni vstup s cislem od 0 do 9
                        pocetMezer = 0;
                        ukazatelNaPozici++;
                        if (slovoNaplneno == 0) // diva se jestli uz znak ve slove nalezen nebyl nahodou
                        {
                            if (znak >= '0' && znak <= '9')
                                poziceCheckPlusVysledekInc(&poziceZnaku, &ukazatelNaPozici, &slovoNaplneno, &vysledek);
                        }
                    }
                    else if (hledanyZnak == '^'){ // pokud hledanyZnak je striska porovna aktualni vstup s pismeny A-Z
                        pocetMezer = 0;
                        ukazatelNaPozici++;
                        if (slovoNaplneno == 0) // diva se jestli uz znak ve slove nalezen nebyl nahodou
                        {
                            if (znak >= 'A' && znak <= 'Z')
                                poziceCheckPlusVysledekInc(&poziceZnaku, &ukazatelNaPozici, &slovoNaplneno, &vysledek);
                        }
                    }
                    else if (hledanyZnak == '.'){ // pokud je hledany znak tecka, tak uz to proslo znakCheck a vi se, ze to je ok tzn se provede fce
                        pocetMezer = 0;
                        ukazatelNaPozici++;
                        if (slovoNaplneno == 0) // diva se jestli uz znak ve slove nalezen nebyl nahodou
                            poziceCheckPlusVysledekInc(&poziceZnaku, &ukazatelNaPozici, &slovoNaplneno, &vysledek);
                    }
                    else { // jestli zadal cokoliv jineho porovna se to cokoliv s aktualnim vstupem
                        pocetMezer = 0;
                        ukazatelNaPozici++;
                        if (slovoNaplneno == 0) // diva se jestli uz znak ve slove nalezen nebyl nahodou
                        {
                            if (znak == hledanyZnak)
                                poziceCheckPlusVysledekInc(&poziceZnaku, &ukazatelNaPozici, &slovoNaplneno, &vysledek);
                        }
                    }

                }
                else
                {
                    slovoNaplneno = 0; //nastavi slovo naplneno na nula protoze uzivatel ukoncil slovo
                    ukazatelNaPozici = 0; //reset pozice
                    if (debugMode == 1) // pokud je debugmode aktivni udela mezeru 
                    {
                        if (pocetMezer == 0) // kontroluje aby byl pocet mezer 1
                        {
                            printf("\n");
                            pocetMezer++;
                        }
                        counter = 0;
                    }
                }

            }
    return vysledek;
}

/* funkce pro vypis napovedy*/
void vypisNapovedu()
{
    printf(
"Napoveda pro program: Najdi pismenko ve slovech 1.0 \n"
"----------------------------\n\n"
"./proj1 X [N] [-d] \n\n"
"----------------------------\n\n"
"X vyjadruje hledany znak. Znakem se rozumi pismeno od 'a' do 'z' , 'A' do 'Z' , cislo od '0' do '9' popripadne '-' nebo '_' \n"
"Dale muzeme vuzit specialni znaky. ':' symbolizuje libovolne cislo od 0 do 9. Znak ^ symbolizuje libovolne velke pismeno od A-Z. Znak '.' symbolizuje libovolny znak. \n\n"
"Volitelny argument N predstavuje pozici znaku ve slove. Cislo N musi byt kladne a cislo 1 oznacuje 1. pozici ve slove. To znamena pokud chci znak 'a' hledat na pozici 2. zadam argumenty takto: ./proj1 a 2 \n\n"
"Volitelny argument -d predstavuje zapnuti tzv. debug modu. Debug mode vypisuje jednotliva slova pod sebe, kazde na jeden radek. \n\n");
}

/* funkce, ktera prevadi cisla datoveho typu char na datovy typ int */
int charToInt(char *argument)
{
    int pozice = 0;
    if (!argument)
    {
        return 0;
    }
    else
    {
        while (*argument != '\0')
        {
            if( (*argument < '0') || (*argument > '9') )
            {
                return 0;
            }
            else
            {
                pozice = (pozice * 10 + *argument - '0');
            }
            argument++;
        }
    }
    return pozice;
}

int main(int argc, char *argv[])
{
    char hledanyZnak;
    int debugMode = 0;
    int i = 0;
    int poziceZnaku = 0;
    int poziceNaleznea = 0;
    if (argc == 1)
    {
        printf("Nebyl zadan znak, jehoz vyskyt hledate, jako argument, pouzijte --help pro napovedu.");
        return 0;
    }
    else
    {
        for (i = 1 ; i < argc ; i++) // for cyklus, ktery zjistuje parametry
        {
            if (strcmp(argv[i], "--help") == 0)
            {
                vypisNapovedu();
                return 0;
            }
            if (strcmp(argv[i], "-d") == 0)
            {
                debugMode = 1;
            }
			if(i > 1) //Podminka, ktera zajistuje, aby nebral jako hledany znak take pozici. Hledany znak se zadava na indexu 1, proto podminka zajisti, aby se hledalo od indexu 2
			{
				if (((*argv[i] >= '0') && (*argv[i] <= '9')) && poziceNaleznea == 0)
				{
					poziceNaleznea = 1;
					poziceZnaku = charToInt(argv[i]);
				}
			}
        }
        hledanyZnak = *argv[1];
        if ((hledanyZnak >= 'a' && hledanyZnak <= 'z') || (hledanyZnak >= 'A' && hledanyZnak<= 'Z') || (hledanyZnak >= '0' && hledanyZnak <= '9') || (hledanyZnak == '-') || (hledanyZnak == '_') || (hledanyZnak == '.') || (hledanyZnak == '^') || (hledanyZnak == ':') )
        {
            printf("%d\n", slovoCheck(hledanyZnak, debugMode, poziceZnaku));
            return 0;
        }
        else
        {
            printf("Zadan spatny znak, pouzijte argument --help pro vypsani napovedy");
            return 0;
        }
    }

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define eps 0.000000000001
#define PI 3.14159265358979323846

/* **************************************************** */
/* * *           Vypocet uhlu trojuhelniku          * * */
/* * *                                              * * */
/* * *                Projekt 2 IZP                 * * */
/* * *                                              * * */
/* * *              Martin Kacmarcik                * * */
/* * *                                              * * */
/* **************************************************** */


void vypisNapovedu() //program pro vypis napovedy
{
    printf("Program pro vypocet uhlu trojuhelniku, arcsinu a odmocniny. By Martin Kacmarcik (c) 2013\n"
           "Pro vypocet uhlu trojuhelniku pridejte parametr --triangle a jako dalsi parametry zadejte souradnice vsech bodu trojuhelniku v tomto poradi: AX, AY, BX, BY, CX, CY . Vysledek je v radianech.\n"
           "Pro vypocet odmocniny z cisla zadejte --sqrt cislo\n"
           "Pro vypocet arcsin zadejte parametr --asin cislo\n"
           );
    return;
}

double d_abs(double x) //program, ktery vrati absolutni hodnotu cisla
{
	if(x<0)
		return (-x);
	else
		return x;

}

int parametrCheck(int argc,char **argv, int *sqrtMode, int *asinMode, int *triangleMode) //funkce ktera nastavi danny mod v zavislosti na parametru, ktery zadal uzivatel
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                vypisNapovedu();
                return 0;
            }
        if (strcmp(argv[i], "--sqrt") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *sqrtMode = 1;
            }
        if (strcmp(argv[i], "--asin") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *asinMode = 1;
            }
        if (strcmp(argv[i], "--triangle") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *triangleMode = 1;
                if ( argc < 8) // kontrola jestli uzivatel zadal spravny pocet parametru
                {
                    *triangleMode = 0;
                    fprintf(stderr, "Chyba, nebyly zadany spravne parametry, zvolte parametr --help pro napovedu");
                    return 999;
                }
            }
    }
    return 0;
}
double my_sqrt(double x) // program pro vypocet odmocniny
{
    double vysledek = 1; // aktualni vysledek
    double vysledek_old; //stary vysledek, uchovavam kvuli porovnani v podmince (presnost)
    if (x < 0){ // kontrola ze zadane cislo je kladne
        return 999;
        }
    else if (x == 1.0){
        return 1;
        }
    else if (x == 0){
        return 0;
        }

    do // cyklus pro vypocet odmocniny pomoci Newtona
    {
        vysledek_old = vysledek; // ulozim si do stareho vysledku hodnotu pro pozdejsi porovnani
        vysledek = (0.5 * ( (x/vysledek) + vysledek)); // vypocet noveho vysledku pomoci newtonovy rady
    } while ( d_abs(vysledek - vysledek_old) >= eps*d_abs(vysledek)); // kontroluju, zda-li prirustek neni tak maly, ze uz je menesi jak presnost a tim padem mam skoncit
    return vysledek;
}

double my_asin(double x) // funkce pro vypocet arcus sinus z danneho cisla
{
    int acosMode = 0; //pokud je hodnota mezi 0.8 a 1 pocitam pomoci tzv. acosMode
    if (d_abs(x) >= 0.8 && d_abs(x) < 1) //zjisteni jestli acosMode bude nastaven na 1
    {
        acosMode = 1;
        x = my_sqrt(1-x*x);
    }
    double citatel = 1;
    double jmenovatel = 2;
    double podx = 3;
    double podx_old = 1;
    double mezivysledek = (citatel/jmenovatel)*(x*x*x/3); // druhy clen taylorova rozvoje
    double vysledek = x + mezivysledek; //prvni krok pro pozdejsi snazsi implementaci
    if(x == 1) //pokud je argument zadan jako 1, je snazsi rovnou vratit presnou hodnotu
    {
        vysledek = 1.5707963268;
        return vysledek;
    }
    else if(x == (-1)) ////pokud je argument zadan jako -1, je snazsi rovnou vratit presnou hodnotu
    {
        vysledek = -1.5707963268;
        return vysledek;
    }
    else if (x == 0)
    {
        vysledek = 0;
        return vysledek;
    }
    else if(x > 1 || x < -1) // argument pro arcus sinus musi byt v rozmezi od <-1,1>
    {
        return 999;
    }
    while( d_abs(mezivysledek) >= eps*d_abs(vysledek)) //kontrola presnosti
    {
        mezivysledek = mezivysledek * (podx_old+=2) * (  (citatel+=2)/(jmenovatel+=2) * (x*x) / (podx +=2) ); //vypocet noveho clenu rozvoje
        vysledek += mezivysledek;        //aktualizace vysledku
    }
    if (acosMode == 1) //je acosMode 1? Jestli ano vrat vysledek za pomoci prevodu zpet na arcus sinus (diky prevodu vyse jsem pocital arcus cosinus)
    {
        vysledek = ((PI/2) - vysledek); //zakladni vztah mezi arcus sinus a arcus cosinus - asin(x) + acos(x) = pi/2
        return vysledek;
    }
    else
    {
        return vysledek;
    }
}

void vypoctiStrany(double AX,double AY,double BX,double BY,double CX,double CY,double *a, double *b, double *c) //funkce pro vypocet delky stran podle zadanych bodu a jejich souradnic. K vypoctu se vyuzivaji zakladni operace s vektory
{
    *a = my_sqrt((CX - BX)*(CX - BX)+(CY - BY)*(CY - BY)); //aplikovani vektorovych zakonu
    *b = my_sqrt((CX - AX)*(CX - AX)+(CY - AY)*(CY - AY)); //aplikovani vektorovych zakonu
    *c = my_sqrt((BX - AX)*(BX - AX)+(BY - AY)*(BY - AY)); //aplikovani vektorovych zakonu
    return;
}

double vypoctiUhel(double a, double b, double c) //funkce pro vypocet odmocniny za vyuziti cosinovi vety
{
    double uhel = ( (PI/2) - my_asin((a*a+b*b-c*c)/(2*a*b)) ); // vyuzivam vztah, ze cosinus na minus prvni je arcus cosinus a ze arcus cosinus je (pi/2 - arcus sinus)
    return uhel;
}

int modeSelect(int sqrtMode, int asinMode, int triangleMode, char **argv) //funkce, ktera provede prislusny mod zadany uzivatelem
{
    if(sqrtMode == 1) //odmocnina
    {
        double cislo = strtod(argv[2], NULL);
        if (strcmp(argv[2], "inf") == 0)
        {
            printf("inf\n");
            return 0;
        }
        if( (cislo == 0) && strcmp(argv[2], "0") == 1)
        {
            fprintf(stderr, "Chyba, argument neni cislo\n");
            return 999;
        }
        double vysledek = my_sqrt(cislo);
        if ( vysledek == 999)
            printf("nan\n");
        else
            printf("%.10e\n", vysledek );
    }
    else if (asinMode == 1) //arcus sinus
    {
        double cislo = strtod(argv[2], NULL);
        if( (cislo == 0) && strcmp(argv[2], "0") == 1)
        {
            fprintf(stderr, "Chyba, argument neni cislo\n");
            return 999;
        }
        double vysledek = my_asin(cislo);
        if ( vysledek == 999)
            printf("nan\n");
        else
            printf("%.10e\n", vysledek );
    }
    else if(triangleMode == 1) //vypocet uhlu
    {
        double AX, AY, BX, BY, CX, CY, a, b, c, alpha, beta, gamma;
        AX = strtod(argv[2], NULL);
        AY = strtod(argv[3], NULL);
        BX = strtod(argv[4], NULL);
        BY = strtod(argv[5], NULL);
        CX = strtod(argv[6], NULL);
        CY = strtod(argv[7], NULL);
        vypoctiStrany(AX, AY, BX, BY, CX, CY, &a, &b, &c);
        if( ( (a+b) <= c) || ( (a+c) <= b) || ( (b+c) <= a) )
        {
            printf("Neplatne zadane body, nejedna se o trojuhelnik.\n");
            return 999;
        }
        gamma = vypoctiUhel(a,b,c);
        beta = vypoctiUhel(a,c,b);
        alpha = vypoctiUhel(c,b,a);
        printf("%.10e\n", alpha);
        printf("%.10e\n", beta);
        printf("%.10e\n", gamma);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int sqrtMode = 0; //tento a 2 radky pod nim obsahuji deklaraci jednotlivych modu
    int asinMode = 0;
    int triangleMode = 0;
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help"))){ //kontrola jestli uzivatel zadal parametry
        fprintf (stderr, "Nebyly zadany spravne parametry, zadejte parametr --help pro napovedu\n"); //kontrola jestli uyivatel zadal parametry
        return 2;
    }
    if (999 == parametrCheck(argc,argv,&sqrtMode, &asinMode, &triangleMode))
        return 3;
    if (999 == modeSelect(sqrtMode, asinMode, triangleMode, argv))
        return 4;

    return 0;
}

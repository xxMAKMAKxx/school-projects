#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* **************************************************** */
/* * *           Vypocet uhlu trojuhelniku          * * */
/* * *                                              * * */
/* * *                Projekt 3 IZP                 * * */
/* * *                    BETA                      * * */
/* * *              Martin Kacmarcik                * * */
/* * *                                              * * */
/* **************************************************** */

typedef struct {
  int rows;
  int cols;
  char *cells;
} Bitmap;

char *alokujMatici2D(Bitmap *b, int *radky, int *sloupce) //funkce pro alokaci 2D matice
{
    if ((b->cells = malloc((*radky)*(*sloupce)*sizeof(char))) == NULL) //kontrola zda se povedl malloc plus samotne volani malocu
    {
        fprintf(stderr, "Chyba pri alokovani matice");
    }
    if (b->cells != NULL) //pokud se alokace povedla nastav rows a cols na radky a sloupce
    {
        b->rows = *radky;
        b->cols = *sloupce;
    }
    return b->cells;
}

void dealokujMatici2D(Bitmap *b) //funkce, ktera dealokuje matici
{
    if ( (b != NULL) && (b->cells != NULL) ) //jestli matice existuje dealokuj
    {
        free(b->cells); //uvolni pamet
        b->cells = NULL; //nastavi vnitrnosti struktury na nulove hodnoty
        b->rows = 0; //nastavi vnitrnosti struktury na nulove hodnoty
        b->cols = 0; //nastavi vnitrnosti struktury na nulove hodnoty
    }
    else
        fprintf(stderr, "Chyba pri dealokovani matice, matice neexistuje"); //pokud matice neexistuje program uzivatele upozorni
}

char getcolor(Bitmap *bitmap, int x, int y) //funkce getcolor vraci znak, ktery ja na souradnicich [x,y] predavaneho pole
{
    //funkce getcolor nekontroluje x a y protoze tato kontrola je uz provadena volajicimi funkcenemi!!!
    char color = (bitmap->cells[x*bitmap->cols + y]);
    return color;
}

int nactiMaticiZeSouboru(FILE *soubor, Bitmap *b) //program ktery naplni obsah matice prislusnou matici v soburou
{
    int counter = 0;
    int q, i;
    int x;
    for(i= 0; i< (b->rows); i++) //projizdim radky
    {
        for(q = 0; q < (b->cols); q++ ) //projizdim sloupce
        {
           if ( fscanf(soubor, "%d",&x) == 1) //nascanuji co je v sobuoru
               {
                   counter++;
                   if(x == 1)
                    {
                        b->cells[i*b->cols + q] = '1'; //zapisu co je v souboru
                    }
                    else if(x == 0)
                    {
                        b->cells[i*b->cols + q] = '0'; //zapisu co je v souboru
                    }
               }
            else
            {
                fprintf(stderr, "Chyba, matice je neplatna");
                return 1;
            }
        }
    }
    while(fscanf(soubor, "%d",&x) == 1) //tento while cyklus pokracuje az do konce souboru a pocita dalsi znaky
    {
        counter++;
    }
    if(( (b->rows) * (b->cols)) != counter) //kontrola zda je matice opravdu validni, zda pocet prvku se shoduje s poctem udavanych prvku
    {
        fprintf(stderr, "Chyba, matice je neplatna");
        return 1;
    }
    return 0;
}

int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2) //program pro nalezeni nejvetsi vertikalni usecky
{
    int vracejiciHodnota = 0;
    int q, i, z = 1, pokracuj = 1;
    int nejvetsiVline = 0;
    int lajna = 0;
    int xpomocne = -1;
    int ypomocne = -1;
    for(i= 0; i< (bitmap->rows); i++) //projizdim radky
    {
        for(q = 0; q < (bitmap->cols); q++ ) //projizdim sloupce
        {
            pokracuj = 1;
            if(getcolor(bitmap, i, q) == '1')
            {
                xpomocne = i;
                ypomocne = q;
                z = q;
                while(pokracuj)
                {
                    if((z == bitmap->cols) || (getcolor(bitmap, i, z) == '0'))
                    {
                        if(lajna > nejvetsiVline) //jestli je lajna aktualne nejvetsi, aktualizuju nejvetsi lajnu
                        {
                            nejvetsiVline = lajna;
                            *x1 = xpomocne;
                            *y1 = ypomocne;
                            *x2 = i;
                            *y2 = --z;
                            vracejiciHodnota = 1;
                        }
                        lajna = 0;
                        pokracuj = 0;
                    }
                    else
                    {
                        lajna++;
                    }
                    z++;
                    }
            }
        }
    }
    return vracejiciHodnota;
}


int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)  //program pro nalezeni nejvetsi vertikalni usecky, pracuju stejne jako vline ovsem ma obracene prochazeni matice.
{
    int vracejiciHodnota = 0;
    int q, i, z = 1, pokracuj = 1;
    int nejvetsiVline = 0;
    int lajna = 0;
    int xpomocne = -1;
    int ypomocne = -1;
    for(i= 0; i< (bitmap->rows); i++) //projizdim radky
    {
        for(q = 0; q < (bitmap->cols); q++ ) //projizdim sloupce
        {
            pokracuj = 1;
            if(getcolor(bitmap, i, q) == '1') //pokud najdu 1 hledam vertikalne dalsi znaky
            {
                xpomocne = i;
                ypomocne = q;
                z = i;
                while(pokracuj)
                {
                    if((z == bitmap->rows) || (getcolor(bitmap, z, q) == '0'))
                    {
                        if(lajna > nejvetsiVline) //jestli je lajna aktualne nejvetsi, aktualizuju nejvetsi lajnu
                        {
                            nejvetsiVline = lajna;
                            *x1 = xpomocne;
                            *y1 = ypomocne;
                            *x2 = --z;
                            *y2 = q;
                            vracejiciHodnota = 1;
                        }
                        lajna = 0;
                        pokracuj = 0;
                    }
                    else
                    {
                        lajna++;
                    }
                    z++;
                    }
            }
        }
    }
    return vracejiciHodnota;
}


int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2) //funkce pro hledani nejvetsiho ctverce v matici. Ctverec je ohraniceni z cernych pixelu
{
    int poprve = 0;
    int velikostCtverce = 1;
    int velikostNejvetsihoCtverce = 1;
    int jeToCtverec = 0;
    int neNarazilJsemNaNulu = 1;
    int vracejiciHodnota = 0;
    int i, q, g;
    int posunuti = 1;
    for(i= 0; i< (bitmap->rows); i++) //projizdim radky
    {
        for(q = 0; q < (bitmap->cols); q++ ) //projizdim sloupce
        {
             if( getcolor(bitmap,i,q) == '1') //toto je to, ze projde vsechnz pixelky, jestli netvori s necim ctverec
             {
                 if (poprve == 0) //pokud mam matici s pouze se ctvercem o velikosti 1 tento if zajisti, ze ctverec prvni cerny pixel je nastaven jako nejvetsi ctverec
                 {
                    *x1 = i;
                    *y1 = q;
                    *x2 = i;
                    *y2 = q;
                    vracejiciHodnota = 1;
                    poprve = 1;
                 }
                 velikostCtverce = 1;
                 while(neNarazilJsemNaNulu) //prochazim sousedni pixelky dokud nenarazim na nulu
                 {
                    if ((i+posunuti) < bitmap->rows && (q+posunuti) < bitmap->cols && getcolor(bitmap, (i+posunuti), q) == '1' &&  getcolor(bitmap, i, (q+posunuti)) == '1' )
                    { // jestlize najdu ctverec od pixelku dole a v pravo, zkontroluju, jestli jsou aj na rovnobezkach cerne pixelky
                        jeToCtverec = 1;
                        for(g = posunuti; g > 0; g--)
                        {
                            if ( getcolor(bitmap, (i+g), (q+posunuti)) == '0') //kontrola rovnobezek
                            {
                                jeToCtverec = 0;
                            }
                            if(getcolor(bitmap, (i+posunuti), (q+g)) == '0') //kontrola rovnobezek
                            {
                                jeToCtverec = 0;
                            }
                        }
                        if(jeToCtverec == 1) //jestlize ctverec najdu, nastavim prislusne souradnice
                        {
                            velikostCtverce += posunuti;
                            if(velikostCtverce > velikostNejvetsihoCtverce) //ovsem jen za predpokladu, ze nalezeny ctverec je nejvetsi nalezeny ctverec
                            {
                                velikostNejvetsihoCtverce = velikostCtverce;
                                *x1 = i;
                                *y1 = q;
                                *x2 = (i+posunuti);
                                *y2 = (q+posunuti);
                                vracejiciHodnota = 1;
                            }
                        }
                        posunuti++;
                    }
                    else
                        neNarazilJsemNaNulu = 0; //pokud najdu nulu tak skoncim. Diky zpusobu algoritmu mam nejvetsi nalezeny ctverec ulozeny
                 }
                 neNarazilJsemNaNulu = 1;
                 posunuti = 1;
             }
        }
    }
    return vracejiciHodnota;
}

void vypisNapovedu() //funkce pro vypsani napovedy
{
    printf("__________________________________________________\n\n"
           "Program pro hledani obrazcu v matici\n"
           "By Martin Kacmarcik (c) 2013\n"
           "__________________________________________________\n\n"
           "misto soubor.txt zadejte nazev vaseho souboru v textovem formatu txt"
           "Zadejte parametr --test soubor.txt pro kontrolu validni matice.\n"
           "Zadejte --hline soubor.txt pro hledani nejdelsi cerne horizontalni usecky v matici\n"
           "Zadejte --vline soubor.txt pro hledani nejdelsi cerne vertikalni usecky v matici\n"
           "Zadejte --square soubor.txt pro hledani nejvetsiho cerneho ctverce v matici\n\n"
           "Validni matice je takova, ktera ma nazacatku dve cisla oznacujici pocet radku a potom kazde cislo zvlast oznacujici barvu v matici, kdy 1 je cerna barva a 0 je bila barva. Kazde cislo, barva etc. jsou oddeleny bilymi znaky \n"
           );
}

void testMatice(FILE *soubor, Bitmap *b) //program ktery pouze otestuje matici, ale neulozi ji. Vraci Valid pokud je spravne a Invalid pokud je spatne.
{
    int counter = 0;
    int q, i;
    int x;
    for(i= 0; i< (b->rows); i++) //projizdim radky
    {
        for(q = 0; q < (b->cols); q++ ) //projizdim sloupce
        {
           if ( fscanf(soubor, "%d",&x) == 1) //pokud najdu cislo zvetsim counter
               {
                   counter++;
               }
            else
            {
                fprintf(stderr, "Invalid\n");
                return;
            }
        }
    }
    while(fscanf(soubor, "%d",&x) == 1) //dopocitani zbytku znaku
    {
        counter++;
    }
    if(( (b->rows) * (b->cols)) != counter) //shoduje se pocet znaku s udavanym poctem znaku?
    {
        fprintf(stderr, "Invalid\n");
        return;
    }
    else
        printf("Valid\n");
    return;
}

int parametrCheck(int argc,char **argv, int *testMode, int *hlineMode, int *vlineMode, int *squareMode) //funkce ktera nastavi danny mod v zavislosti na parametru, ktery zadal uzivatel
{
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                vypisNapovedu();
                return 1;
            }
        if (strcmp(argv[i], "--test") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *testMode = 1;
            }
        if (strcmp(argv[i], "--hline") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *hlineMode = 1;
            }
        if (strcmp(argv[i], "--vline") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
            {
                *vlineMode = 1;
            }
        if (strcmp(argv[i], "--square") == 0) //porovnava pomoci funkce strcmp, pokud funkce vrati 0 nastavi prislusny mod
        {
            *squareMode = 1;
        }
    }
    return 0;
}

int modeSelect(int *testMode, int *hlineMode, int *vlineMode, int *squareMode, FILE *soubor, int *radky, int *sloupce) //funkce ktera vybere prislusny mod.
{
    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;
    if(*testMode == 1)
    {
        Bitmap b;
        alokujMatici2D(&b, radky, sloupce);
        testMatice(soubor, &b);
        dealokujMatici2D(&b);
    }
    else if(*hlineMode == 1)
    {
        Bitmap b;
        alokujMatici2D(&b, radky, sloupce);
        if ( nactiMaticiZeSouboru(soubor, &b) == 1)
        {
            return 3;
        }
        if (find_hline(&b, &x1, &y1, &x2, &y2) == 1)
        {
            printf("%d %d %d %d\n", x1, y1, x2, y2);
        }
        else
        {
            fprintf(stderr, "Nenalezeno nic.");
        }
        dealokujMatici2D(&b);
    }
    else if(*vlineMode == 1)
    {
        Bitmap b;
        alokujMatici2D(&b, radky, sloupce);
        if ( nactiMaticiZeSouboru(soubor, &b) == 1)
        {
            return 3;
        }
        if (find_vline(&b, &x1, &y1, &x2, &y2) == 1)
        {
            printf("%d %d %d %d\n", x1, y1, x2, y2);
        }
        else
        {
            fprintf(stderr, "Nenalezeno nic.\n");
        }
        dealokujMatici2D(&b);
    }
    else if(*squareMode == 1)
    {
        Bitmap b;
        alokujMatici2D(&b, radky, sloupce);
        if ( nactiMaticiZeSouboru(soubor, &b) == 1)
        {
            return 3;
        }
        if (find_square(&b, &x1, &y1, &x2, &y2) == 1)
        {
            printf("%d %d %d %d\n", x1, y1, x2, y2);
        }
        else
        {
            fprintf(stderr, "Nenalezeno nic.\n");
        }
        dealokujMatici2D(&b);
    }
    else
    {
        fprintf(stderr,"Spatne zadane parametry, zadejte parametr --help pro napovedu\n");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int radky = 0;
    int sloupce = 0;
    int squareMode = 0;
    int hlineMode = 0;
    int vlineMode = 0;
    int testMode = 0;
    if (parametrCheck(argc, argv, &testMode, &hlineMode, &vlineMode, &squareMode) == 1) //pokud uzivatel chtel jen napovedy skonci
        return 1;


    FILE *soubor;
    if( (soubor = fopen(argv[2], "r")) == NULL) //otevreni souboru a kontrola zda probehlo otevreni uspesne
	{
        fprintf(stderr, "Chyba pri otevreni souboru\n");
		return 2;
	}
    fscanf(soubor, "%d",&(radky)); //nacti radky
    fscanf(soubor, "%d",&(sloupce)); //nacti sloupce
    if( (radky < 1) || (sloupce < 2)) //kontrola zda je matice vhodna podle zadani. Viz printf.
        fprintf(stderr, "Matice je chybne zadana. Nejmensi sirka by mela byt dva a nejmensi delka by mela byt 1\n");
    modeSelect(&testMode, &hlineMode, &vlineMode, &squareMode, soubor, &radky, &sloupce); //modSelect si prebere rizeni a podle modu vybere akci
    fclose(soubor); //zavreni souboru, dulezite
    return 0;
}

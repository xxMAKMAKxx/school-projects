/************************************************************
*														    *
* Projekt do IMS										    *
* Tema: Modelovani a simulace dopravniho uzlu 				*
*	    Pro nas pripad Zeleznicni stanice - Brnno Hl.n.		*
* Autori: Martin Kacmarcik (xkacma03)						*
*		  Klara Mihalikova (xmihal05)						*
*															*
*************************************************************/

/* Include sekce */

#include <string>
#include "simlib.h"
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>


/* Sekce s konstantami */

#define HODINA 3600
#define MINUTA 60
#define SEKUNDA 1
#define PRIJEZD_ODJEZD 38*SEKUNDA //cca 38 sekund trva nez dojede vlak 30 km/h rychlosti od napojeni koleje na nastupiste

//cekani
#define PRUMERNA_DOBA_CEKANI_RYCHLIK 13*MINUTA //doba cekani rychliku na nastupisti
#define PRUMERNA_DOBA_CEKANI_EC_P 3.5*MINUTA //doba cekani eurocity na nastupisti # projizdi
#define PRUMERNA_DOBA_CEKANI_EC_Z 21*MINUTA //doba cekani eurocity na nastupisti  # zacina
#define PRUMERNA_DOBA_CEKANI_OS_P 4.4*MINUTA //doba cekani osobaku na nastupisti  # projizdi
#define PRUMERNA_DOBA_CEKANI_OS_Z 20*MINUTA //doba cekani osobaku na nastupisti   # zacina

//procenta zacinajicich
#define PRUMER_ZACINAJICI_EC 10
#define PRUMER_ZACINAJICI_OS 25

//priority
#define RYCHLIK_PRIORITY 2
#define EC_PRIORITY 3
#define OS_PRIORITY 1


//spozdeni vlaku
//procentualne
#define RYCHLIK_SPOZDENI_PROCENT 90
#define EC_SPOZDENI_PROCENT 90
#define OS_SPOZDENI_PROCENT 60
//prumerna doba spozdeni
#define SPOZDENI_PRUMER_RYCHLIK 4*MINUTA
#define SPOZDENI_PRUMER_EC 5*MINUTA
#define SPOZDENI_PRUMER_OS 1*MINUTA

//generovani vlaku
#define GENEROVANI_RYCHLIKU_DEFAULT HODINA/2-100
#define GENEROVANI_EC_DEFAULT HODINA/(3/2)-100
#define GENEROVANI_OS_DEFAULT HODINA/8-100

//Testovaci promenne
int rychlik = 0;
int ec = 0;
int os = 0;
int test = 0;

//promenne urcujici generovani vlaku
int generovani_rychliku = GENEROVANI_RYCHLIKU_DEFAULT;
int generovani_ec = GENEROVANI_EC_DEFAULT;
int generovani_os = GENEROVANI_OS_DEFAULT;

//promenne urcujici spozdeni vlaku
int spozdeni_rychliku = SPOZDENI_PRUMER_RYCHLIK;
int spozdeni_ec = SPOZDENI_PRUMER_EC;
int spozdeni_os = SPOZDENI_PRUMER_OS;

//doba behu simulace
double runtime = 1;

//spozdeni vyp/zap
bool spozdeni_bool = true;
int flag_max_spozdeni = 0;
int max_spozdeni_r = 0;
int max_spozdeni_ec = 0;
int max_spozdeni_os = 0;



/***************************/
/* Sekce nastupist         */
/***************************/


Store Koleje_store("Koleje", 6); //pocet koleji na hlavnim nadrazi

/****************************/
/* Sekce generovani procesu */
/****************************/

class Rychlik : public Process
{
	void Behavior()
	{
		Priority = RYCHLIK_PRIORITY;
		//Nektere vlaky maji spozdeni
		if(spozdeni_bool == true)
		{
			//Zjisteni, zda li ma vlak spozdeni #pravdepodobnost
			if(Uniform(0,100) <= RYCHLIK_SPOZDENI_PROCENT)
			{
				//male spozdeni
				if(flag_max_spozdeni == 0)
				{
					if(Uniform(0,100) <= RYCHLIK_SPOZDENI_PROCENT)
					{
						//printf("Rychlik ma spozdeni: %d\n", spozdeni_rychliku);
						Wait(spozdeni_rychliku);
					}	
				}
				//velke spozdeni
				else
				{
					int spozdeni = Uniform(0, max_spozdeni_r);
					//printf("Rychlik ma spozdeni: %d\n", spozdeni);
					Wait(spozdeni*MINUTA);
				}
			}
		}
		//Obsad prvni volnou kolej 
		Enter(Koleje_store, 1);
		//Cekej nez dojedes na kolej
		Wait(PRIJEZD_ODJEZD);
		//Cekej nez probehne nastup
		Wait(PRUMERNA_DOBA_CEKANI_RYCHLIK);	
		//Cekej nez odjedes z koleje
		Wait(PRIJEZD_ODJEZD);
		//Uvolni kolej	
		Leave(Koleje_store, 1);
	}
};

class Eurocity : public Process
{
	void Behavior()
	{
		Priority = EC_PRIORITY;
		//Nektere vlaky maji spozdeni
		if(spozdeni_bool == true)
		{
			//Zjisteni, zda li ma vlak spozdeni #pravdepodobnost
			if(Uniform(0,100) <= RYCHLIK_SPOZDENI_PROCENT)
			{
				//male spozdeni
				if(flag_max_spozdeni == 0)
				{
					if(Uniform(0,100) <= EC_SPOZDENI_PROCENT)
					{
						Wait(spozdeni_ec);
					}	
				}
				//velke spozdeni
				else
				{
					int spozdeni = Uniform(0, max_spozdeni_ec);
					//printf("EC ma spozdeni: %d\n", spozdeni);
					Wait(spozdeni*MINUTA);
				}
			}
		}
		//Obsad prvni volnou kolej 
		Enter(Koleje_store, 1);
		//Cekej nez dojedes na kolej
		Wait(PRIJEZD_ODJEZD);
		//Cekej nez probehne nastup
		if(Uniform(0,100) <= PRUMER_ZACINAJICI_EC)
		{
			Wait(PRUMERNA_DOBA_CEKANI_EC_Z);
		}	
		else
		{
			Wait(PRUMERNA_DOBA_CEKANI_EC_P);
		}
		//Cekej nez odjedes z koleje
		Wait(PRIJEZD_ODJEZD);
		//Uvolni kolej	
		Leave(Koleje_store, 1);
		
	}
};

class Osobak : public Process
{
	void Behavior()
	{
		Priority = OS_PRIORITY;
		//Nektere vlaky maji spozdeni
		if(spozdeni_bool == true)
		{
			//Zjisteni, zda li ma vlak spozdeni #pravdepodobnost
			if(Uniform(0,100) <= RYCHLIK_SPOZDENI_PROCENT)
			{
				//male spozdeni
				if(flag_max_spozdeni == 0)
				{
					if(Uniform(0,100) <= OS_SPOZDENI_PROCENT)
					{
						Wait(spozdeni_os);
					}	
				}
				//velke spozdeni
				else
				{
					int spozdeni = Uniform(0, max_spozdeni_os);
					Wait(spozdeni*MINUTA);
				}
			}
		}
		//Obsad prvni volnou kolej 
		Enter(Koleje_store, 1);
		//Cekej nez dojedes na kolej
		Wait(PRIJEZD_ODJEZD);
		//Cekej nez probehne nastup
		if(Uniform(0,100) <= PRUMER_ZACINAJICI_OS)
		{
			Wait(PRUMERNA_DOBA_CEKANI_OS_Z);
		}	
		else
		{
			Wait(PRUMERNA_DOBA_CEKANI_OS_P);
		}
		//Cekej nez odjedes z koleje
		Wait(PRIJEZD_ODJEZD);
		//Uvolni kolej	
		Leave(Koleje_store, 1);
	}
};



/***************************/
/* Sekce generovani vlaku  */
/***************************/



//generovnai rychliku
class Generator_rychlik : public Event {
	void Behavior(){
		(new Rychlik)->Activate();
		rychlik++;
		Activate( Time + generovani_rychliku + Exponential(100));
	}	
};
//generovnai ec
class Generator_ec : public Event {
	void Behavior(){

		(new Eurocity)->Activate(); 
		ec++;
		Activate( Time + generovani_ec + Exponential(100));

	}	
};
//generovnai osobkau
class Generator_os : public Event {
	void Behavior(){
		(new Osobak)->Activate();
		os++;		
		Activate( Time + generovani_os + Exponential(100));
	}	
};


int main(int argc, char *argv[])
{
	std::string output_soubor = "Output_Nadrazi.out";
	//Zpracovani argumentu, f = false
	//               0.        1.             2.            3.          4.        5.         6.         7.            8.                9.             10.         argument
	//argumenty: ./model PocetNastupist SpozdeniRychliku SpozdeniEC SpozdeniOS Runtime Generovani_R Generovani_EC Generovani_OS   spozdeni_bool   nazev_outputu
	//pocet nastupist
	if(argc >= 2)
	{
		if(argv[1][0] != 'f')
			Koleje_store.SetCapacity(atol(argv[1]));
	}
	//pridani spozdeni
	if(argc >= 3)
	{
		
		if(argv[2][0] != 'f')
		{
			flag_max_spozdeni = 1;
			max_spozdeni_r = atol(argv[2]);
		}
		if(argv[3][0] != 'f')
			max_spozdeni_ec = atol(argv[3]);
		if(argv[4][0] != 'f')
			max_spozdeni_os = atol(argv[4]);
	}
	//pridani vlaku (posily, svatky)
	if(argc >= 6)
	{
		if(argv[5][0] != 'f')
			runtime = atol(argv[5]);
	}
	if(argc >= 7)
	{
		if(argv[6][0] != 'f')
			generovani_rychliku = HODINA/atol(argv[6])-100;
		if(argv[7][0] != 'f')
			generovani_ec = HODINA/atol(argv[7])-100;
		if(argv[8][0] != 'f')
			generovani_os = HODINA/atol(argv[8])-100;
	}
	if(argc >= 10)
		if(argv[9][0] != 'f')
		{
			if(atol(argv[9]) == 1)
				spozdeni_bool = true;
			else
				spozdeni_bool = false;
		}
	//posledni param
	if(argc >= 11)
	{
		output_soubor = argv[10];
	}
	
	//Samotna simulace
	SetOutput(output_soubor.c_str());
	Init(0,HODINA*runtime);
	(new Generator_rychlik)->Activate();
	(new Generator_ec)->Activate();
	(new Generator_os)->Activate();
	Run();
	//Vypis vystupu
	//printf("R: %d, EC: %d, OS: %d Test: %d celkem: %d\n", rychlik, ec, os, test, (rychlik+ec+os));
	Koleje_store.Output();
}
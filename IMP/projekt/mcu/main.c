//Martin Kacmarcik xkacma03 IMP projekt 90% - pouzita funkce SetLine z projektu Rasterizace usecek
//Jednoduchy Voltmeter s vyuzitim zobrazeni graficky pres VGA

/*******************************************************************************
   main.c: main application for voltmeter
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Martin Kacmarcik <xkacma03@stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <msp430x16x.h>
#include <signal.h>
#include <stdio.h>
#include <fitkitlib.h>
#include "adc.h"
#include "main.h"
#include <stdlib.h>

#define MAX_X 511	//maximalni X pro usecky
#define MAX_Y 479   //maximalni Y pro usecky
#define POCET_VZORKU 50 //pocet vzorku ktere vykreslim
#define DELIC_Y 11  //hodnota, ktera po vydeleni prevede MV na hodnotu vhodnou pro nase meritko

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd){}
void print_user_help(void){}
void fpga_initialized(){}


unsigned long value, desetina_cast, cela_cast, mv; //value -> hodnota z ADC, MV = milivolty
int isHigh = 1; // je vysoke napeti? 0 ne, 1 ano; 1 is default
int arrayPointer = 0; //ukazatel do pole hodnot
unsigned long arrayOfValues[POCET_VZORKU]; //pole vysledku z ADC


//nastavi pro dalsi mereni napeti
void chooseVoltage()
{
  //pokud je napeti vysoke a hodnota je mensi nez 2047, sniz napeti pro vyssi presnost
  if (isHigh && value <= 2048)
  {
    isHigh = 0; //flag abych vedel jake napeti mam
  }
  //pokud je napeti nizke, zvys napeti
  else if( !isHigh && value >= 4090)
  {
	isHigh = 1;
  }
}

//vypise do terminalu pocet voltu
void processResult()
{
    //vypocti vysledek na zaklade napeti
	if(isHigh)
	{
		cela_cast = value / 819;
		desetina_cast = ((value*1000/819)%1000);
		term_send_num(cela_cast);
		term_send_str(".");
		term_send_num(desetina_cast);
		term_send_str_crlf(" V");
	}
	else
	{
		cela_cast = value / 1365;
		desetina_cast = ((value*1000/1365)%1000);
		term_send_num(cela_cast);
		term_send_str(".");
		term_send_num(desetina_cast);
		term_send_str_crlf(" V");
	}
	//spocitej hodnotu v milivoltech (kvuli presnosti pro draw)
	mv = cela_cast*1000 + desetina_cast;
	//pridej do pole hodnotu delenou DELIC_Y
	arrayOfValues[arrayPointer] = mv/DELIC_Y;
	//zvys ukazatel do pole a kdyz bude vetsi jak 50 tak ho vrat na nulu
	arrayPointer++;
	if(arrayPointer > 50)
		arrayPointer = 0;
	//vykresli vysledek
	drawResult();
}

//vymaze obrazovku pomoci cernych car
void ClearScreen()
{
  int y;
  //kresli cerne cary od hora dolu vodorovne
  for (y = 0; y <= MAX_Y; y++)
  {
	SetLine(0,y,MAX_X,y,0);
  }
  for (y = 0; y <= MAX_Y; y++)
  {
	SetLine(0,y,MAX_X,y,0);
  }
  for (y = 0; y <= MAX_Y; y++)
  {
	SetLine(0,y,MAX_X,y,0);
  }
}


//funkce ktera vykresli vysledek
void drawResult()
{
	//Vyprazdni obrazovku
	ClearScreen();
	//Vykresli graf, vykresluj od nejnovejsi hodnoty po nejstarsi
	int i, x = MAX_X, y, previousX, previousY, actualIndex;
	for ( i = 1; i <= 50; i++ )
	{
	  x -= 10; //skok po ose x 50 vzorku
	  actualIndex = arrayPointer - i; //spocitej index pro tuto iteraci cyklu
	  //pokud je index mensi jako nula, pricti 50 aby ses zase dostal do rozmeru pole (pole ma 50 prvku) 
	  if(actualIndex < 0)
	    actualIndex += POCET_VZORKU;
	  //inverze os (prakticky 0 je ted dole a MAX_Y je nahore) ; -8 je tam aby to slo videt, kdyz je hodnota nizko
      y = (MAX_Y-8) - arrayOfValues[actualIndex];
	  //pri prvni iteraci nic nekresli, potrebujes 2 body abys vykreslil
	  if(i != 1)
	    SetLine(previousX,previousY, x,y,50);
      //uloz si hodnoty pro vykresleni dalsiho grafu
	  previousX = x;
	  previousY = y;
	}
}



//<AUTHOR>
//by Ladislav Capka - demo aplikace pro fitkit pro rasterizaci usecek
char SetLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char clr)
{
  unsigned char data[5];
  
  // Out of range
  if (x1 >= 512) return -1;
  if (y1 >= 480) return -1;
  if (x2 >= 512) return -1;
  if (y2 >= 480) return -1;
  
  data[5] = x1 & 0xFF;                               // FPGA (7 downto 0)
  data[4] = ((x1 >> 8) & 0x01) + ((y1 << 1) & 0xFF); // FPGA (15 downto 8)
  data[3] = ((y1 >> 7) & 0x03) + ((x2 << 2) & 0xFF); // FPGA (23 downto 16)
  data[2] = ((x2 >> 6) & 0x07) + ((y2 << 3) & 0xFF); // FPGA (31 downto 24)
  data[1] = ((y2 >> 5) & 0xFF);                      // FPGA (39 downto 32)
  data[0] = clr;                                     // FPGA (47 downto 40)
 
  FPGA_SPI_RW_AN_DN(1, 0, &(data[0]), 1, 6);
 
  return 0;
}
//</AUTHOR>

int main(void)
{
  //inicializace hardware a zastaveni hlidaciho psa
  initialize_hardware();
  WDG_stop();

  //ziskej prvni hodnotu a podle ni uprav referencni napeti
  value = adcInit(isHigh);
  chooseVoltage(value);

  while(1){
    //ziskej novou hodnotu
    value = adcInit(isHigh);
	//zpracuj vysledek
	processResult();
	//uprav referencni napeti pokud je treba
	chooseVoltage(value);
	//pockej 30 ms - 33 Hz
	delay_ms(30);
    terminal_idle();
  }
}

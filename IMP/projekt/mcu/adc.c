//Soubor pro AD prevodnik, na projekt od xkacma03 (Martin Kacmarcik) 10% - funkce pouzita z demo aplikace theromometer, doplnena automaticka zmena ref. napeti
//Author(s): Vojtech Cizinsky <xcizin01 AT stud.fit.vutbr.cz>

#include <fitkitlib.h>
#include <msp430x16x.h>
#include <signal.h>
#include <stdbool.h>
#include "adc.h"

unsigned int value = 0; //zde si ulozim hodnotu vracenou AD prevodnikem
static unsigned int FirstADCVal;                      // holds 1st ADC result
int flag = 0; //flag kontrolujici dokonecnost prenosu

unsigned int adcInit(int isHigh){
   
   ADC12CTL0 = ADC12ON | SHT0_15 | MSH | REFON;  // ADC on, int. ref. on (1,5 V),
                                                 // multiple sample & conversion
   ADC12CTL1 = ADC12SSEL_2 | ADC12DIV_7 | CSTARTADD_0 | CONSEQ_1 | SHP;  

   ADC12MCTL0 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL1 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL2 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL3 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL4 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL5 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL6 = SREF_1 | INCH_0;                 // int. ref., channel 10
   ADC12MCTL7 = EOS | SREF_1 | INCH_0;           // int. ref., channel 10, last seg.
   
   //set voltage based on the reference
   if(isHigh)
     ADC12CTL0 |= REF2_5V;
   else
     ADC12CTL0 &= ~REF2_5V;
  
   ADC12CTL0 |= ENC;                              // povoleni prevodu
   ADC12CTL0 |= ADC12SC;                          // sample & convert

  while (ADC12CTL0 & ADC12SC);                   // cekani na prevod
  
  
  
   ADC12CTL0 &= ~ENC;      
   return ADC12MEM0;
}


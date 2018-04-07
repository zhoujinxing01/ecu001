/******************************************************************************* 
* File Name    : assist.c
* Version      : 1.0 
* Description  : This is source file for Pole drive.
* Designer     : zhou,jinxing
******************************************************************************/ 

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"

#include "app.h"
#include "config.h"
#include "iodefine.h"


volatile  uint8_t  	Tick_5ms;
volatile  uint16_t  	Tick_CAN;
volatile  uint16_t  	Tick_ADC;
volatile  uint16_t  	Tick_ADJ;
volatile  uint16_t  	Tick_HOL;
volatile  uint8_t  	Tick_PWR;
volatile  uint16_t  	Tick_BUZ;
volatile  uint16_t  	Tick_DIN;
volatile  uint16_t  	Tick_COM;
volatile  uint16_t  	Tick_UART;
volatile  uint16_t  	Tick_ECO;

void BaseTick(void)
{
	if(Tick_Base)//1ms
	{
		Tick_Base = 0;
		
    		if(Tick_PWR 	< Tick_PWR_ms)Tick_PWR++;
    		if(Tick_CAN 	< Tick_CAN_ms)Tick_CAN++;
    		if(Tick_ADC 	< Tick_ADC_ms)Tick_ADC++;
    		if(Tick_HOL 	< Tick_HOL_ms)Tick_HOL++;
    		if(Tick_BUZ 	< Tick_BUZ_ms)Tick_BUZ++;
    		if(Tick_DIN 	< Tick_DIN_ms)Tick_DIN++;
    		if(Tick_COM 	< 10000)      Tick_COM++;//10s
    		if(Tick_UART 	< 10000)      Tick_UART++;//10s
    		if(Tick_ADJ 	< 300)	      Tick_ADJ++;
		//Tick_ADJ:300ms timeout,only use in adjust.for distinguish Static and Dynamic.
    		if(Tick_ECO 	< Tick_ECO_ms)Tick_ECO++;
	}
}





  
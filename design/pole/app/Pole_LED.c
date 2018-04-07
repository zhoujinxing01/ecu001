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


volatile  uint8_t  	MCU_flag;

void MCU_Status(void)
{
	if(MCU_flag)
	{
		MCU_flag = 0;
		LED_ON();
	}
	else
	{
		MCU_flag = 1;
		LED_OFF();		
	}
}






  
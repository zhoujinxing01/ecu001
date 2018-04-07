/******************************************************************************* 
* File Name    : app.c
* Version      : 1.0 
* Description  : This is source file for Pole drive.
* Designer     : zhou,jinxing
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 03.29.2013 1.00     First Release 
******************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"
#include "app.h"
#include "config.h"

volatile  uint8_t  	System_ECO_TIM;



void Pole_System_ECO(void)
{
	uint8_t i;
	if(Tick_ECO >= Tick_ECO_ms)//1000ms
  	{
		//System_ECO_TIM++;
		//if((System_ECO_TIM > 6)&&(LOCK_STATUS == LOCK_STATUS_All))TJA1041_Sleep();//6s
		
		
		i = DIN.Value;
		i &= LockDownMask;		
		if(i == LockDown_Pull)
		{
			System_ECO_TIM++;
			if(System_ECO_TIM > 8)TJA1041_Sleep();//8s			
		}
		else System_ECO_TIM = 0;
		
		
		MCU_Status();
		
		
		Tick_ECO = 0;
	} 
}

void TJA1041_Normal(void)
{
  BIT_SET(P3,2);//EN
  BIT_SET(P7,1);//STB
}

void TJA1041_Sleep(void)
{
//  BIT_CLR(P3,2);//EN
  BIT_CLR(P7,1);//STB
}


/*
	1)It can be selected by the option byte whether the WDT-dedicated low-speed on-chip oscillator
	continues oscillating or stops in the HALT or STOP mode. 
*/
void Stop(void)
{
	/*
	1)The STOP mode can be used only when the CPU is operating on the main system clock.
	2)be sure to stop the peripheral hardware operation operating with main system clock 
	before executing STOP instruction (except for the setting unit of SNOOZE mode).
	*/
	//__stop();
	STOP();
	/*
	1)In the STOP mode, the high-speed system clock and high-speed on-chip oscillator stop
	2)a wait time is required to secure the oscillation stabilization time after the STOP mode is released when the
	X1 clock is selected, 
	select the HALT mode if it is necessary to start processing immediately upon interrupt request generation.
	*/
}
void Halt(void)
{
	//__halt();
	HALT();
	/*
	1)In the HALT mode, the CPU operation clock is stopped.
	2)If the highspeed system clock, high-speed on-chip oscillator, subsystem clock, or low-speed on-chip oscillator is operating
before the HALT mode is set, oscillation of each clock continues. 
	3)effective for restarting operation immediately upon interrupt request generation .
	*/
}


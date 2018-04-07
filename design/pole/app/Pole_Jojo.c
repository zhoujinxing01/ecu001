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
#include "r_rl78_can_drv.h"


volatile   struct JOJO_base 	JOJO;

void JOJO_Procedure(void)
{
  if(Tick_COM >= 10)//10ms
  {	
	Door_Procedure();
	if(JOJO.Track)JOJO_Track();
	
	Tick_COM = 0; 
  }
}

void JOJO_Track(void)
{
	switch(DOOR_STATUS)
	{	
		case DOOR_STATUS_Opening:	
		if(HOLL.LCNT >= JOJO.Next)
		{
			PoleSign.HollL = 1;
			JOJO.Track = 0;
		}
		break;
	
		case DOOR_STATUS_Closing:		
		if(HOLL.LCNT <= JOJO.Next)
		{	
			PoleSign.HollL = 1;	
			JOJO.Track = 0;
		}
		break;
	
		default :
			
		break;
	}		
}

void JOJO_CAN(void)
{
  uint16_t  i,j;
    
  	
       	switch(JOJO.Data[0])
       	{
		case 0x01:
			i =  JOJO.Data[1] * 256; 
			i += JOJO.Data[2];
			i += 20000;
			if(i <= HOLL_POSITION_MAX)HOLL.LCNT = i;
			else HOLL.LCNT = HOLL_POSITION_MAX;
			JOJO.Ratio = 10;
	  	break;
		
		case 0x02:
			i =  JOJO.Data[1] * 256; 
			i += JOJO.Data[2];
			j = HOLL_POSITION_MAX - 20000;
			if(j >= i)JOJO.Next = HOLL_POSITION_MAX - i;
			else JOJO.Next = 20000;

			if(JOJO.Next >= HOLL.LCNT)DOOR_STATUS = DOOR_STATUS_Open;	
			else DOOR_STATUS = DOOR_STATUS_Close;
			
			JOJO.Track = 1;
			PoleSign.KEY0 = 1;
			JOJO.Ratio = 10;
		break;
		
		case 0x03:
		PoleSign.KEY0 = 1;
		JOJO.Track = 0;
		JOJO.Ratio = 10;
		break;
		
		case 0x04:
		if(JOJO.Data[1])DOOR_STATUS = DOOR_STATUS_Open;
		else DOOR_STATUS = DOOR_STATUS_Close;
		if(JOJO.Data[2] <= 10)JOJO.Ratio = JOJO.Data[2];	
		else JOJO.Ratio = 10;
		
		PoleSign.KEY0 = 1;
		JOJO.Track = 0;
		break;	
		
		case 0x05:
		HOLL.LCNT = 20000;
		Buzzer_NUM = 2;
		JOJO.Ratio = 10;		
		break;
	 
		default :
		JOJO.Ratio = 10;
	  	break;
       }   
}

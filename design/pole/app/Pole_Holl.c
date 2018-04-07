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

volatile   struct HOLL_base 	HOLL;

//********************************************

void Holl_Logic(void)
{
  if(Tick_HOL >= Tick_HOL_ms)//10ms
  {
	Holl_EEPROM_Command();//must read the position from eeprom first!!
  	HollRefresh(); 
	
	//if(AntiNip.AntiNipCheck == 1)HollLegalityCheck();
	if(AntiNip.FlagTIM >= 60)HollLegalityCheck();
	
	HollPositionCheck();
	
  	Tick_HOL = 0;
  }
}


void HollRefresh(void)
{	
	HOLL.SenseTIM++;
	if(HOLL.SenseTIM > HOLL_SENSE)//HOLL_SENSE = 5
	{
		HOLL.SenseTIM = 0;
		
		if(HOLL.LCNT > HOLL.LPreCNT)
		{
			HOLL.LDIR = 1;
			HOLL.LSPD = HOLL.LCNT - HOLL.LPreCNT;
		}
		else 
		{
			HOLL.LDIR = 0;
			HOLL.LSPD = HOLL.LPreCNT - HOLL.LCNT;	
		}
		HOLL.LPreCNT = HOLL.LCNT;		
	}
}

/*
void HollLegalityCheck(void)
{
	switch(DOOR_STATUS)
	{	
		case DOOR_STATUS_Opening:
		if(HOLL.LSPD == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_SPDerrorTIM)PoleSign.HollL = 1; 
		//	AntiNip.Flag = 1;//see 'Pole_Adjust.c'
		}
		else if(HOLL.LDIR == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_DIRerrorTIM)PoleSign.HollL = 1; 
		}
		else 
		{
			HOLL.LegalityTIM = 0;	
		//	AntiNip.Flag = 0;
		}
		//if(PoleSign.HollL)HOLL.LCNT = HOLL_POSITION_MAX;
		break;
	
		case DOOR_STATUS_Closing:	
		if(HOLL.LSPD == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_SPDerrorTIM)PoleSign.HollL = 1; 	
	//		AntiNip.Flag = 1;
		}
		else if(HOLL.LDIR == 1)
		{
			if(HOLL.LegalityTIM++ > HOLL_DIRerrorTIM)PoleSign.HollL = 1;	
		}
		else	
		{
			HOLL.LegalityTIM = 0;	
	//		AntiNip.Flag = 0;
		}	
		//if(PoleSign.HollL)HOLL.LCNT = 20000;
		break;
	
		default :
		HOLL.LegalityTIM = 0;	
	//	AntiNip.Flag = 0;
		break;
	}	
}
*/

void HollLegalityCheck(void)
{
	switch(DOOR_STATUS)
	{	
		case DOOR_STATUS_Opening:
		if(HOLL.LSPD == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_SPDerrorTIM)AntiNip.Flag = 1;
		}
		else if(HOLL.LDIR == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_DIRerrorTIM)PoleSign.HollL = 1;
		}
		else 
		{
			HOLL.LegalityTIM = 0;	
		}
		break;
	
		case DOOR_STATUS_Closing:	
		if(HOLL.LSPD == 0)
		{
			if(HOLL.LegalityTIM++ > HOLL_SPDerrorTIM)AntiNip.Flag = 1;	
		}
		else if(HOLL.LDIR == 1)
		{
			if(HOLL.LegalityTIM++ > HOLL_DIRerrorTIM)PoleSign.HollL = 1;	
		}
		else	
		{
			HOLL.LegalityTIM = 0;	
		}	
		break;
	
		default :
		HOLL.LegalityTIM = 0;	
		break;
	}	
}
void HollPositionCheck(void)
{
	switch(DOOR_STATUS)
	{	
		case DOOR_STATUS_Opening:	
		if(HOLL.LCNT > HollCommandPosition)PoleSign.HollL = 1;
		
		if(AntiNip.FlagTIM < 60)AntiNip.FlagTIM++;
		else 		
		break;
	
		case DOOR_STATUS_Closing:
		if(AntiNip.FlagTIM < 60)AntiNip.FlagTIM++;
		break;
	
		default :
		AntiNip.FlagTIM = 0;
		break;
	}	
}


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
#include "app.h"
#include "config.h"

volatile   	struct ADCbase 		PoleADC;
volatile  	uint8_t  		ADC_Flag;
volatile  	uint8_t  		ADC_STEP;
volatile  	uint8_t  		ADC_Counter;

//*********************************
/*
void ADC_Logic(void)
{
  if(Tick_ADC >= 300)
  {
   R_ADC_Start();
   Tick_ADC = 0;
  }
}
*/
   /*
    uint8_t 	STEP;
    uint8_t 	AIN0;
    uint8_t 	AIN1;
    uint8_t 	AIN2;   
    uint8_t 	AIN00[5]; 
    uint8_t 	AIN11[5];
    uint8_t 	AIN22[5];
    uint8_t 	AIN00Point;
    uint8_t 	AIN11Point;
    uint8_t 	AIN22Point;
    */
void ADC_Logic(void)
{
  uint8_t i;
  uint16_t temp;
  if(Tick_ADC >= Tick_ADC_ms)//11ms
  {
   temp = 0;
   for(i=0;i<5;i++)temp += PoleADC.AIN00[i];
   PoleADC.AIN0 = (uint8_t)(temp/5);  
   Flash_Data.DB[5] = PoleADC.AIN0;
   temp = 0;
   for(i=0;i<5;i++)temp += PoleADC.AIN11[i];
   PoleADC.AIN1 = (uint8_t)(temp/5); 
   Flash_Data.DB[6] = PoleADC.AIN1;
   temp = 0;
   for(i=0;i<5;i++)temp += PoleADC.AIN22[i];
   PoleADC.AIN2 = (uint8_t)(temp/5);  
   Flash_Data.DB[7] = PoleADC.AIN2;
   	  
   switch(PoleADC.STEP)
   {
   case 0:
   ADC_CH_SW(1);
   PoleADC.STEP = 1;
   break;
   case 1:
   ADC_CH_SW(2);
   PoleADC.STEP = 2;
   break;
   case 2:
   ADC_CH_SW(0);
   PoleADC.STEP = 0;
   break;
   default :
   ADC_CH_SW(0);
   PoleADC.STEP = 0;
   break;	   
   }
   R_ADC_Start();
   Tick_ADC = 0;
   
   Overcurrent_Protect();
  }
}

void Overcurrent_Protect(void)
{
	switch(DOOR_STATUS)
	{	
		case DOOR_STATUS_Opening:
		CurrentProtect_Opening();
		break;
	
		case DOOR_STATUS_Closing:
		CurrentProtect_Closing();
		break;
	
		default :
		if((PoleADC.AIN1 > 0x80)||(PoleADC.AIN2 > 0x80))Brige_ALL_Prohibit(); 
		ADC_Flag 	= 0x11;
		ADC_STEP 	= 0;
		ADC_Counter 	= 0;		
		break;
	}
	
}

void CurrentProtect_Opening(void)
{
   switch(ADC_STEP)
   {
	case 0:
	if(ADC_Flag != DOOR_STATUS_Opening)
	{
		ADC_Flag = DOOR_STATUS_Opening;
		ADC_Counter = 0;
	}
	else
	{
		ADC_Counter++;
		if(ADC_Counter > 80)//880ms
		{
	  		ADC_STEP = 1;
	  		ADC_Counter = 0;
		}		
	}
	break;
	
	case 1:
	/*
	if((PoleADC.AIN1 > 0x25)||(PoleADC.AIN2 > 0x25))
	{
		Brige1_Stop();
		Brige2_Stop();
		DOOR_STATUS = DOOR_STATUS_Close;
		Buzzer_NUM = 3;			
	}
	*/
	if(PoleADC.AIN1 > 0x55)PoleSign.ADC1 = 1;
	if(PoleADC.AIN2 > 0x55)PoleSign.ADC2 = 1;	
	break;	
	
	default :
	ADC_STEP = 0;
	ADC_Counter = 0;
	break;
   }
}

void CurrentProtect_Closing(void)
{
   switch(ADC_STEP)
   {
	case 0:
	if(ADC_Flag != DOOR_STATUS_Closing)
	{
		ADC_Flag = DOOR_STATUS_Closing;
		ADC_Counter = 0;
	}
	else
	{
		ADC_Counter++;
		if(ADC_Counter > 50)//550ms
		{
	  		ADC_STEP = 1;
	  		ADC_Counter = 0;
		}		
	}
	break;
	
	case 1:
	/*
	if((PoleADC.AIN1 > 0x20)||(PoleADC.AIN2 > 0x20))
	{
		Brige1_Stop();
		Brige2_Stop();
		DOOR_STATUS = DOOR_STATUS_Open;
		Buzzer_NUM = 3;			
	}*/
	if(PoleADC.AIN1 > 0x55)PoleSign.ADC1 = 1;
	if(PoleADC.AIN2 > 0x55)PoleSign.ADC2 = 1;
	break;	
	
	default :
	ADC_STEP = 0;
	ADC_Counter = 0;
	break;
   }
}  
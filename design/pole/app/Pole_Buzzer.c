#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "app.h"
#include "config.h"

extern volatile  	uint16_t  	Tick_BUZ;
volatile  		uint16_t  	Buzzer_Step,Buzzer_NUM;
volatile  		uint8_t  	Buzzer_Mode;


void Buzzer_Logic(void)
{
  if(Tick_BUZ >= Tick_BUZ_ms)
  {
   switch(Buzzer_Step)
   {
   case 0:
   //if(Buzzer_NUM){TDR07 = _0FA0_TAU_TDR07_VALUE;Buzzer_NUM--;} 
   if(Buzzer_NUM){TDR07 = 0x0FA0;Buzzer_NUM--;}
   else Buzzer_Mode = 0;//mode=0 or 1
   Buzzer_Step = 1;	     
   break;
   
   case 1:
   if(Buzzer_Mode == 0)TDR07 = 0;
   Buzzer_Step = 0;   
   break;
 
   default :
   TDR07 = 0;
   Buzzer_Step = 0;
   Buzzer_Mode = 0;
   break;	   
   }
   Tick_BUZ = 0;
  }
}

void Buzzer_4KHz(void)
{
	//Buzzer_NUM = 0;//no sound,2018-3-26
  if(Tick_BUZ >= Tick_BUZ_ms)//150ms
  {
   switch(Buzzer_Step)
   {
   case 0:
   if(Buzzer_NUM){TDR07 = 0x0FA0;Buzzer_NUM--;} //4KHz,50%
   else Buzzer_Mode = 0;//mode=0 or 1
   Buzzer_Step = 1;	     
   break;
   
   case 1:
   if(Buzzer_Mode == 0)TDR07 = 0;
   Buzzer_Step = 0;   
   break;
 
   default :
   TDR07 = 0;
   Buzzer_Step = 0;
   Buzzer_Mode = 0;
   break;	   
   }
   Tick_BUZ = 0;
  }
}

  
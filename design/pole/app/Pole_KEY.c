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


volatile   struct    PoleKEY_base 	PoleKEY;

volatile 	uint16_t 	KEY0_cnt;

volatile 	uint8_t 	KEY0Flag;
volatile 	uint8_t 	TIM_Enable;

void Lock_KEY0_Logic(void)
{
uint16_t i;

	i = 0x80;
	//i = 0x08;
	i &= DIN.Value;
	if(i == 0)
      	{  
		if(PoleKEY.STA0 == 0)
	       	{
		     PoleKEY.TIM0++;
		     if(PoleKEY.TIM0 > 3)PoleKEY.STA0 = 1;//9ms
		}		
		/*
		if(PoleKEY.TIM0 < 500)//3s
	       	{
		     PoleKEY.TIM0++;//3s 
		     PoleKEY.STA0 = 1;
		}
		*/
		/*
		else if(PoleKEY.TIM0 == 500)//3s
		{
			PoleKEY.TIM0++;
			Buzzer_Mode = 1;
			Buzzer_NUM  = 4;
		     	PoleKEY.STA0 = 2;		
			HollCommandPosition = HollInf.LCNT;
			HollCommandMode = 2;//make sure the holl position
		}	*/       	
      }
      else
      {
	      	if(PoleKEY.STA0 == 1)PoleSign.KEY0 = 1;  
		/*
		if(PoleKEY.STA0 == 1)
		{
	     		TIM_Enable = 1;  
			PoleSign.KEY0 = 1;
			KEY0_cnt = 0;
		}*/
	      	PoleKEY.STA0 = 0;
	      	PoleKEY.TIM0 = 0;
      }
      
     //if(TIM_Enable)
     {
     	//if(KEY0_cnt++ >= 5000){KEY0_cnt = 0;PoleSign.KEY0 = 1;}//5000*3=15000;
	//if(KEY0_cnt++ >= 6666){KEY0_cnt = 0;PoleSign.KEY0 = 1;}//7000*3=20000;
     }
}

void Lock_KEY1_Logic(void)
{
uint8_t i;

	//i = 0x04;
	i = 0x10;
	i &= DIN.Value;
      	if(i)
     	{
	       if(PoleKEY.STA1 == 0)
	       {
		     PoleKEY.TIM1++;
		     if(PoleKEY.TIM1 > 3)PoleKEY.STA1 = 1;//18ms
	       }	       	
      	}
      	else
      	{
	      if(PoleKEY.STA1 == 1)PoleSign.KEY1 = 1;
	      //if(PoleKEY.STA1 == 1){TIM_Enable = 0;PoleSign.KEY0 = 1;}	
	      //if(PoleKEY.STA1 == 1)TIM_Enable = 0;
	      
	      PoleKEY.STA1 = 0;
	      PoleKEY.TIM1 = 0;
      	}	
}

void Lock_KEY2_Logic(void)
{
uint8_t i;

      i = P1;
      i &= 0x02;//P11
      if(i)
      {
	       if(PoleKEY.STA2 == 0)
	       {
		     PoleKEY.TIM2++;
		     if(PoleKEY.TIM2 > 3)PoleKEY.STA2 = 1;//18ms
	       }	       	
      }
      else
      {
	      if(PoleKEY.STA2 == 1)PoleSign.KEY2 = 1;  
	      PoleKEY.STA2 = 0;
	      PoleKEY.TIM2 = 0;
      }	
}

  
void Lock_KEY0_Logic2(void)
{
	uint8_t i;
	
	i = P1;
      	i &= 0x02;//P11
	if(i == 0)
	{
		KEY0Flag = 1;
	}
	else if(KEY0Flag)
	{
		KEY0Flag = 0;
		PoleSign.KEY0 = 1; 
	}	
}
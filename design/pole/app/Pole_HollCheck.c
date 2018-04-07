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


volatile  uint8_t  	CHK_start;
volatile  uint8_t  	CHK_step;
volatile  uint8_t  	CHK_dir;
volatile  uint16_t  	CHK_tim;
volatile  uint16_t  	CHK_speed;
volatile  uint8_t  	CHK_tim2;
volatile  uint16_t  	CHK_pre;
volatile  uint16_t  	CHK_PWM;
volatile  uint16_t  	CHK_Flag;

//volatile  uint8_t  	CHK_LEDstep;
volatile  uint8_t  	CHK_LEDerror;
//volatile  uint8_t  	CHK_LEDcnt;



void HollCheck_Logic(void)
{
	uint8_t  	i;
	
	
	
	if(Tick_COM >= 10)//10ms
  	{	
		switch(CHK_LEDerror)
		{
			case 0://LED off
			P9 &= 0xF9;
			break;
			
			case 1://pass
			//P9 &= 0xF9;
			P9 |= 0x02;				
			break;
			
			case 2://NG
			//P9 &= 0xF9;
			P9 |= 0x04;				
			break;
			
			default :
			//CHK_LEDstep = 0;
			P9 &= 0xF9;
			break;
		}
		switch(CHK_step)
		{
			case 0x00:
			i = (uint8_t)DIN.Value;
			i &= 0x01;
			if(i == 0x00)
			{
				Buzzer_NUM = 1;
				CHK_step  = 1;
				CHK_LEDerror = 0;
			}
			break;
			
			case 0x01:
			CHK_Flag = 0;
			CHK_tim   = 0;
			CHK_tim2  = 0;
			HOLL.LCNT = 20000;
			CHK_pre   = HOLL.LCNT;
			CHK_step  = 2;
			break;
			
			case 0x02:
			if(CHK_tim++ > 100)
			{
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_PWM = 0;
				CHK_step  = 3;
			}
			break;
			
			case 0x03:
			if(CHK_PWM < 800)
			{
				CHK_PWM += 30; 
				BaseIO_Motor(
				1,//ch
				1,//dir
				CHK_PWM);//val	
			}
			else 
			{
				CHK_pre   = HOLL.LCNT;
				CHK_Flag = 0;
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_step  = 4;
			}
			break;
			
			case 0x04:
			if(CHK_tim++ > 300)
			{
	
				CHK_tim = 0;
				CHK_step  = 5;		
			}	
			if(CHK_tim2++ > 10)
			{
				CHK_tim2 = 0;
				if(CHK_pre  < HOLL.LCNT)CHK_pre   = HOLL.LCNT;
				else 
				{
					CHK_Flag = 1;
					CHK_step  = 5;
				}
			}
			break;
			
			case 0x05:
			if(CHK_PWM > 30)
			{
				CHK_PWM -= 30; 
				BaseIO_Motor(
				1,//ch
				1,//dir
				CHK_PWM);//val	
			}
			else
			{
				BaseIO_Motor(
				0,//ch
				0,//dir
				0);//val	
				CanLoopBack = 1;
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_PWM = 0;
				if(CHK_Flag)CHK_step  = 11;
				else CHK_step  = 6;
			}
			break;
			
			case 0x06:
			if(CHK_tim++ > 100)
			{
				CHK_tim = 0;
				CHK_step  = 7;
			}
			break;
			
			case 0x07:
			if(CHK_PWM < 800)
			{
				CHK_PWM += 30; 
				BaseIO_Motor(
				1,//ch
				0,//dir
				CHK_PWM);//val	
			}
			else 
			{
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_pre   = HOLL.LCNT;
				CHK_step  = 8;
			}
			break;		
			
			case 0x08:
			if(CHK_tim++ > 300)
			{
				CHK_tim = 0;
				CHK_step  = 9;		
			}
			if(CHK_tim2++ > 10)
			{
				CHK_tim2 = 0;
				if(CHK_pre  > HOLL.LCNT)CHK_pre   = HOLL.LCNT;
				else 
				{
					CHK_Flag = 1;
					CHK_step  = 9;
				}
			}			
			break;
			
			case 0x09:
			if(CHK_PWM > 30)
			{
				CHK_PWM -= 30; 
				BaseIO_Motor(
				1,//ch
				0,//dir
				CHK_PWM);//val	
			}
			else
			{
				BaseIO_Motor(
				0,//ch
				0,//dir
				0);//val	
				Buzzer_NUM = 3;
				CanLoopBack = 1;				
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_PWM = 0;
				if(CHK_Flag)CHK_step  = 11;
				else CHK_step  = 10;				
			}
			break;			
			
			case 10:
			if(CHK_tim++ > 100)
			{
				CHK_tim = 0;
				CHK_tim2 = 0;
				CHK_step  = 0;
				//CHK_step  = 1;
				CHK_LEDerror = 1;//PASS
			}			
			break;
			
			case 11:
			if(CHK_tim++ > 20)
			{	
				CHK_tim = 0;
				i = (uint8_t)DIN.Value;
				i &= 0x04;
				if(i == 0x00)
				{
					Buzzer_Mode = 0;
					Buzzer_NUM = 0;
					CHK_step  = 0;
					CHK_LEDerror = 0;
				}
				else
				{
					Buzzer_Mode = 1;
					Buzzer_NUM = 10;
					CHK_LEDerror = 2;
				}
			}			
			break;
			
			default :
			CHK_Flag = 0;
			CHK_tim = 0;
			CHK_tim2 = 0;	
			CHK_PWM = 0;
			CHK_step = 0;
			break;
		}
	
		Tick_COM = 0; 
  	}
}





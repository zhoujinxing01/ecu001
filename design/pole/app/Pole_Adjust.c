#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "app.h"
#include "config.h"

volatile   struct PWM_base 	PWM;
volatile   struct AntiNip_base 	AntiNip;

void PWM_Stop(void)
{
	if(PWM.Present > PWM_STOP_PID)
	{	
		PWM.Present -= PWM_STOP_PID;
		PWM_Adjust_Refresh();				
	}
	else 
	{
		PWM.Flag = 0;
			
		PoleSign_Clear();
  		Brige1_Stop();
  		Brige2_Stop();
  		if(DOOR_STATUS == DOOR_STATUS_Opening)DOOR_STATUS = DOOR_STATUS_Close;
		else if(DOOR_STATUS == DOOR_STATUS_Closing)DOOR_STATUS = DOOR_STATUS_Open;
	}					
}
void PWM_Adjust(void)
{
	uint16_t 	a;
	
	Angle_Catch();
	switch(DOOR_STATUS)
	{
		case DOOR_STATUS_Open:
			PWM.DIR 	= HaltDIR;
			PWM.Present 	= HaltPWM;	
		break;
		
		case DOOR_STATUS_Opening:
			if(Tick_ADJ >= 300)
			{
				Tick_ADJ = 0;
				PWM.DIR 	= PWMOpen[AntiNip.Angle][0];
				a = PWMOpen[AntiNip.Angle][1]*PWMOpenRatio;
				//a = PWMOpen[AntiNip.Angle][1]*JOJO.Ratio;
				PWM.Present = a/10;
			}
			else 
			{
				Tick_ADJ = 0;
				
				AntiNip.DIR   = PWMOpen[AntiNip.Angle][0];
				a = PWMOpen[AntiNip.Angle][1]*PWMOpenRatio;
				//a = PWMOpen[AntiNip.Angle][1]*JOJO.Ratio;
				AntiNip.Value = a/10;				
				
				AntiNip.PID   = PWMPID;				
				//if(AntiNip.Flag)//see 'Pole_Holl.c'
				//{
					//AntiNip.Value  += AntiNipPWM;
					//AntiNip.PID   	= AntiNipPID;					
				//}
				PWM_Adjust_PID();
			}			
		break;
		
		case DOOR_STATUS_Close:
			PWM.DIR 	= HaltDIR;
			PWM.Present 	= HaltPWM;
		break;
		
		case DOOR_STATUS_Closing:
			if(Tick_ADJ >= 300)
			{
				Tick_ADJ = 0;
				PWM.DIR 	= PWMClose[AntiNip.Angle][0];
				a = PWMClose[AntiNip.Angle][1]*PWMCloseRatio;
				//a = PWMClose[AntiNip.Angle][1]*JOJO.Ratio;
				PWM.Present = a/10;
			}
			else 
			{
				Tick_ADJ = 0;
				
				AntiNip.DIR   = PWMClose[AntiNip.Angle][0];
				a = PWMClose[AntiNip.Angle][1]*PWMCloseRatio;
				//a = PWMClose[AntiNip.Angle][1]*JOJO.Ratio;
				AntiNip.Value = a/10;
				
				AntiNip.PID   = PWMPID;				
				//if(AntiNip.Flag)//see 'Pole_Holl.c'
				//{
					//AntiNip.Value  += AntiNipPWM;
					//AntiNip.PID   	= AntiNipPID;					
				//}
				PWM_Adjust_PID();
			}			
		break;
		
		default :
		PWM.Present 	= 0;
		break;
	}
	PWM_Adjust_Refresh();
}



void PWM_Adjust_PID()
{	
	uint16_t 	i;
	if(PWM.DIR == AntiNip.DIR)
	{
		if(PWM.Present < AntiNip.Value)
		{	
			PWM.Present += AntiNip.PID;
		}
		if(PWM.Present > AntiNip.Value)
		{		
			if(PWM.Present >= AntiNip.PID)PWM.Present -= AntiNip.PID;
		}
	}
	else
	{
		if(PWM.Present)
		{
			if(PWM.Present >= AntiNip.PID)PWM.Present -= AntiNip.PID;
			else 
			{
				PWM.Present = 0;
				PWM.DIR = AntiNip.DIR;
			}
		}
		else	PWM.DIR = AntiNip.DIR;
	}	
}

void PWM_Adjust_Refresh(void)
{	
	if(PWM.DIR == 0)
	{
		Brige1_Reverse(PWM.Present);
  		Brige2_Reverse(PWM.Present); 
	}
	else if(PWM.DIR == 1)
	{
		Brige1_Forward(PWM.Present);
  		Brige2_Forward(PWM.Present); 		
	}
	else	Brige_ALL_Prohibit();
}


void Angle_Catch(void)
{
	uint16_t 	temp1,temp2;
	
	if(HOLL.LCNT > 20000)
	{
		temp1 = HOLL.LCNT - 20000;			
		temp2 = HOLL_POSITION_MAX - 20000;
		temp2 /= 16;
		if(temp2)temp1 /= temp2;	
		if(temp1 > 15)temp1 = 15;
	}
	else temp1 = 0;	
	
	AntiNip.Angle = (uint8_t )temp1;	
}


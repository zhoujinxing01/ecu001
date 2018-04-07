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


volatile   struct Lock_base 	LockDown;


void LockDown_Logic(void)
{	
	switch(LockDown.Present)
	{
		case LockDown_Open:
			LockDownLogic_Open();
		break;
		
		case LockDown_Pull:
			LockDownLogic_Pull();	
		break;
		
		case LockDown_Push:
			LockDownLogic_Push();		
		break;		
		
		default :
		LockDown.Present = LockDown_Open;
		LockDown.Time = 0;
		LockDown.TimeOut = 100;//1s
		LockDown.Task = 0;
		LockDown.Pole = 0;//0:disable,1:enable
		LockDown.Error = 0;
		
		Brige_ALL_Prohibit();	  	
		PoleSign_Clear();			
		break;	
	}
}

void LockDownLogic_Open(void)
{
	uint8_t i;
	
	if(LockUp.Task == 0x07)//reset
	{
		if(LockDown.Time < LockDown.TimeOut)LockDown.Time++;
		else	LockDown.Task = 0;
	}	
	else
	{
		i = DIN.Value;	
		i &= LockDownMask;
		if(i == LockDown_Open)
		{
			LockDown.Present = LockDown_Open;
			LockDown.Time = 0;
			LockDown.TimeOut = 300;//3s
			LockDown.Task = 0;
			LockDown.Pole = 1;//0:disable,1:enable
			LockDown.Error = 0;					
		}
		else	if(i == LockDown_Push)
		{
			HOLL.LCNT = 20000;
			LockDown.Present = LockDown_Pull;
			LockDown.Time = 0;
			LockDown.TimeOut = 300;//3s
			LockDown.Task = 1;LockPlan.Book = 1;//pull
			LockDown.Pole = 0;//0:disable,1:enable
			LockDown.Error = 0;	
			
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();			
		}		
		else
		{
			if(LockDown.Time < LockDown.TimeOut)LockDown.Time++;//3s
			else
			{	
				LockDown.Present = LockDown_Open;
				LockDown.Time = 0;
				LockDown.TimeOut = 800;//8s
				if(LockDown.Error == 0)
				{
					LockDown.Task = 7;LockPlan.Book = 7;//pull+push+open
					LockDown.Error = 1;
				}
				LockDown.Pole = 0;//0:disable,1:enable
				//if(LockDown.Error == 0)LockDown.Error = 1;	
				
				Brige_ALL_Prohibit();	  	
				PoleSign_Clear();					
			}
		}
	}	
}


void LockDownLogic_Pull(void)
{
	uint8_t i;
		
	if(LockDown.Task == 0x01)//Pull
	{
		if(LockDown.Time < LockDown.TimeOut)
		{
			LockDown.Time++;
			
			i = DIN.Value;
			i &= LockDownMask;
			if(i == LockDown_Pull)
			{
				HOLL.LCNT = 20000;
				
				LockDown.Present = LockDown_Pull;
				LockDown.Time = 0;
				LockDown.TimeOut = 100;//1s
				LockDown.Task = 0;
				LockDown.Pole = 0;//0:disable,1:enable
				LockDown.Error = 0;
				
				//Brige_ALL_Prohibit();	
				LockPlan.Counter =  LockPlan_Pull_TIM - LockPlan_DLY_TIM;//Speedup,see LockPlan.c
				PoleSign_Clear();	
				Buzzer_NUM = 2;
			}
		}
		else	
		{
			LockDown.Present = LockDown_Open;
			LockDown.Time = 0;
			LockDown.TimeOut = 100;//1s
			LockDown.Task = 0;
			LockDown.Pole = 0;//0:disable,1:enable
			LockDown.Error = 0;	
		}
	}	
	else
	{
		i = DIN.Value;
		i &= LockDownMask;		
		if(i == LockDown_Pull)
		{
			if(PoleSign.KEY0 == 1)
			{
				HOLL.LCNT = 20000;
				LockDown.Present = LockDown_Push;
				LockDown.Time = 0;
				LockDown.TimeOut = 300;//3s
				LockDown.Task = 2;LockPlan.Book = 2;//Push
				LockDown.Pole = 0;//0:disable,1:enable
				LockDown.Error = 0;
				
				Brige_ALL_Prohibit();	  	
				PoleSign_Clear();
				Buzzer_NUM = 2;
			}
		}
		else
		{
			if(LockDown.Time < LockDown.TimeOut)LockDown.Time++;
			else 
			{			
				LockDown.Present = LockDown_Open;
				LockDown.Time = 0;
				LockDown.TimeOut = 100;//1s
				LockDown.Task = 0;
				LockDown.Pole = 0;//0:disable,1:enable
				LockDown.Error = 0;
			}
		}
	}
}

void LockDownLogic_Push(void)
{
	uint8_t i;
		
	if(LockDown.Task == 0x02)//Push
	{
		if(LockDown.Time < LockDown.TimeOut)
		{
			LockDown.Time++;
			
			i = DIN.Value;
			i &= LockDownMask;
			if(i == LockDown_Push)
			{			
				LockDown.Present = LockDown_Push;
				LockDown.Time = 0;
				LockDown.TimeOut = 0;
				LockDown.Task = 0;
				LockDown.Pole = 0;//0:disable,1:enable
				LockDown.Error = 0;	
				
				LockPlan.Counter =  LockPlan_Push_TIM - LockPlan_DLY_TIM;//Speedup,see LockPlan.c
			}
		}
		else	
		{
			LockDown.Present = LockDown_Open;
			LockDown.Time = 0;
			LockDown.TimeOut = 100;//1s
			LockDown.Task = 0;
			LockDown.Pole = 0;//0:disable,1:enable
			LockDown.Error = 0;	
		}
	}
	if(LockDown.Task == 0x04)//Open
	{
		if(LockDown.Time < LockDown.TimeOut)
		{
			LockDown.Time++;
			
			i = DIN.Value;
			i &= LockDownMask;
			if(i == LockDown_Open)
			{
				PoleSign.Lock = 1;
				DOOR_STATUS = DOOR_STATUS_Open;
				
				LockDown.Present = LockDown_Open;
				LockDown.Time = 0;
				LockDown.TimeOut = 100;//1s
				LockDown.Task = 0;
				LockDown.Pole = 1;//0:disable,1:enable
				LockDown.Error = 0;				
			}
		}
		else	
		{
			LockDown.Present = LockDown_Open;
			LockDown.Time = 0;
			LockDown.TimeOut = 100;//1s
			LockDown.Task = 0;
			LockDown.Pole = 0;//0:disable,1:enable
			LockDown.Error = 0;	
		}
	}	
	else
	{
		i = DIN.Value;
		i &= LockDownMask;
		if(i == LockDown_Push)
		{
			LockDown.Present = LockDown_Push;
			LockDown.Time = 0;
			LockDown.TimeOut = 200;//2s
			LockDown.Task = 4;LockPlan.Book = 4;//open
			LockDown.Pole = 0;//0:disable,1:enable
			LockDown.Error = 0;
				
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();
		}
		else
		{
			if(LockDown.Time < LockDown.TimeOut)LockDown.Time++;
			else 
			{			
				LockDown.Present = LockDown_Open;
				LockDown.Time = 0;
				LockDown.TimeOut = 100;//1s
				LockDown.Task = 0;
				LockDown.Pole = 0;//0:disable,1:enable
				LockDown.Error = 0;
			}
		}
	}
}

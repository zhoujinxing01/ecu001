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



volatile   struct Lock_base 	LockUp;

void LockUp_Logic(void)
{	
	switch(LockUp.Present)
	{
		case LockUp_Open:
			LockUpLogic_Open();
		break;
		
		case LockUp_Half:
			LockUpLogic_Half();
		break;
		
		case LockUp_All_Pull:
			LockUpLogic_All_Pull();	
		break;
		
		case LockUp_All_Push:
			LockUpLogic_All_Push();		
		break;		
		
		default :
		LockUp.Present = LockUp_Open;
		LockUp.Time = 0;
		LockUp.TimeOut = 100;//1s
		LockUp.Task = 0;
		LockUp.Pole = 0;//0:disable,1:enable
		LockUp.Error = 0;
		
		Brige_ALL_Prohibit();	  	
		//PoleSign_Clear();			
		break;	
	}
}

void LockUpLogic_Open(void)
{
	uint8_t i;
	
	if(LockUp.Task == 0x07)//reset
	{
		if(LockUp.Time < LockUp.TimeOut)LockUp.Time++;//8s
		else	LockUp.Task = 0;
	}	
	else
	{
		i = DIN.Value;	
		i &= LockUpMask;
		if((i == LockUp_Open)&&(LockUp.Error == 0))
		{
			LockUp.Present = LockUp_Open;
			LockUp.Time = 0;
			LockUp.TimeOut = 300;//3s
			LockUp.Task = 0;
			LockUp.Pole = 1;//0:disable,1:enable
			//LockUp.Error = 0;					
		}
		else	if((i == LockUp_Half)&&(LockUp.Error == 0))
		{
			HOLL.LCNT = 20000;
			LockUp.Present = LockUp_Half;
			LockUp.Time = 0;
			LockUp.TimeOut = 300;//3s
			LockUp.Task = 1;LockPlan.Book = 1;//pull
			LockUp.Pole = 0;//0:disable,1:enable
			//LockUp.Error = 0;	
			
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();			
		}
		else	if((i == LockUp_All_Push)&&(LockUp.Error == 0))
		{
			HOLL.LCNT = 20000;
			LockUp.Present = LockUp_All_Push;
			LockUp.Time = 0;
			LockUp.TimeOut = 300;//3s
			LockUp.Task = 0;
			LockUp.Pole = 0;//0:disable,1:enable
			//LockUp.Error = 0;	
			
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();			
		}		
		else LockUp_Error();
	}	
}

void LockUpLogic_Half(void)
{
	uint8_t i;
	
	if(LockUp.Time < LockUp.TimeOut)
	{
		LockUp.Time++;
		
		i = DIN.Value;
		i &= LockUpMask;
		if(i == LockUp_All_Pull)
		{
			//LockPlan.Counter =  LockPlan_Pull_TIM - LockPlan_DLY_TIM;//Speedup,see LockPlan.c
			LockPlan.Counter =  LockPlan_Pull_TIM - 50;//Speedup,see LockPlan.c
			LockUp.Present = LockUp_All_Pull;
			LockUp.Time = 0;
			LockUp.TimeOut = 300;//3s
			LockUp.Task = 2;LockPlan.Book = 2;//push
			LockUp.Pole = 0;//0:disable,1:enable
			LockUp.Error = 0;					
		}		
	}
	else	
	{
		LockUp.Present = LockUp_Open;
		LockUp.Time = 0;
		LockUp.TimeOut = 100;//1s
		LockUp.Task = 0;
		LockUp.Pole = 0;//0:disable,1:enable
		LockUp.Error = 1;			
	}
}

void LockUpLogic_All_Pull(void)
{
	uint8_t i;
	
	if(LockUp.Time < LockUp.TimeOut)
	{
		LockUp.Time++;
		
		i = DIN.Value;
		i &= LockUpMask;
		if(i == LockUp_All_Push)
		{
			PoleSign_Clear();
			LockPlan.Counter =  LockPlan_Pull_TIM - LockPlan_DLY_TIM;//Speedup,see LockPlan.c
			LockUp.Present = LockUp_All_Push;
			LockUp.Time = 0;
			//LockUp.TimeOut = 0;
			LockUp.TimeOut = 100;//1s
			LockUp.Task = 0;
			LockUp.Pole = 0;//0:disable,1:enable
			LockUp.Error = 0;					
		}		
	}
	else	
	{
		PoleSign_Clear();
		LockUp.Present = LockUp_Open;
		LockUp.Time = 0;
		LockUp.TimeOut = 100;//1s
		LockUp.Task = 0;
		LockUp.Pole = 0;//0:disable,1:enable
		LockUp.Error = 1;			
	}	
}

void LockUpLogic_All_Push(void)
{
	uint8_t i;
		
	if(LockUp.Task == 0x04)//open
	{
		if(LockUp.Time < LockUp.TimeOut)
		{
			LockUp.Time++;
			
			i = DIN.Value;
			i &= LockUpMask;
			if(i == LockUp_Open)
			{
				PoleSign.Lock = 1;
				DOOR_STATUS = DOOR_STATUS_Open;	
				//Brige_ALL_Prohibit();		
				//LockPlan.Counter =  LockPlan_Pull_TIM - LockPlan_DLY_TIM;//Speedup,see LockPlan.c
				
				LockUp.Present = LockUp_Open;
				LockUp.Time = 0;
				LockUp.TimeOut = 0;
				LockUp.Task = 0;
				LockUp.Pole = 0;//0:disable,1:enable
				LockUp.Error = 0;				
			}
		}
		else	
		{
			Brige_ALL_Prohibit();
			LockUp.Present = LockUp_Open;
			LockUp.Time = 0;
			LockUp.TimeOut = 100;//1s
			LockUp.Task = 0;
			LockUp.Pole = 0;//0:disable,1:enable
			LockUp.Error = 1;	
		}
	}	
	else
	{
		i = DIN.Value;
		i &= LockUpMask;
		if(i == LockUp_All_Push)
		{
			if(PoleSign.KEY0 == 1)
			{
				HOLL.LCNT = 20000;
				LockUp.Present = LockUp_All_Push;
				LockUp.Time = 0;
				LockUp.TimeOut = 100;//1s
				LockUp.Task = 4;LockPlan.Book = 4;//open
				LockUp.Pole = 0;//0:disable,1:enable
				LockUp.Error = 0;
				
				Brige_ALL_Prohibit();	  	
				PoleSign_Clear();				
			}
			LockUp.Time = 0;
			LockUp.TimeOut = 100;//1s			
		}
		else
		{
			if(LockUp.Time < LockUp.TimeOut)LockUp.Time++;
			else 
			{			
				LockUp.Present = LockUp_Open;
				LockUp.Time = 0;
				LockUp.TimeOut = 100;//1s
				LockUp.Task = 0;
				LockUp.Pole = 0;//0:disable,1:enable
				LockUp.Error = 1;
			}
		}
	}
}
	
void LockUp_Error(void)
{	
	if(LockUp.Time < LockUp.TimeOut)LockUp.Time++;//1s
	else
	{
		if(PoleSign.KEY0 == 1)
		{
			LockUp.Present = LockUp_Open;
			LockUp.Time = 0;
			LockUp.TimeOut = 800;//8s
			if(LockUp.Error == 1)
			{
				LockUp.Task = 7;LockPlan.Book = 7;//pull+push+open
				LockUp.Error = 0;
			}
			LockUp.Pole = 0;//0:disable,1:enable	
				
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();
		}
		else
		{
			LockUp.Present = LockUp_Open;
			LockUp.Time = 0;
			LockUp.TimeOut = 100;//1s
			LockUp.Pole = 0;//0:disable,1:enable	
			LockUp.Error = 1;
				
			Brige_ALL_Prohibit();	  	
			PoleSign_Clear();			
		}
	}	
}



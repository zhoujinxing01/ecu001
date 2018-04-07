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

volatile	struct	LockPlanBase	LockPlan;

/*
typedef struct LockPlanBase
{
    uint8_t 	StepA; 
    uint8_t 	StepB;
    uint8_t 	Counter; 
    uint8_t 	Flag; 
    uint8_t 	Book;//001:pull,010:push,100:open
};
*/

//before use,give value to LockPlan.Book
//LockPlan.Book = 0x00,do nothing
//LockPlan.Book = 0x01,do pull
//LockPlan.Book = 0x02,do push
//LockPlan.Book = 0x04,do open
//LockPlan.Book = 0x07,do pull,push and open,reset the lock
//LockPlan.Book = 0x03,do pull and push
//2017-12-14,zhou jin xing.
void LockPlan_Call(void)
{
	uint8_t i;		
	switch(LockPlan.StepA)
   	{
		case 0:
		i = 0x01&LockPlan.Book;
		if(i)
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 1;			
			LockPlan.StepA = 0x01;
			LockPlan.Book &= 0xFE;//1110
		}
		else 
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 0;					
			LockPlan.StepA = 0x02;
		}
		break;
		
		case 1://pull
		if(LockPlan.Flag)LockPlan_Pull();
		else
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;						
			LockPlan.StepA = 0x02;			
		}
		break;
		
	
		case 2:	
		i = 0x02&LockPlan.Book;
		if(i)
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 1;			
			LockPlan.StepA = 0x03;
			LockPlan.Book &= 0xFD;//1101
		}
		else 
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 0;					
			LockPlan.StepA = 0x04;
		}		
		break;
	
		case 3://push
		if(LockPlan.Flag)LockPlan_Push();
		else
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;					
			LockPlan.StepA = 0x04;			
		}		
		break;
		
		case 4:
		i = 0x04&LockPlan.Book;
		if(i)
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 1;			
			LockPlan.StepA = 0x05;
			LockPlan.Book &= 0xFB;//1011
		}
		else 
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;	
			LockPlan.Flag = 0;					
			LockPlan.StepA = 0x06;
		}		
		break;
		
		case 5://open
		if(LockPlan.Flag)LockPlan_Open();
		else
		{
			LockPlan.Counter = 0;
			LockPlan.StepB = 0;					
			LockPlan.StepA = 0x06;			
		}		
		break;		
		
		default :
		i = 0x07&LockPlan.Book;
		if(i)LockPlan.StepA = 0x00;
		break;
	}	
}

void LockPlan_Open(void)
{
   	switch(LockPlan.StepB)
   	{
	case 0:
	Lock_Open();
	SpringUp_Start();
	LockPlan.StepB = 1;
	break;
	
	case 1:
	LockPlan.Counter++;
	if(LockPlan.Counter > LockPlan_Open_TIM)
	{
		LockPlan.Flag = 0;
		Lock_OpenStop();
		SpringUp_Stop();		
	}	
	break;
	
	default :
	break;
   	}	 
}

void LockPlan_Pull(void)
{
   	switch(LockPlan.StepB)
   	{
	case 0:
	Lock_Pull();
	LockPlan.StepB = 1;
	break;
	
	case 1:
	LockPlan.Counter++;
	if(LockPlan.Counter > LockPlan_Pull_TIM)
	{
		LockPlan.Flag = 0;
		Lock_PullStop();
	}	
	break;
	
	default :
	break;
   	}	 
}

void LockPlan_Push(void)
{
   	switch(LockPlan.StepB)
   	{
	case 0:
	Lock_Push();
	LockPlan.StepB = 1;
	break;
	
	case 1:
	LockPlan.Counter++;
	if(LockPlan.Counter > LockPlan_Push_TIM)
	{
		LockPlan.Flag = 0;
		Lock_PushStop();
	}	
	break;
	
	default :
	break;
   	}	 
}


void SpringUp_Start(void)
{
	if(SpringUp_EN)
	{
		Brige1_Forward(SpringUp_VAL);
  		Brige2_Forward(SpringUp_VAL); 		
	}
}

void SpringUp_Stop(void)
{
	if(SpringUp_EN)
	{
		if((LockUp.Present != LockUp_Open)&&(LOCK_Type == 1))
		{
			Brige1_Stop();
  			Brige2_Stop(); 				
		}
		else if((LockDown.Present != LockDown_Open)&&(LOCK_Type == 0))
		{
			Brige1_Stop();
  			Brige2_Stop(); 				
		}		
	}
}


void Lock_Pull(void)
{
	Brige3_Reverse(1599);//pull
}
void Lock_PullStop(void)
{
	Brige3_Stop();
}
void Lock_Push(void)
{
	Brige3_Forward(1599);//push
}
void Lock_PushStop(void)
{
	Brige3_Stop();
}



  
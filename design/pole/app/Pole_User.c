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



void Pole_Procedure_Up(void)
{
  if(Tick_COM >= 10)//10ms
  {	
	LockPlan_Call();
	LockUp_Logic();
	if(LockUp.Pole == 1)
	{
		if(AntiNip.Flag == 1)DoorNip_Procedure();
		else Door_Procedure();
	}
	if(LockUp.Pole == 2)DoorNip_Procedure();
	
	Tick_COM = 0; 
  }
}

void Pole_Procedure_Down(void)
{
  if(Tick_COM >= 10)//10ms
  {	
	LockPlan_Call();
	LockDown_Logic();
	if(LockDown.Pole == 1)Door_Procedure();
	
	Tick_COM = 0; 
  }
}





  
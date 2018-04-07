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

volatile    	   uint8_t  	Power_Switch;
extern volatile    uint8_t  	Tick_PWR;


void PWR_Switch(void)
{
  if(Tick_PWR >= Tick_PWR_ms)
  {
    switch(Power_Switch)
    {
      case SW_ON:
      PWR_Switch_ON();
      break;
      case SW_OFF:
      PWR_Switch_OFF();
      break;
      default :
      Power_Switch = SW_OFF;
      PWR_Switch_OFF();
      break;	    	    
    }
    Tick_PWR = 0;
  }
}




  
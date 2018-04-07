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


volatile	struct	DIN_base	DIN;
volatile  	uint8_t  	DIN_Point;
volatile  	uint8_t  	dddiiinnn;


void DigitalInput(void)
{		
	uint8_t i;
  	if(Tick_DIN >= Tick_DIN_ms)//3ms
  	{
			
		//DigitalInput_8ch();
		P8 |= 0xFE;
		P9 |= 0x01;
		i = P8;
		DIN.Value = i >> 1;
		i = P9;
		i &= 0x01;
		if(i)DIN.Value |= 0x80;		
		
		if(dddiiinnn)
		{
			DIN.Value &= 0xF8;
			i = dddiiinnn&0x07;
			DIN.Value |= i;
		}
		
		Lock_KEY0_Logic();
		Lock_KEY1_Logic();
		//Lock_KEY0_Logic2();
		
	
		Flash_Data.DB[2] = DIN.Value;
     
    		Tick_DIN = 0;
  }
}


void DigitalInput_8ch(void)
{
	uint8_t i,j;
	
	j = 1;
	j = j<<DIN_Point;
      	i = P6;
      	i &= 0x08;//P63
	if(i)DIN.Value |= j;
	else
	{
		j = ~j;
		DIN.Value &= j;
	}
	
	DIN_Point++;
	DIN_Point %= 8;		
	if(DIN_Point&0x01)BIT_SET(P8,1);
	else BIT_CLR(P8,1);
	if(DIN_Point&0x02)BIT_SET(P8,2);
	else BIT_CLR(P8,2);
	if(DIN_Point&0x04)BIT_SET(P8,3);
	else BIT_CLR(P8,3);		
}

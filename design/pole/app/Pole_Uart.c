#include "app.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "config.h"


volatile  uint8_t   	DataBuff[30];


void Pole_Uart(void)
{
	uint8_t temp[8];
	
  	if(Tick_UART >= 300)//300ms
  	{
    		temp[0] = (uint8_t)(HOLL.LCNT/256);
		temp[1] = (uint8_t)(HOLL.LCNT%256);
    		temp[2] = (uint8_t)(HOLL.LSPD/256);
		temp[3] = (uint8_t)(HOLL.LSPD%256);	
		//Uart_SendByASCII(temp,8);
		//R_UART0_Send(temp,2);
    		Uart_Logic();
	
    
    		Tick_UART = 0;
  	}
}

void Uart_SendByASCII(uint8_t *tx_buf, uint8_t tx_num)
{
	uint8_t i,j;
	
	for(i=0;i<tx_num;i++)
	{
		j = tx_buf[i]/100;
		DataBuff[i*4]   = 48 + j;
		
		j = tx_buf[i]%100;
		j /= 10;
		DataBuff[i*4+1] = 48 + j;	
		
		j = tx_buf[i]%100;
		j %= 10;
		DataBuff[i*4+2] = 48 + j;	
		
		DataBuff[i*4+3] = 48 + j;		
	}
	DataBuff[tx_num*4]   = 0x0d;	
	DataBuff[tx_num*4+1] = 0x0a;
	i = tx_num*4+2;
	
	R_UART0_Send(DataBuff,i); 
}

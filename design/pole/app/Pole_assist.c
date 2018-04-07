/******************************************************************************* 
* File Name    : assist.c
* Version      : 1.0 
* Description  : This is source file for Pole drive.
* Designer     : zhou,jinxing
******************************************************************************/ 

#include "app.h"
#include "config.h"

volatile  uint8_t   	Temp_Buff[10];
volatile   struct 	PoleSign_base 	PoleSign;
//volatile   struct DoorControl_base 	DoorControl;

void Digital_Input(void)
{
	Temp_Buff[0] = P8;
	R_UART0_Send(Temp_Buff,1);
}

void delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for(i=0;i<ms;i++)delay_100us(10);//1ms
}
void delay_100us(uint16_t us)
{
	uint16_t i,j;
	for(i=0;i<us;i++)for(j=0;j<353;j++);//100us,100us/353=0.283us
}

void PoleSign_Clear(void)
{
	PoleSign.CANid0 = 0;
	PoleSign.CANid1 = 0;
	PoleSign.CANid2 = 0;
	PoleSign.CANid3 = 0;
	PoleSign.KEY0 = 0;
	PoleSign.KEY1 = 0;
	PoleSign.KEY2 = 0;
	PoleSign.ADC0 = 0;
	PoleSign.ADC1 = 0;
	PoleSign.ADC2 = 0;
	PoleSign.HollL = 0;
	PoleSign.HollR = 0;	
	PoleSign.PWM = 0;	
	PoleSign.Lock = 0;
}

/*
void DoorControl_Init(void)
{
	DoorControl.Flag = 0;
	DoorControl.PWM = 0;	
}

*/
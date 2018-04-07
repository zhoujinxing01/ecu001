/******************************************************************************* 
* File Name    : assist.c
* Version      : 1.0 
* Description  : This is source file for Pole drive.
* Designer     : zhou,jinxing
******************************************************************************/ 

#include "app.h"
#include "config.h"


void BaseIO_Motor(uint8_t ch,uint8_t dir,uint16_t val)
{
	switch(ch)
	{
		case 1:
		if(dir)Brige1_Forward(val);
		else Brige1_Reverse(val);
		break;
		case 2:
		if(dir)Brige2_Forward(val);
		else Brige2_Reverse(val);		
		break;
		case 3:
		if(dir)Brige3_Forward(val);
		else Brige3_Reverse(val);
		break;
		case 4:
		if(dir)Lock_Open();
		else Lock_OpenStop();
		break;
		default :
		Brige_ALL_Prohibit();
		Lock_OpenStop();
		break;	
	}

}

//*********************************
void CH1_Prohibit(void)
{
  //BIT_CLR(P8,6);
  BIT_CLR(P13,0);
  TDR03 = 0;
}
void CH2_Prohibit(void)
{
  //BIT_CLR(P8,7);
  //P8 &= 0x7F;
  BIT_CLR(P14,0);
  TDR04 = 0;
}
void CH3_Prohibit(void)
{
  //BIT_CLR(P9,0);
  BIT_CLR(P13,0);
  TDR02 = 0;
}
void CH4_Prohibit(void)
{
  //BIT_CLR(P9,1);
  BIT_CLR(P14,0);
  TDR05 = 0;
}
void CH5_Prohibit(void)
{
  //BIT_CLR(P9,2);
  BIT_CLR(P6,3);
  TDR01 = 0;
}
//*********************************
void CH1_PMOS_OUT(void)
{
  //BIT_SET(P8,6);
  BIT_SET(P13,0);
  TDR03 = 0;
}
void CH2_PMOS_OUT(void)
{
  //BIT_SET(P8,7);
  //P8 |= 0x80;
  BIT_SET(P14,0);
  TDR04 = 0;
}
void CH3_PMOS_OUT(void)
{
  //BIT_SET(P9,0);
  BIT_SET(P13,0);
  TDR02 = 0;
}
void CH4_PMOS_OUT(void)
{
  //BIT_SET(P9,1);
  BIT_SET(P14,0);
  TDR05 = 0;
}
void CH5_PMOS_OUT(void)
{
  //BIT_SET(P9,2);
  BIT_SET(P6,3);
  TDR01 = 0;
}
//*********************************
void CH1_NMOS_OUT(uint16_t val)
{
  //BIT_CLR(P8,6);
  BIT_CLR(P13,0);
  TDR03 = val;
}
void CH2_NMOS_OUT(uint16_t val)
{
  //BIT_CLR(P8,7);
  //P8 &= 0x7F;
  BIT_CLR(P14,0);
  TDR04 = val;
}
void CH3_NMOS_OUT(uint16_t val)
{
  //BIT_CLR(P9,0);
  BIT_CLR(P13,0);
  TDR02 = val;
}
void CH4_NMOS_OUT(uint16_t val)
{
  //BIT_CLR(P9,1);
  BIT_CLR(P14,0);
  TDR05 = val;
}
void CH5_NMOS_OUT(uint16_t val)
{
  //BIT_CLR(P9,2);
  BIT_CLR(P6,3);
  TDR01 = val;
}
//*********************************
void CH6_Speaker_PWM(uint16_t val)
{
  TDR07 = val;
}
//**************************************************
void Brige_ALL_Prohibit(void)
{
  CH1_Prohibit();
  CH2_Prohibit();
  CH3_Prohibit();
  CH4_Prohibit();
  CH5_Prohibit();
}
void Brige1_Forward(uint16_t val)
{
  CH1_PMOS_OUT();
  CH2_NMOS_OUT(val);
}
void Brige1_Reverse(uint16_t val)
{
  CH2_PMOS_OUT();
  CH1_NMOS_OUT(val);
}
void Brige1_Stop(void)
{
  CH1_Prohibit();
  CH2_Prohibit();		
}

void Brige2_Forward(uint16_t val)
{
  CH3_PMOS_OUT();
  CH4_NMOS_OUT(val);
}
void Brige2_Reverse(uint16_t val)
{
  CH4_PMOS_OUT();
  CH3_NMOS_OUT(val);
}
void Brige2_Stop(void)
{
  CH3_Prohibit();
  CH4_Prohibit();		
}

void Brige3_Forward(uint16_t val)
{
  CH2_PMOS_OUT();
  CH5_NMOS_OUT(val);
}
void Brige3_Reverse(uint16_t val)
{
  CH5_PMOS_OUT();
  CH2_NMOS_OUT(val);
}
void Brige3_Stop(void)
{
  CH2_Prohibit();
  CH5_Prohibit();		
}

void Brige4_Forward(uint16_t val)
{
  CH4_PMOS_OUT();
  CH5_NMOS_OUT(val);
}
void Brige4_Reverse(uint16_t val)
{
  CH5_PMOS_OUT();
  CH4_NMOS_OUT(val);
}
void Brige4_Stop(void)
{
  CH4_Prohibit();
  CH5_Prohibit();		
}

void Holl_Power_ON(void)
{
  //BIT_SET(P1,2);//P12	
  BIT_SET(P4,1);//P41
}
void Holl_Power_OFF(void)
{
 // BIT_CLR(P1,2);//P12
  BIT_CLR(P4,1);//P41
}

void Lock_Open(void)
{
	//BIT_SET(P13,0);
	BIT_SET(P3,1);
}
void Lock_OpenStop(void)
{
	//BIT_CLR(P13,0);
	BIT_CLR(P3,1);
}

void PWR_Switch_ON(void)
{
  //BIT_SET(P8,5);	
  BIT_SET(P1,2);
}
void PWR_Switch_OFF(void)
{
  //BIT_CLR(P8,5);
  BIT_CLR(P1,2);
}
void LED_ON(void)
{
	//BIT_SET(P14,0);
	BIT_SET(P6,0);
}

void LED_OFF(void)
{
	//BIT_CLR(P14,0);
	BIT_CLR(P6,0);
}

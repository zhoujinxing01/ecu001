/******************************************************************************* 
* File Name    : assist.c
* Version      : 1.0 
* Description  : This is source file for Pole drive.
* Designer     : zhou,jinxing
******************************************************************************/ 

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
#include "r_rl78_can_drv.h"


extern volatile  uint8_t  	Flash_flag;
extern volatile  uint16_t  	Tick_CAN;

volatile  uint8_t  	CAN_TXD_CNT;
volatile  uint8_t  	CAN_RXD_CNT;
volatile  uint8_t  	CRXD_ID0_STEP;
volatile  uint8_t  	CRXD_ID1_STEP;
volatile  uint8_t  	CanLoopBack;

/*
const can_frame_t Data_send[]=
{
	0x0022,
	0x0,
	0,
	0,
	0,
	0x0000,
	0x000,
	8,
	{0x88,1,2,0x66,4,5,6,0x99}
};
*/


can_frame_t Data_rcv=
{
	0x0002,
	0x0,
	0,
	0,
	0,
	0x0000,
	0x000,
	8,
	{0,1,2,3,4,5,6,7}
};
uint16_t temp16;



void CAN_Logic(void)
{
  if(Tick_CAN >= Tick_CAN_ms)//5ms
  {
	/*  
	 if(CanLoopBack)
	 {
		CanLoopBack = 0;
       		Flash_Data.DB[0] = (uint8_t)(HOLL.LCNT/256);
       		Flash_Data.DB[1] = (uint8_t)(HOLL.LCNT%256);		
       		if(R_CAN_TrmByTxBuf_CH0(0,(const can_frame_t *)&Flash_Data)==CAN_RTN_OK)
       		{
        		if(R_CAN_CheckTxBufResult_CH0(0)!=CAN_RTN_TX_END)NOP();
      		} 		
	 }*/
  	Pole_CAN_TXD();	
	
	
	/*
	if(CAN_RXD_CNT <  200)CAN_RXD_CNT++;//1s for Power on.
	if(CAN_RXD_CNT >= 200)Pole_CAN_RXD_2();//5ms cycle
	*/
	
  	Tick_CAN = 0;
  }
  //if(CAN_RXD_CNT < 200)Pole_CAN_RXD_1();
  Pole_CAN_RXD_1();
}

void Pole_CAN_TXD(void)
{
	CAN_TXD_CNT++;
	//if(CAN_TXD_CNT > 100)//500ms
	if(CAN_TXD_CNT > 60)//300ms
	{
		CAN_TXD_CNT = 0;
		
       		Flash_Data.DB[0] = (uint8_t)(HOLL.LCNT/256);
       		Flash_Data.DB[1] = (uint8_t)(HOLL.LCNT%256);
		Flash_Data.DB[3] = (uint8_t)HOLL.LDIR;
		Flash_Data.DB[4] = (uint8_t)HOLL.LSPD;
		//Flash_Data.DB[4] = DOOR_STATUS;	
       		//Flash_Data.DB[6] = (uint8_t)(HOLL.LDIR);
       		//Flash_Data.DB[7] = (uint8_t)(HOLL.LSPD);		
       		if(R_CAN_TrmByTxBuf_CH0(0,(const can_frame_t *)&Flash_Data)==CAN_RTN_OK)
       		{
        		if(R_CAN_CheckTxBufResult_CH0(0)!=CAN_RTN_TX_END)NOP();
      		} 	
	}
}

void Pole_CAN_RXD_1(void)
{
  if(R_CAN_ReadRxBuffer(0,(can_frame_t *)&Data_rcv)==CAN_RTN_OK)
  {
	 CRXD_ID0();
	 CRXD_ID1();
	 CRXD_ID2();
	}
}	


void Pole_CAN_RXD_2(void)
{
  if(R_CAN_ReadRxBuffer(0,(can_frame_t *)&Data_rcv)==CAN_RTN_OK)
  { 
	 CRXD_ID0();
	 CRXD_ID1();
	 CRXD_ID2();
		
  }
}

void CRXD_ID0(void)//id = 392,key
{
	if((Data_rcv.IDL == 0x0392)&&(Data_rcv.IDH == 0x0000))
	{
		System_ECO_TIM = 0;//for ECO time out
		switch(CRXD_ID0_STEP)
		{
			case 0:
				if(Data_rcv.DB[3] == 0x00)CRXD_ID0_STEP = 1;
				else if(Data_rcv.DB[3] == 0x08)CRXD_ID0_STEP = 2;
			break;
			
			case 1:
				if(Data_rcv.DB[3] == 0x08)CRXD_ID0_STEP = 2;			
			break;
			
			case 2:
				if(Data_rcv.DB[3] == 0x00)
				{
					CRXD_ID0_STEP = 1;
					if(PoleSign.CANid2 == 0)PoleSign.CANid0 = 1;//judgement by id 393
					//PoleSign.CANid0 = 1;
				}		
			break;	
			
			default :
				CRXD_ID0_STEP = 0;
			break;
		}		
	}
}
void CRXD_ID1(void)//id = 58b,remote
{
	if((Data_rcv.IDL == 0x058b)&&(Data_rcv.IDH == 0x0000))
	{
		switch(CRXD_ID1_STEP)
		{
			case 0:
				if((Data_rcv.DB[0] == 0x80)&&(Data_rcv.DB[4] == 0x08))
				{
					CRXD_ID1_STEP = 1;
				}
				else if((Data_rcv.DB[0] == 0x84)&&(Data_rcv.DB[4] == 0x08))
				{
					CRXD_ID1_STEP = 2;				
				}
			break;
			
			case 1:
				if((Data_rcv.DB[0] == 0x84)&&(Data_rcv.DB[4] == 0x08))
				{
					CRXD_ID1_STEP = 2;
				}		
			break;
			
			case 2:
				if((Data_rcv.DB[0] == 0x80)&&(Data_rcv.DB[4] == 0x08))
				{
					CRXD_ID1_STEP = 1;
					PoleSign.CANid1 = 1;
				}		
			break;	
			
			default :
				CRXD_ID1_STEP = 0;
			break;
		}	
	}
}

void CRXD_ID2(void)//id = 393,Lock and Unlock
{
	if((Data_rcv.IDL == 0x0393)&&(Data_rcv.IDH == 0x0000))
	{
		//if(Data_rcv.DB[5] == 0x00)PoleSign.CANid2 = 1;//Lock
		//if(Data_rcv.DB[5] == 0x04)PoleSign.CANid2 = 0;//Unlock
//**************************************************************************		
		if(Data_rcv.DB[0] == 0x77)LockPlan.Book = Data_rcv.DB[1];
		CAN_BaseIO();
//**************************************************************************			
	}
}

/*
void CRXD_ID2(void)//id = 393,Lock and Unlock
{
	if((Data_rcv.IDL == 0x0003)&&(Data_rcv.IDH == 0x0000))
	{
		JOJO.Data[0] = Data_rcv.DB[0];
		JOJO.Data[1] = Data_rcv.DB[1];
		JOJO.Data[2] = Data_rcv.DB[2];
		JOJO.Data[3] = Data_rcv.DB[3];
		JOJO.Data[4] = Data_rcv.DB[4];
		JOJO.Data[5] = Data_rcv.DB[5];
		JOJO.Data[6] = Data_rcv.DB[6];
		JOJO.Data[7] = Data_rcv.DB[7];
		JOJO_CAN();
		Buzzer_NUM = 3;
	}
}
*/
void CAN_BaseIO(void)
{
  uint16_t  i;
  
	i =    Data_rcv.DB[1];   
       	switch(Data_rcv.DB[0])
       	{
		case 0x03://P125,ch1
	  		TDR03 = i;
	  	break;
		case 0x04://P13,ch2
	  		TDR04 = i;
	  	break;
		case 0x05://P16
	  		TDR02 = i;
	  	break;
		case 0x06://P15
	  		TDR05 = i;
	  	break;
		case 0x07://P30
	  		TDR01 = i;
	  	break;
		
		case 0x08:
		i = Data_rcv.DB[3]*256;
		i += Data_rcv.DB[4];
		if(i > 1600)i = 1600;
		BaseIO_Motor(
		Data_rcv.DB[1],//ch
		Data_rcv.DB[2],//dir
		i);//val
		break;
		
		case 0x09:
		if(Data_rcv.DB[1])LED_ON();
		else LED_OFF();
		break;
		
		case 0x0A:
      		Buzzer_Mode = 0;
      		Buzzer_NUM = Data_rcv.DB[1];	
		CanLoopBack = 1;
		break;
		
		case 0x0B:
		PoleSign.KEY0 = 1;
		break;
		
		case 0x0C:
		dddiiinnn = Data_rcv.DB[1];
		break;
	 
		default :
	  	break;
       }    
}

  
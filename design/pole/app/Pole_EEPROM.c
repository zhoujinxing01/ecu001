#include "app.h"
#include "config.h"
#include "iodefine.h"
#include "r_rl78_can_drv.h"
#include "Pole_EEPROM.h"   


volatile  uint8_t  	Flash_flag;
volatile  uint8_t  	EEPROM_WR_buf[8];
volatile  uint8_t  	EEPROM_RD_buf[8];
extern can_frame_t 	Data_rcv;

volatile  uint16_t  	HollCommandPosition;
volatile  uint8_t  	HollCommandMode;

can_frame_t Flash_Data=
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



void FLASH_Copy(void)
{
 uint8_t i;

 switch(Flash_flag)
 {
   case 0x01://WR
   for(i=0;i<6;i++)EEPROM_WR_buf[i] = Data_rcv.DB[i+2];   
   EEPROM_Manipulate(EEPROM_WR,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf);  
   for(i=0;i<6;i++)Flash_Data.DB[i+2] = EEPROM_RD_buf[i]; 
   break;
   
   case 0x02://RD
   for(i=0;i<6;i++)EEPROM_WR_buf[i] = Data_rcv.DB[i+2];   
   EEPROM_Manipulate(EEPROM_RD,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf);  
   for(i=0;i<6;i++)Flash_Data.DB[i+2] = EEPROM_RD_buf[i];    
   break;
   
   case 0x03://WR+RD
   for(i=0;i<6;i++)EEPROM_WR_buf[i] = Data_rcv.DB[i+2];   
   EEPROM_Manipulate(EEPROM_WR_RD,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf);  
   for(i=0;i<6;i++)Flash_Data.DB[i+2] = EEPROM_RD_buf[i];     
   break;
   
   default :
   Flash_flag = 0x00;
   break;
 }
 if(Flash_flag)
 {
   if(R_CAN_TrmByTxBuf_CH0(0,(const can_frame_t *)&Flash_Data)==CAN_RTN_OK)
   {
     if(R_CAN_CheckTxBufResult_CH0(0)!=CAN_RTN_TX_END)NOP();
   }
   for(i=0;i<8;i++){EEPROM_WR_buf[i] = 0;EEPROM_RD_buf[i] = 0;}
   Flash_flag = 0;
 }
}

void Holl_EEPROM_Command(void)
{
	switch(HollCommandMode)
	{
		case 1://read
		HollCommandMode = 0;
		EEPROM_Manipulate(EEPROM_RD,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf); 
		HollCommandPosition = EEPROM_RD_buf[0]*256;
		HollCommandPosition += EEPROM_RD_buf[1];
		if((HollCommandPosition < HOLL_POSITION_MIN)||(HollCommandPosition > HOLL_POSITION_MAX))
		{
			HollCommandPosition = HOLL_POSITION_MAX;
			EEPROM_WR_buf[0] = (uint8_t)(HollCommandPosition/256);
			EEPROM_WR_buf[1] = (uint8_t)(HollCommandPosition%256);
			EEPROM_Manipulate(EEPROM_WR,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf);  			
		}
		break;
		
		case 2://write
		HollCommandMode = 0;
		if((HollCommandPosition < HOLL_POSITION_MIN)||(HollCommandPosition > HOLL_POSITION_MAX))
		HollCommandPosition = HOLL_POSITION_MAX;
		EEPROM_WR_buf[0] = (uint8_t)(HollCommandPosition/256);
		EEPROM_WR_buf[1] = (uint8_t)(HollCommandPosition%256);
		EEPROM_Manipulate(EEPROM_WR,EEPROM_ID_5,EEPROM_WR_buf,EEPROM_RD_buf); 		
		break;
		
		default :
		HollCommandMode = 0;
		break;
	}
	
}






  
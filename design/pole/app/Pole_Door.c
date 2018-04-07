#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "app.h"
#include "config.h"

volatile  uint8_t  	DOOR_STATUS;
volatile  uint8_t  	DOOR_STEP;



void Door_Procedure(void)
{
	switch(DOOR_STATUS)
	{
	case DOOR_STATUS_Open:
		if(PWM.Flag == 1)
		{
			PWM.Flag = 0;
			
			PoleSign_Clear();
  			DOOR_STATUS = DOOR_STATUS_Opening;		
		}
  		else if(PoleSign.KEY0   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY1   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY2   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.Lock   == 1){PWM.Flag = 1;}
		else if(PoleSign.CANid0 == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.CANid1 == 1){PWM.Flag = 1;Buzzer_NUM = 2;}	
		else PWM_Adjust();
	break;
		
	case DOOR_STATUS_Opening:
		if(PWM.Flag == 1)PWM_Stop();
  		else if(PoleSign.KEY0   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY1   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY2   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}		
		else if(PoleSign.CANid0 == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.CANid1 == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
  		else if(PoleSign.HollL  == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.HollL  == 2) {PWM.Flag = 1;Buzzer_NUM = 2;}//holl position error
		else if(PoleSign.HollL  == 3) {PWM.Flag = 1;Buzzer_NUM = 2;}//holl position error
  		else if(PoleSign.ADC1   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
  		else if(PoleSign.ADC2   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}	
		else if(PoleSign.PWM    == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}	
		else PWM_Adjust();
	break;

	case DOOR_STATUS_Close:
		if(PWM.Flag == 1)
		{
			PWM.Flag = 0;
			
			PoleSign_Clear();
  			DOOR_STATUS = DOOR_STATUS_Closing;			
		}
  		else if(PoleSign.KEY0   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY1   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY2   == 1){PWM.Flag = 1;Buzzer_NUM = 2;}		
		else if(PoleSign.CANid0 == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.CANid1 == 1){PWM.Flag = 1;Buzzer_NUM = 2;}
		else PWM_Adjust();
	break;
	
	case DOOR_STATUS_Closing:
		if(PWM.Flag == 1)PWM_Stop();
  		else if(PoleSign.KEY0   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY1   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.KEY2   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}		
		else if(PoleSign.CANid0 == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.CANid1 == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
  		else if(PoleSign.HollL  == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else if(PoleSign.HollL  == 3) {PWM.Flag = 1;Buzzer_NUM = 2;}//holl position error
  		else if(PoleSign.ADC1   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
  		else if(PoleSign.ADC2   == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}	
		else if(PoleSign.PWM    == 1) {PWM.Flag = 1;Buzzer_NUM = 2;}
		else PWM_Adjust();
	break;
	
	default :
	break;
	}
}


void DoorNip_Procedure(void)
{
	switch(DOOR_STATUS)
	{		
	case DOOR_STATUS_Opening:
		AntiNip.Flag = 0;
		PoleSign_Clear();
  		Brige1_Stop();
  		Brige2_Stop();
  		DOOR_STATUS = DOOR_STATUS_Close;	
	break;
	
	case DOOR_STATUS_Closing:
		AntiNip.Flag = 0;
		PoleSign_Clear();
  		Brige1_Stop();
  		Brige2_Stop();
  		DOOR_STATUS = DOOR_STATUS_Open;		
	break;
	
	default :
		AntiNip.Flag = 0;
	break;
	}
}








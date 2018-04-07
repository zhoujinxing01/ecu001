/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/F13 V2.03.00.02 [19 Apr 2017]
* Device(s)    : R5F10BGE
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 2018/2/26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "app.h"
//#include "config.h"
#include "iodefine.h"
#include "r_rl78_can_drv.h"
#include "eel_user_types.h"         /* EEL user types definition header file */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

volatile   uint8_t   tx_buff[10];
extern volatile    	   uint8_t  	Power_Switch;

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    TJA1041_Normal();//can
    R_CAN_Init();
    
    R_UART0_Start();//uart
    R_TAU0_Channel0_Start();//20k PWM
    R_TAU0_Channel6_Start();//4k PWM
    R_TAU1_Channel0_Start();//tick
    R_INTC3_Start();//HOLL
    //R_INTC9_Start();//LOW POWER
    R_ADC_Set_OperationOn();//ADC
    R_ADC_Start();    
   // flah_logic();
    Brige_ALL_Prohibit();
    
    Holl_Power_ON();
    ADC_CH_SW(1);
    
    Power_Switch = SW_ON;
    
    HOLL.LCNT = 20000;
    
    CAN_TXD_CNT = 0;
    CAN_RXD_CNT = 0;
    HollCommandMode = 1;//make sure the holl position
    PoleSign_Clear();
    
    P0 	&= 0xFE;//P00
    P9 &= 0xF9;
    Buzzer_NUM = 3;
    
    PoleSign.KEY0 = 1;
    LockUp.TimeOut = 100;//1s
    LockDown.Present = LockDown_Pull;
    //JOJO.Ratio = 9;
    
   // HollCommandPosition = HOLL_POSITION_MAX;
    //HollCommandMode = 2;//make sure the holl position
    while (1U)
    {
       	R_WDT_Restart();
	BaseTick();
       
	if(LOCK_Type == 1)Pole_Procedure_Up();
	else Pole_Procedure_Down();
	//HollCheck_Logic();
	//JOJO_Procedure();
	//Door_Procedure();
	
       	Pole_Uart();
	CAN_Logic();
	ADC_Logic();
	Holl_Logic();	
	FLASH_Copy();
	PWR_Switch();
	//Buzzer_Logic();	
	Buzzer_4KHz();
	DigitalInput();		
	Pole_System_ECO();
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    //R_CAN_Init();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

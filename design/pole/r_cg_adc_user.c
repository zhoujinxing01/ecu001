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
* File Name    : r_cg_adc_user.c
* Version      : CodeGenerator for RL78/F13 V2.03.00.02 [19 Apr 2017]
* Device(s)    : R5F10BGE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for ADC module.
* Creation Date: 2018/2/26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTAD r_adc_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_cg_serial.h"
#include "app.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint16_t ADC_data;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_adc_interrupt
* Description  : This function is INTAD interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_adc_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /*
    R_ADC_Get_Result(&ADC_data);
   switch(PoleADC.STEP)
   {
   case 0:
   PoleADC.AIN0 = (uint8_t)ADC_data;
   break;
   case 1:
   PoleADC.AIN1 = (uint8_t)ADC_data;
   break;
   case 2:
   PoleADC.AIN2 = (uint8_t)ADC_data;
   break;
   default :
   break;	   
   }   
   */
   uint8_t i;
   R_ADC_Get_Result(&ADC_data);
   switch(PoleADC.STEP)
   {
   case 0:
   PoleADC.AIN00Point++;
   i = PoleADC.AIN00Point%5;
   PoleADC.AIN00[i] = (uint8_t)ADC_data;
   break;
   case 1:
   PoleADC.AIN11Point++;
   i = PoleADC.AIN11Point%5;
   PoleADC.AIN11[i] = (uint8_t)ADC_data;
   break;
   case 2:
   PoleADC.AIN22Point++;
   i = PoleADC.AIN22Point%5;
   PoleADC.AIN22[i] = (uint8_t)ADC_data;
   break;
   default :
   break;	   
   }     
   /*
    uint8_t 	STEP;
    uint8_t 	AIN0;
    uint8_t 	AIN1;
    uint8_t 	AIN2;   
    uint8_t 	AIN00[5]; 
    uint8_t 	AIN11[5];
    uint8_t 	AIN22[5];
    uint8_t 	AIN00Point;
    uint8_t 	AIN11Point;
    uint8_t 	AIN22Point;
    */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

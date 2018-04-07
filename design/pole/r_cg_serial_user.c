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
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/F13 V2.03.00.02 [19 Apr 2017]
* Device(s)    : R5F10BGE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 2018/2/26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTST0 r_uart0_interrupt_send
#pragma interrupt INTSR0 r_uart0_interrupt_receive
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "app.h"
#include "config.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
/* Start user code for global. Do not edit comment generated here */
#define    UART_RCV_LEN    10
#define    UART_SEN_LEN    10
volatile   uint16_t  UART_RX_STA=0;
volatile   uint8_t   UART_RX_BUF[UART_RCV_LEN];
volatile   uint8_t   UART_TX_BUF[UART_SEN_LEN];
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        SDR00L = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
__interrupt static void r_uart0_interrupt_receive(void)//flag:0x0d 0x0a
{
    uint8_t rx_data;
    uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;
    rx_data = SDR01L;

    if((UART_RX_STA&0x8000)==0)//receive not complete
    {
	if(UART_RX_STA&0x4000)//received 0x0d
	{
	  if(rx_data!=0x0a)UART_RX_STA=0;//error,again
	  else UART_RX_STA|=0x8000;//received complete
	}
	else 
	{	
	  if(rx_data==0x0d)UART_RX_STA|=0x4000;
	  else
	  {
	    UART_RX_BUF[UART_RX_STA&0X3FFF]=rx_data ;
	    UART_RX_STA++;
	    if(UART_RX_STA>(UART_RCV_LEN-1))UART_RX_STA=0;  
	  }		 
	}
     }  
}

void Uart_Logic(void)
{
  uint16_t  len,i;
  uint8_t   DATA_flag;
	
  DATA_flag = 0;	
  if(UART_RX_STA&0x8000)
  {					   
    len	= UART_RX_STA&0x3fff;
    if(len < UART_RCV_LEN)DATA_flag = 1;
    else 
    {
      for(i=0;i<UART_RCV_LEN;i++)UART_RX_BUF[i] = 0;	
      UART_RX_STA = 0;			
    }
  }
  if(DATA_flag == 1)
  {
    switch(UART_RX_BUF[0])
    {
      case 0x00:
    	UART_TX_BUF[0] = 0x00;
    	UART_TX_BUF[1] = 0x01;
    	UART_TX_BUF[2] = 0x02;
    	UART_TX_BUF[3] = 0x03;
    	UART_TX_BUF[4] = 0x04;
    	UART_TX_BUF[5] = 0x05;
    	UART_TX_BUF[6] = 0x06;
    	UART_TX_BUF[7] = 0x07;
    	UART_TX_BUF[8] = 0x0D;
    	UART_TX_BUF[9] = 0x0A;      
	R_UART0_Send(UART_TX_BUF,UART_SEN_LEN);
        break;
      case 0x01:
    	UART_TX_BUF[9] = 0x00;
    	UART_TX_BUF[8] = 0x01;
    	UART_TX_BUF[7] = 0x02;
    	UART_TX_BUF[6] = 0x03;
    	UART_TX_BUF[5] = 0x04;
    	UART_TX_BUF[4] = 0x05;
    	UART_TX_BUF[3] = 0x06;
    	UART_TX_BUF[2] = 0x07;
    	UART_TX_BUF[1] = 0x0D;
    	UART_TX_BUF[0] = 0x0A;      
	R_UART0_Send(UART_TX_BUF,UART_SEN_LEN);      
        break;	
      case 0x02:
    	UART_TX_BUF[0] = UART_RX_BUF[0];
    	UART_TX_BUF[1] = UART_RX_BUF[1];
    	UART_TX_BUF[2] = UART_RX_BUF[2];
    	UART_TX_BUF[3] = UART_RX_BUF[3];
    	UART_TX_BUF[4] = UART_RX_BUF[4];
    	UART_TX_BUF[5] = UART_RX_BUF[5];
    	UART_TX_BUF[6] = UART_RX_BUF[6];
    	UART_TX_BUF[7] = UART_RX_BUF[7];
    	UART_TX_BUF[8] = UART_RX_BUF[8];
    	UART_TX_BUF[9] = UART_RX_BUF[9];      
	R_UART0_Send(UART_TX_BUF,UART_SEN_LEN);      
	break;	
	
      case 0x03:
       i = UART_RX_BUF[2];
       i *= 160;
       i /= 255;
       i *= 10;
       switch(UART_RX_BUF[1])
       {
	case 0x03://P125
	  TDR03 = i;
	  break;
	case 0x04://P13
	  TDR04 = i;
	  break;
	case 0x05://P15
	  TDR05 = i;
	  break;
	case 0x06://P14
	  TDR06 = i;
	  break;
	case 0x07://P120
	  TDR07 = i;
	  break;
	  
	case 0x11://P12
	  if(i)TDR11 = 0x1F40;
	  else TDR11 = 0;
	  break;
	case 0x12://P11
	  if(i)TDR12 = 0x1F40;
	  else TDR12 = 0;
	default :
	  break;
       }
       break;
       
      case 0x04:
       TJA1041_Sleep();
       //Halt();
       break;
       
      case 0x05:
      //TDR07 = _0FA0_TAU_TDR07_VALUE/2;
      Buzzer_Mode = 0;
      Buzzer_NUM = UART_RX_BUF[1];
      break;
      
      case 0x06:
      TDR07 = 0;
      break;
      
      case 0x07://LockPlan_Call
      LockPlan.Book = UART_RX_BUF[1];
      break;      
      
      case 0x08:  
      break;
       
      default :
	break;
    }	
    DATA_flag = 0;
    UART_RX_STA = 0;
    for(i=0;i<UART_RCV_LEN;i++)UART_RX_BUF[i] = 0;
  }	
}
/* End user code. Do not edit comment generated here */

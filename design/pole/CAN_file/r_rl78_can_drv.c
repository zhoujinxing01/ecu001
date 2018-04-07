/******************************************************************************* 
* DISCLAIMER 
* This software is supplied by Renesas Electronics Corporation and is only  
* intended for use with Renesas products. No other uses are authorized. This  
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. 
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS  
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the  
* following link: 
* http://www.renesas.com/disclaimer 
* 
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     
*******************************************************************************/
/******************************************************************************* 
* File Name    : r_rl78_can_drv.c 
* Version      : 1.0 
* Description  : This is source file for CAN driver code.
******************************************************************************/ 
/***************************************************************************** 
* History      : DD.MM.YYYY Version Description 
*              : 29.03.2013 1.00     First Release 
******************************************************************************/

/*****************************************************************************
 Includes   <System Includes> , "Project Includes"
 *****************************************************************************/
#pragma sfr
#pragma interrupt   INTCAN0TRM  R_CAN0_transmit_interrupt
#pragma interrupt   INTCANGERR  R_CAN_Global_error_interrupt
#pragma interrupt   INTCAN0ERR  R_CAN0_Global_channel_interrupt

#include "r_cg_macrodriver.h"
#include "iodefine.h"
#include "r_rl78_can_drv.h"
#include "r_rl78_can_sfr.h"
#include "r_can.h"
/*****************************************************************************
 Typedef definitions
 *****************************************************************************/
/*****************************************************************************
 Macro definitions
 *****************************************************************************/

#define CAN_FIFO_PTR_INC                    0xffUL

#define CAN_8_BITS_MASK                     0x00FFU

/*****************************************************************************
 Private variables and functions
 *****************************************************************************/
/*****************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *****************************************************************************/
void R_CAN_Port_init(void)
{
	/*
	//PU1
	//	0 	On-chip pull-up resistor not connected
	//	1 	On-chip pull-up resistor connected
	BIT_SET(PU1,0);
	BIT_SET(PU1,1);
	//P1	
	//	0 Output 0		Input low level
	//	1 Output 1 		Input high level
	BIT_SET(P1,0);
	BIT_SET(P1,1);
	//PM1
	//	0 Output mode (output buffer on)
	//	1 Input mode (output buffer off)
	BIT_CLR(PM1,0);
	BIT_SET(PM1,1);
	*/
	//CTXD0:P10.
	BIT_CLR(POM1,0);
	BIT_CLR(PM1,0);
	BIT_SET(P1,0);
	//CRXD0:P11.
	BIT_SET(PM1,1);
	BIT_CLR(PIM1,1);
}

void R_CAN_Port_init1(void)
{
	BIT_SET(PIOR4,6);//CAN I/O redirection
	//CTXD0:P72
	BIT_CLR(POM7,2);
	BIT_CLR(PM7,2);
	BIT_SET(P7,2);
	//CRXD0:P73
	BIT_SET(PM7,3);
	BIT_CLR(PIM7,3);
}
/*****************************************************************************
CAN RAM init
*****************************************************************************/
void CAN_RAM_Init(void)
{
	/*CAN0EN 	Control of CAN input clock supply/control of CANi wakeup interrupt
		0 	Stops input clock supply.
			Disables writing to the SFR used by CAN.
			CAN is in the reset state.
			Disables CANi wakeup interrupt.
		1 	Enables input clock supply.		---set
			Enables reading from and writing to the SFR used by CAN.
			Enables CANi wakeup interrupt
	*/
	CAN0EN=1;
	
    	/* ==== CAN RAM initialization ==== 
	*/
    	/*CAN Global Status Register (GSTS)
    	3 GRAMINIT 	CAN RAM Initialization Status Flag 
    		0: CAN RAM initialization is completed.		---check
		1: CAN RAM initialization is ongoing.
    	*/
    	while(GSTS & BIT(3))
    	{
        	//__nop();//It is better to add timeout function.
		NOP();
    	}
}

/************************************************************************
global mode switch (stop->reset
************************************************************************/
void Global_stop_to_reset_mode(void)
{
	/* ==== global mode switch (stop->reset) ==== 
	*/
    	/*CAN Global Status Register (GSTS)
    	2	GSLPSTS Global Stop Status Flag 
    		0: Not in global stop mode
		1: In global stop mode			---check
    	*/
    	if (GSTS & BIT(2))
    	{
        	/* exit global stop mode 
		*/
		/*CAN Global Control Register L (GCTRL)
		2	GSLPR Global Stop Mode 
			0: Other than global stop mode	---set
			1: Global stop mode
		*/
		BIT_CLR(GCTRL,2);
		/*CAN Global Status Register (GSTS)
	    	2	GSLPSTS Global Stop Status Flag 
	    		0: Not in global stop mode
			1: In global stop mode			---check
	    	*/
		while(GSTS & BIT(2))
        	{
            		//__nop();//It is better to add timeout function.
			NOP();
        	}
    	}
}

/************************************************************************
Channel mode switch (stop->reset
************************************************************************/
void Channel_stop_to_reset_mode(void)
{
	/* ==== channel mode switch (stop->reset)==== 
	*/
	/*CANi Status Register L (CiSTSL) (i = 0)
	2 CSLPSTS 	Channel Stop Status Flag 
		0: Not in channel stop mode
		1: In channel stop mode		---check
	*/
    	if (C0STSL & BIT(2))
    	{
       		/* exit channel stop mode 
		*/
		/*CANi Control Register L (CiCTRL) (i = 0)
		2 CSLPR Channel Stop Mode 
			0: Other than channel stop mode		---set
			1: Channel stop mode
		*/
		BIT_CLR(C0CTRL,2);
        	while(C0STSL & BIT(2))
        	{
            		//__nop();//It is better to add timeout function.
			NOP();
        	}
    	}
}

/************************************************************************
Global function setting
************************************************************************/
void Global_function_setting(void)
{
	/* ==== global function setting ==== */
	/*CAN Global Configuration Register L (GCFGL)
	12 TSSS 	Timestamp Clock Source Select 
		0: Clock obtained by frequency-dividing fCLK by 2 (fCLK/2)
							---set
		1: CANi bit time clock
	11 to 8 TSP[3:0] Timestamp Clock Source Division 
		b11 b10 b9 b8
		0 0 0 0 : Not divided			---set
		0 0 0 1 : Divided by 2
		0 0 1 0 : Divided by 4
		0 0 1 1 : Divided by 8
		0 1 0 0 : Divided by 16
		0 1 0 1 : Divided by 32
		0 1 1 0 : Divided by 64
		0 1 1 1 : Divided by 128
		1 0 0 0 : Divided by 256
		1 0 0 1 : Divided by 512
		1 0 1 0 : Divided by 1024
		1 0 1 1 : Divided by 2048
		1 1 0 0 : Divided by 4096
		1 1 0 1 : Divided by 8192
		1 1 1 0 : Divided by 16384
		1 1 1 1 : Divided by 32768
	4 DCS 	CAN Clock Source Select 
		0: Clock obtained by frequency-dividing fCLK by 2 (fCLK/2)
							---set 
		1: X1 clock (fx)
	3 MME 	Mirror Function Enable 
		0: Mirror function is disabled.		---set
		1: Mirror function is enabled.
	2 DRE 	DLC Replacement Enable 
		0: DLC replacement is disabled.		---set
		1: DLC replacement is enabled.
	1 DCE 	DLC Check Enable 
		0: DLC check is disabled.		---set
		1: DLC check is enabled.
	0 TPRI 	Transmit Priority Select 
		0: ID priority				---set
		1: Transmit buffer number priority
	------------
	*/
    	GCFGL = 0x0000;
	/*CAN Global Configuration Register H (GCFGH)
	These bits are used to set a clock source division value of the interval timer for FIFO buffers.
	*/
    	GCFGH = 0x0001;
}

/********************************************************************
==== communication speed setting ==== 
fCAN: Frequency of CAN clock (selected by the DCS bit in the GCFGL register)	(16MHz)
Tq: Time quantum
i = 0
CANiTq = (BRP[9:0] bits in the CiCFGL register + 1)/fCAN
CANi bit time = CANiTq x Tq count per bit
Communication speed (CANi bit time clock) = 1/CANi bit time
1MHz:
	fCAN:16MHz
	C0CFGH = 0x0349;
	C0CFGL = 0x0000;
********************************************************************/
void Communication_speed_setting(void)
{
	/* ==== communication speed setting ==== */
	/*CANi Bit Configuration Register L (CiCFGL) (i = 0)
	9 to 0 BRP[9:0] 	Prescaler Division Ratio Set 
	--When these bits are set to P (0 to 1023), the baud rate
	prescaler divides fCAN by P + 1
	*/
    	//C0CFGL = 0x0000;	//0
	C0CFGL = 0x0001;	//500kps
	//C0CFGL = 0x0003;	//250kps
	//C0CFGL = 0x0007;	//125kps
	
	/* CANi Bit Configuration Register H (CiCFGH) (i = 0)
	9, 8 SJW[1:0] 	Resynchronization Jump Width Control
		b9 b8
		0 0 : 1 Tq
		0 1 : 2 Tq
		1 0 : 3 Tq
		1 1 : 4 Tq			---set
	6 to 4 TSEG2[2:0]	Time Segment 2 Control b6 b5 b4
		0 0 0 : Setting prohibited
		0 0 1 : 2 Tq
		0 1 0 : 3 Tq
		0 1 1 : 4 Tq
		1 0 0 : 5 Tq			---set
		1 0 1 : 6 Tq
		1 1 0 : 7 Tq
		1 1 1 : 8 Tq
	3 to 0 TSEG1[3:0]	Time Segment 1 Control b3 b2 b1 b0
		0 0 0 0 : Setting prohibited
		0 0 0 1 : Setting prohibited
		0 0 1 0 : Setting prohibited
		0 0 1 1 : 4 Tq
		0 1 0 0 : 5 Tq
		0 1 0 1 : 6 Tq
		0 1 1 0 : 7 Tq
		0 1 1 1 : 8 Tq
		1 0 0 0 : 9 Tq
		1 0 0 1 : 10 Tq			---set
		1 0 1 0 : 11 Tq
		1 0 1 1 : 12 Tq
		1 1 0 0 : 13 Tq
		1 1 0 1 : 14 Tq
		1 1 1 0 : 15 Tq
		1 1 1 1 : 16 Tq
	*/
    	//C0CFGH = 0x0349;	//1+10+5+4  (16)
	C0CFGH = 0x03A;	//1+11+4+0  (16)
	
}

/****************************************************************************
==== Rx rule setting ==== 
****************************************************************************/
void  Rx_rule_setting(void)
{
	//#if CAN_RX_RULE_NUM > 0
    	//{
	volatile __near can_rxrule_sfr_t * p_RxRuleSfr;
	uint16_t temp_rpage;
	uint16_t rxrule_idx;

	/* ---- Set Rx rule number per channel ---- */
	/*CAN Receive Rule Number Configuration Register (GAFLCFG)
	4 to 0 RNC0[4:0] CANi Receive Rule Number Set 
		Set the number of receive rules of channel 0.
		Set these bits to a value within a range of H'00 to H'10.
	*/
	//GAFLCFG = 0x01;
	GAFLCFG = 0x03;

	/* ---- Save value of GRWCR register ---- */
	/*CAN Global RAM Window Control Register (GRWCR)
	0 RPAGE 	RAM Window Select
		0: Selects window 0 (receive rule entry registers, CAN RAM test registers)
		1: --Selects window 1 (receive buffer, receive FIFO buffer, transmit/receive FIFO buffer, transmit buffer, 
		  transmit history data access register)
	*/
	temp_rpage = GRWCR;

	/* ---- Select window 0 ---- */
	BIT_CLR(GRWCR,0);
	/* ---- Copy Rx rule one by one ---- */
	//0x0001U, 0x0000U, 0x07FFU, 0xC000U, 0x8000U, 0x0000U
	/*CAN Receive Rule Entry Register jAL (GAFLIDLj) (j = 0 to 15)
	15 to 0 	GAFLID[15:0] 	ID Set L
	Set the ID of the receive rule.
	--For the standard ID, set the ID in bits 10 to 0 and set bits
	15 to 11 to 0
	*/
	/*CAN Receive Rule Entry Register jAH (GAFLIDHj) (j = 0 to 15)
	15 GAFLIDE 	IDE Select 
		0: Standard ID
		1: Extended ID
	14 GAFLRTR 	RTR Select 
		0: Data frame
		1: Remote frame
	13 GAFLLB 	Receive Rule Target Message Select
		0: When a message transmitted from another CAN node is received
		1: When a message transmitted from own node is received
	12 to 0 GAFLID[28:16] 	ID Set H 
		Set the ID of the receive rule.
		For the standard ID, set these bits to 0.
	*/
	/*CAN Receive Rule Entry Register jBL (GAFLMLj) (j = 0 to 15)
	15 to 0 GAFLIDM[15:0]	ID Mask L
		0: The corresponding ID bit is not compared.
		1: The corresponding ID bit is compared.
	*/
	/*CAN Receive Rule Entry Register jBH (GAFLMHj) (j = 0 to 15)
	15 GAFLIDEM 	IDE Mask 
		0: The IDE bit is not compared.
		1: The IDE bit is compared.
	14 GAFLRTRM 	RTR Mask 
		0: The RTR bit is not compared.
		1: The RTR bit is compared
	12 to 0 GAFLIDM[28:16]	ID Mask H
		0: The corresponding ID bit is not compared.
		1: The corresponding ID bit is compared.
	*/
	/*CAN Receive Rule Entry Register jCL (GAFLPLj) (j = 0 to 15)
	15 GAFLRMV 	Receive Buffer Enable 
		0: No receive buffer is used.
		1: A receive buffer is used.
	14 to 8 GAFLRMDP[6:0] 	Receive Buffer Number Select 
		Set the receive buffer number to store receive messages.(n (n = 0 to 15): Receive buffer number)
	4 GAFLFDP4 	CAN0 Transmit/Receive FIFO Buffer Select 0
		0: Not select a CAN0 transmit/receive FIFO buffer 0
		1: Select a CAN0 transmit/receive FIFO buffer 0
	1 GAFLFDP1 	Receive FIFO Buffer Select 1
		0: Not select a receive FIFO buffer 1
		1: Select a receive FIFO buffer 1
	0 GAFLFDP0 	Receive FIFO Buffer Select 0
		0: Not select a receive FIFO buffer 0
		1: Select a receive FIFO buffer 0
	
	*/
	/*CAN Receive Rule Entry Register jCH (GAFLPHj) (j = 0 to 15)
	15 to 12 GAFLDLC[3:0]	Receive Rule DLC
		b15 b14 b13 b12
		0 0 0 0 : DLC check is disabled.
		0 0 0 1 : 1 data byte
		0 0 1 0 : 2 data bytes
		0 0 1 1 : 3 data bytes
		0 1 0 0 : 4 data bytes
		0 1 0 1 : 5 data bytes
		0 1 1 0 : 6 data bytes
		0 1 1 1 : 7 data bytes
		1 X X X : 8 data bytes
	11 to 0 GAFLPTR[11:0]
		Receive Rule Label Set the 12-bit label information.
	*/
	p_RxRuleSfr = (volatile __near can_rxrule_sfr_t *)&GAFLIDL0;
	for (rxrule_idx = 0U; rxrule_idx < CAN_RX_RULE_NUM; rxrule_idx++)
	{
    		/* Set a single Rx rule */
            	p_RxRuleSfr->IDL = g_rxrule_table[rxrule_idx][0];
            	p_RxRuleSfr->IDH = g_rxrule_table[rxrule_idx][1];
           	p_RxRuleSfr->ML  = g_rxrule_table[rxrule_idx][2];
           	p_RxRuleSfr->MH  = g_rxrule_table[rxrule_idx][3];
            	p_RxRuleSfr->PL  = g_rxrule_table[rxrule_idx][4];
            	p_RxRuleSfr->PH  = g_rxrule_table[rxrule_idx][5];
	
            	/* Next sfr */
            	p_RxRuleSfr++;
	}

	/* ---- restore value of GRWCR register ---- */
	GRWCR = temp_rpage;
    	//}
	//#endif /* CAN_RX_RULE_NUM > 0 */
}

/****************************************************************************
==== buffer setting ==== 
****************************************************************************/
void Buffer_and_Interrupt_setting(void)
{
	/*
	INTCAN0TRM 	CAN0 channel transmit 	0054H
	*/
	/*disable INTWDTI interrupt 
	21.3.2 Interrupt mask flag registers (MK0L, MK0H, MK1L, MK1H, MK2L, MK2H, MK3L)
	MKxx 	Interrupt servicing control
	0 	Interrupt servicing enabled
	1 	Interrupt servicing disabled	---set
	*/
	CAN0TRMMK = 1U;
	/*clear INTWDTI interrupt flag
	21.3.1 Interrupt request flag registers (IF0L, IF0H, IF1L, IF1H, IF2L, IF2H, IF3L)
	IFxx 	Interrupt request flag
	0 	No interrupt request signal is generated		---set
	1 	Interrupt request is generated, interrupt request status
	*/
	CAN0TRMIF = 0U; 
	/* Set INTWDTI low priority 
	21.3.3 Priority specification flag registers (PR00L, PR00H, PR01L, PR01H, PR02L, PR02H, PR03L, PR10L, PR10H,
PR11L, PR11H, PR12L, PR12H, PR13L)
	XXPR1X XXPR0X 	Priority level selection
	0 	0 	Specify level 0 (high priority level)
	0 	1 	Specify level 1
	1 	0 	Specify level 2
	1 	1 	Specify level 3 (low priority level)	---set
	*/
	CAN0TRMPR1 = 1U;
	CAN0TRMPR0 = 1U;
	
	/*
	INTCANGERR 	CAN global error 	0058H
	*/
	CANGERRMK = 1U;
	CANGERRIF = 0U;
	CANGERRPR1 = 1U;
	CANGERRPR0 = 1U;
	
	/*
	INTCAN0ERR 	CAN0 channel error Internal 	004EH
	*/
	CAN0ERRMK = 1U;
	CAN0ERRIF = 0U;
	CAN0ERRPR1 = 1U;
	CAN0ERRPR0 = 1U;
	
    	/* ---- Set Rx buffer number ---- */
	/* CAN Receive Buffer Number Configuration Register (RMNB)
	4 to 0 NRXMB[4:0] 	Receive Buffer Number Configuration
		These bits are used to set the total number of receive buffers of the CAN module.
		Set a value of 0 to 16.
		Setting these bits to all 0 makes receive buffers unavailable.
	*/
    	RMNB = 1;

    	/* ---- Set Rx FIFO buffer ---- */
	/*CAN Receive FIFO Control Register m (RFCCm) (m = 0, 1)
	15 to13	RFIGCV[2:0] Receive FIFO Interrupt Request Timing Select
		b15 b14 b13
		0 0 0 : When FIFO is 1/8 full.			---set
		0 0 1 : When FIFO is 2/8 full.
		0 1 0 : When FIFO is 3/8 full.
		0 1 1 : When FIFO is 4/8 full.
		1 0 0 : When FIFO is 5/8 full.
		1 0 1 : When FIFO is 6/8 full.
		1 1 0 : When FIFO is 7/8 full.
		1 1 1 : When FIFO is full.
	12 RFIM 	Receive FIFO Interrupt Source Select
		0: An interrupt occurs when the condition set by the RFIGCV[2:0] bits is met.
								---set
		1: An interrupt occurs each time a message has been received.
	10 to 8 RFDC[2:0] Receive FIFO Buffer Depth Configuration
		b10 b9 b8
		0 0 0 : 0 messages				---set
		0 0 1 : 4 messages
		0 1 0 : 8 messages
		0 1 1 : 16 messages
	1 RFIE 	Receive FIFO Interrupt Enable 
		0: Receive FIFO interrupt is disabled.		---set
		1: Receive FIFO interrupt is enabled.
	0 RFE 	Receive FIFO Buffer Enable 
		0: No receive FIFO buffer is used.		---set
		1: Receive FIFO buffers are used.
	*/
    	RFCC0 = 0x0000U;
    	RFCC1 = 0x0000U;

    	/* ---- Set common (Tx/Rx) FIFO buffer ---- */
	/*CANi Transmit/Receive FIFO Control Register kL (CFCCLk) (i = 0) (k = 0)
	15 to 13 CFIGCV[2:0] 	Transmit/Receive FIFO Receive Interrupt Request Timing Select
		b15 b14 b13
		0 0 0 : When FIFO is 1/8 full.				---set
		0 0 1 : When FIFO is 2/8 full.
		0 1 0 : When FIFO is 3/8 full.
		0 1 1 : When FIFO is 4/8 full.
		1 0 0 : When FIFO is 5/8 full.
		1 0 1 : When FIFO is 6/8 full.
		1 1 0 : When FIFO is 7/8 full.
		1 1 1 : When FIFO is full.
	12 CFIM 	Transmit/Receive FIFO Interrupt Source Select
		0:							---set
			Receive mode:
			--When the number of received messages has met the
			condition set by the CFIGCV[2:0] bits, a FIFO receive
			interrupt request is generated.
			Transmit mode:
			--When the buffer becomes empty upon completion of
			message transmission, a FIFO transmit interrupt
			request is generated.
		1:
			Receive mode:
			A FIFO receive interrupt request is generated each
			time a message has been received.
			Transmit mode:
			A FIFO transmit interrupt request is generated each
			time a message has been transmitted.
	10 to 8 CFDC[2:0] Transmit/Receive FIFO Buffer Depth Configuration
		b10 b9 b8
		0 0 0 : 0 messages					---set
		0 0 1 : 4 messages
		0 1 0 : 8 messages
		0 1 1 : 16 messages
	2 CFTXIE 	Transmit/Receive FIFO Transmit Interrupt Enable
		0: Transmit/receive FIFO transmit interrupt is disabled.---set
		1: Transmit/receive FIFO transmit interrupt is enabled.
	1 CFRXIE  Transmit/Receive FIFO Receive Interrupt Enable
		0: Transmit/receive FIFO receive interrupt is disabled.	---set
		1: Transmit/receive FIFO receive interrupt is enabled.
	0 CFE Transmit/Receive FIFO Buffer Enable
		0: No transmit/receive FIFO buffer is used.		---set
		1: Transmit/receive FIFO buffers are used.
	*/
    	CFCCL0 = 0x0000U;
	//CFCCL0 = 0x04;//by zhou...
	/* CANi Transmit/Receive FIFO Control Register kH (CFCCHk) (i = 0) (k = 0)
	15 to 8 CFITT[7:0] 	Message Transmission Interval Configuration
		Set a message transmission interval.
		Set these bits to a value within a range of H'00 to H'FF
								---set 0
	5, 4 CFTML[1:0] 	Transmit Buffer Link Configuration 
		Set the transmit buffer number to be linked to the transmit/receive FIFO buffer.
								---set 0
	3 CFITR 	Transmit/Receive FIFO Interval Timer Resolution
		0: Clock obtained by frequency-dividing fCLK/2 by the ITRCP[15:0] value
								---set
		1: Clock obtained by frequency-dividing fCLK/2 by the ITRCP[15:0] value ¡Á 10
	2 CFITSS 	Interval Timer Clock Source Select 
		0: Clock selected by the CFITR bit		---set
		1: CANi bit time clock
	1, 0 CFM[1:0] 	Transmit/Receive FIFO Mode Select b1 b0
		0 0 : Receive mode				---set
		0 1 : Transmit mode
		1 0 : Setting prohibited
		1 1 : Setting prohibited
	*/
    	CFCCH0 = 0x0000U;
	
	/* ---- Tx buffer transmission complete interrupt ---- */
	/*CANi Transmit Buffer Interrupt Enable Register (TMIEC) (i = 0)
	3 TMIE3 CANi Transmit Buffer 3 Interrupt Enable
	2 TMIE2 CANi Transmit Buffer 2 Interrupt Enable
	1 TMIE1 CANi Transmit Buffer 1 Interrupt Enable
	0 TMIE0 CANi Transmit Buffer 0 Interrupt Enable		---set 1
		0: Transmit buffer interrupt is disabled
		1: Transmit buffer interrupt is enabled
	-------------------------------------------------------------
	INTCAN0TRM 	CAN0 channel transmit 		0054H
	*/
    	TMIEC = 0x0001U;

    	/* ==== global error interrupt setting ==== */
	/*CAN Global Control Register L (GCTRL)
	After Reset : 0 0 0 0, 0 0 0 0, 0 0 0 0, 0 1 0 1
	10 THLEIE Transmit History Buffer Overflow Interrupt Enable
		0: Transmit history buffer overflow interrupt is disabled.
								---set
		1: Transmit history buffer overflow interrupt is enabled.
	9 MEIE 	FIFO Message Lost Interrupt Enable 
		0: FIFO message lost interrupt is disabled.	---set
		1: FIFO message lost interrupt is enabled.
	8 DEIE 	DLC Error Interrupt Enable 
		0: DLC error interrupt is disabled.		---set
		1: DLC error interrupt is enabled.
	2 GSLPR Global Stop Mode 
		0: Other than global stop mode
		1: Global stop mode
	1, 0 GMDC[1:0] 	Global Mode Select b1 b0
		0 0 : Global operating mode
		0 1 : Global reset mode
		1 0 : Global test mode
	*/
    	GCTRL = GCTRL & 0x07;
	
	/* ==== channel function setting ==== */
	/*CANi Error Flag Register L (CiERFLL) (i = 0)
	14 ADERR 	ACK Delimiter Error Flag 
		0: No ACK delimiter error is detected.	---set
		1: ACK delimiter error is detected.
	13 B0ERR 	Dominant Bit Error Flag 
		0: No dominant bit error is detected.	---set
		1: Dominant bit error is detected.
	12 B1ERR 	Recessive Bit Error Flag 
		0: No recessive bit error is detected.	---set
		1: Recessive bit error is detected.
	11 CERR 	CRC Error Flag 
		0: No CRC error is detected.		---set
		1: CRC error is detected.
	10 AERR 	ACK Error Flag 
		0: No ACK error is detected.		---set
		1: ACK error is detected.
	9 FERR 		Form Error Flag 
		0: No form error is detected.		---set
		1: Form error is detected.
	8 SERR 	Stuff Error Flag 
		0: No stuff error is detected.		---set
		1: Stuff error is detected.
	7 ALF 	Arbitration Lost Flag 
		0: No arbitration lost is detected.	---set
		1: Arbitration lost is detected.
	6 BLF	Bus Lock Flag 
		0: No channel bus lock is detected.	---set
		1: Channel bus lock is detected.
	5 OVLF 	Overload Flag 
		0: No overload is detected.		---set
		1: Overload is detected.
	4 BORF 	Bus Off Recovery Flag 
		0: No bus off recovery is detected.	---set
		1: Bus off recovery is detected.
	3 BOEF 	Bus Off Entry Flag 
		0: No bus off entry is detected.	---set
		1: Bus off entry is detected.
	2 EPF 	Error Passive Flag 
		0: No error passive is detected.	---set
		1: Error passive is detected.
	1 EWF 	Error Warning Flag 
		0: No error warning is detected.	---set
		1: Error warning is detected.
	0 BEF 	Bus Error Flag 
		0: No channel bus error is detected.	---set
		1: Channel bus error is detected.
	*/
	C0ERFLL=0x0000;
	/*CANi Control Register L (CiCTRL) (i = 0)
	15 ALIE 	Arbitration Lost Interrupt Enable 
		0: Arbitration lost interrupt is disabled.
		1: Arbitration lost interrupt is enabled. 		---set
	14 BLIE Bus Lock Interrupt Enable 
		0: Bus lock interrupt is disabled.
		1: Bus lock interrupt is enabled.			---set
	13 OLIE Overload Frame Transmit Interrupt Enable
		0: Overload frame transmit interrupt is disabled.
		1: Overload frame transmit interrupt is enabled.	---set
	12 BORIE Bus Off Recovery Interrupt Enable 
		0: Bus off recovery interrupt is disabled.
		1: Bus off recovery interrupt is enabled.		---set
	11 BOEIE Bus Off Entry Interrupt Enable 
		0: Bus off entry interrupt is disabled.
		1: Bus off entry interrupt is enabled.			---set
	10 EPIE Error Passive Interrupt Enable 
		0: Error passive interrupt is disabled.
		1: Error passive interrupt is enabled.			---set
	9 EWIE Error Warning Interrupt Enable 
		0: Error warning interrupt is disabled.			---set
		1: Error warning interrupt is enabled.
	8 BEIE 	Protocol Error Interrupt Enable 
		0: Protocol error interrupt is disabled.		---set
		1: Protocol error interrupt is enabled.
	3 RTBO Forcible Return from Bus-off 
		--When this bit is set to 1, forcible return from the bus off
		state is made. This bit is always read as 0.		---set 0
	2 CSLPR Channel Stop Mode 
		0: Other than channel stop mode
		1: Channel stop mode
	1, 0 CHMDC[1:0] Mode Select 
		b1 b0
		0 0: Channel communication mode
		0 1: Channel reset mode
		1 0: Channel halt mode
		1 1: Setting prohibited
	*/
    	C0CTRL &= 0x0007;
	//C0CTRL |= 0xFC00;
	/*CANi Control Register H (CiCTRH) (i = 0)
	After Reset:0x00.
	10, 9 CTMS[1:0] Communication Test Mode Select
		b10 b9
		0 0: Standard test mode					---set
		0 1: Listen-only mode
		1 0: Self-test mode 0 (external loopback mode)
		1 1: Self-test mode 1 (internal loopback mode)
	8 CTME 	Communication Test Mode Enable
		0: Communication test mode is disabled.			---set
		1: Communication test mode is enabled.
	7 ERRD 	Error Display Mode Select 
		0: --Only the first error is indicated after bits 14 to 8 in the
		   CiERFLL register have all been cleared.		---set
		1: The error flags of all errors are indicated.
	6, 5 BOM[1:0] 	Bus Off Recovery Mode Select 
		b6 b5
		0 0: ISO11898-1 compliant				---set
		0 1: Entry to channel halt mode at bus-off entry
		1 0: Entry to channel halt mode at bus-off end
		1 1: Entry to channel halt mode (in the bus off state) by a program request
	0 TAIE 	Transmit Abort Interrupt Enable 
		0: Transmit abort interrupt is disabled.		---set
		1: Transmit abort interrupt is enabled.
	*/
    	C0CTRH = C0CTRH;
	
	
	
	/*enable INTWDTI interrupt 
	21.3.2 Interrupt mask flag registers (MK0L, MK0H, MK1L, MK1H, MK2L, MK2H, MK3L)
	MKxx 	Interrupt servicing control
	0 	Interrupt servicing enabled	---set
	1 	Interrupt servicing disabled
	*/
	CAN0TRMMK = 0U;
	//CANGERRMK = 0U;
	//CAN0ERRMK = 0U;
}

/****************************************************************************** 
* Function Name: Global_reset_to_communication_mode
* Description  : Global_reset_to_communication_mode
* Arguments    : none 
* Return Value : none
******************************************************************************/
void Global_reset_to_communication_mode(void)
{

    	/* ==== switch to global operation mode from global reset mode ==== */
    	/*18.3.13 CAN Global Status Register (GSTS)
    	After Reset:0x000d.
    	3 GRAMINIT 	CAN RAM Initialization Status Flag 
		0: CAN RAM initialization is completed.
		1: CAN RAM initialization is ongoing.
	2 GSLPSTS 	Global Stop Status Flag 
		0: Not in global stop mode
		1: In global stop mode
	1 GHLTSTS 	Global Test Status Flag 
		0: Not in global test mode
		1: In global test mode
	0 GRSTSTS 	Global Reset Status Flag 
		0: Not in global reset mode
		1: In global reset mode		---check
    	*/
	if (GSTS & BIT(0))
	{
		/*CAN Global Control Register L (GCTRL)
		1, 0 GMDC[1:0]	 Global Mode Select
			b1 b0
			0 0 : Global operating mode	---set
			0 1 : Global reset mode
			1 0 : Global test mode
		*/
		GCTRL = GCTRL & (uint16_t)~BITS_2_ON(0);
		while((GSTS & BIT_ON(0)) != 0U)
		{
		    //__nop();
		    NOP();
		}
	}

    	/* ==== Global error ==== */
	/*CAN Global Error Flag Register (GERFLL)
	2 THLES 	Transmit History Buffer Overflow Status Flag
		0: No transmit history buffer overflow is present.
								---set
		1: A transmit history buffer overflow is present.
	1 MES 		FIFO Message Lost Status Flag 
		0: No FIFO message lost error is present.	---set
		1: A FIFO message lost error is present.
	0 DEF 		DLC Error Flag 
		0: No DLC error is present.			---set
		1: A DLC error is present.
	*/
    	GERFLL = 0x0U;

    	/* ==== enable reception FIFO ==== */
    	if (g_rxfifo0_use_mode != CAN_NOUSE)
    	{
		/*CAN Receive FIFO Control Register m (RFCCm) (m = 0, 1)
		0 RFE 	Receive FIFO Buffer Enable 
			0: No receive FIFO buffer is used.
			1: Receive FIFO buffers are used.	---set
		*/
		BIT_SET(RFCC0,0);
    	}

    	if (g_rxfifo1_use_mode != CAN_NOUSE)
    	{
		/*CAN Receive FIFO Control Register m (RFCCm) (m = 0, 1)
		0 RFE 	Receive FIFO Buffer Enable 
			0: No receive FIFO buffer is used.
			1: Receive FIFO buffers are used.	---set
		*/
		BIT_SET(RFCC1,0);
    	}

    	/* ==== Tx/Rx FIFO setting ==== */
    	/* ---- enable Tx/Rx FIFO (Rx mode) ---- */
    	if (g_trfifo_use_mode == CAN_USE_RX_MODE)
    	{
		/*CANi Transmit/Receive FIFO Control Register kL (CFCCLk) (i = 0) (k = 0)
		0 CFE 	Transmit/Receive FIFO Buffer Enable
			0: No transmit/receive FIFO buffer is used.
			1: Transmit/receive FIFO buffers are used.	---set
		*/
		BIT_SET(CFCCL0,0);
    	}

}

/****************************************************************************** 
* Function Name: Channel_reset_to_communication_mode
* Description  : Channel_reset_to_communication_mode(Channel 0)
	
* Arguments    : none
* Return Value : none
******************************************************************************/
void Channel_reset_to_communication_mode(void)
{
    	/* ---- switch to channel operation mode ---- */
	/*CANi Status Register L (CiSTSL) (i = 0)
	After Reset :0x05
	0 CRSTSTS 	Channel Reset Status Flag 
		0: Not in channel reset mode
		1: In channel reset mode	---check
	*/
    	if (C0STSL & BIT(0))
    	{
		/*CANi Control Register L (CiCTRL) (i = 0)
		1, 0 CHMDC[1:0]	Mode Select
			b1 b0
			0 0: Channel communication mode		---set
			0 1: Channel reset mode
			1 0: Channel halt mode
		*/
	        C0CTRL = C0CTRL & (uint16_t)~BITS_2_ON(0);
	        while(C0STSL & BIT(0))
	        {
	        	//__nop();
			NOP();
	        }
    	}

    	/* ---- enable Tx/Rx FIFO (Tx mode) ---- */
    	if (g_trfifo_use_mode == CAN_USE_TX_MODE)
    	{
		/*CANi Transmit/Receive FIFO Control Register kL (CFCCLk) (i = 0) (k = 0)
		0 CFE 	Transmit/Receive FIFO Buffer Enable
			0: No transmit/receive FIFO buffer is used.
			1: Transmit/receive FIFO buffers are used.		---set
		*/
		BIT_SET(CFCCL0,0);
    	}
}

/****************************************************************************** 
* Function Name: R_Can_Init
* Description  : Initialize CAN controller after reset
* Arguments    : none 
* Return Value : none
******************************************************************************/
 void R_CAN_Init(void)
{
	//R_CAN_Port_init();//CTXD0:P10,CRXD0:P11
	R_CAN_Port_init1();//CTXD0:P72,CRXD0:P73
	
	CAN_RAM_Init();

	Global_stop_to_reset_mode();
    	
	Channel_stop_to_reset_mode();

	Global_function_setting();
    	
	Communication_speed_setting();//500K
    	
	Rx_rule_setting();
    
	Buffer_and_Interrupt_setting();
	
	Global_reset_to_communication_mode();
	
	Channel_reset_to_communication_mode();
}



/****************************************************************************** 
* Function Name: R_CAN_TrmByTxBuf_CH0
* Description  : Transmit a frame by Tx buffer (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
*                pFrame -
*                    pointer to frame to be transmitted
* Return Value : CAN_RTN_OK -
*                    normal completion
*                CAN_RTN_STS_ERROR -
*                    failure to clear Tx buffer status
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_TrmByTxBuf_CH0(can_txbuf_t txbuf_idx,const can_frame_t * pFrame)
{

    /* ---- Clear Tx buffer status ---- */
    {
	volatile __near uint8_t * p_TMSTSp;

	/*CANi Transmit Buffer Status Register p (TMSTSp) (i = 0) (p = 0 to 3)
	4 TMTARM 	Transmit Buffer Transmit Abort Request Status Flag
		0: No transmit abort request is present.	---set
		1: A transmit abort request is present.
	3 TMTRM 	Transmit Buffer Transmit Request Status Flag
		0: No transmit request is present.		---set
		1: A transmit request is present.
	2, 1 TMTRF[1:0] Transmit Buffer Transmit Result Flag b2 b1
		0 0 : Transmission is in progress or no transmit request is present.
								---set
		0 1 : Transmit abort has been completed.
		1 0 : Transmission has been completed (without transmit abort request).
		1 1 : Transmission has been completed (with transmit abort request).
	0 TMTSTS 	Transmit Buffer Transmit Status Flag 
		0: Transmission is not in progress.		---set
		1: Transmission is in progress.
	*/
	p_TMSTSp = CAN_ADDR_TMSTSp(txbuf_idx);
	*p_TMSTSp = 0x0U;
	if (*p_TMSTSp != 0x0U)
	{
	    return CAN_RTN_STS_ERROR;
	}
    }

    /* ---- Store message to tx buffer ---- */
    {
        volatile __near can_frame_sfr_t * p_TxMsgSfr;
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
	/*CAN Global RAM Window Control Register (GRWCR)
	0 RPAGE 	RAM Window Select
		0: Selects window 0 (receive rule entry registers, CAN RAM test registers)
		1: --Selects window 1 (receive buffer, receive FIFO buffer, transmit/receive FIFO buffer, transmit buffer, 
		  transmit history data access register)
	*/
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= BIT_ON(0);

        /* ---- Set frame data ---- */
	/*CANi Transmit Buffer Register pAL (TMIDLp) (i = 0) (p = 0 to 3)
	15 to 0 TMID[15:0] 	Transmit Buffer ID Data L
		Set standard ID or extended ID.
		For standard ID, write an ID to bits 10 to 0 and write 0 to bits 15 to 11.
	*/
	/*CANi Transmit Buffer Register pAH (TMIDHp) (i = 0) (p = 0 to 3)
	15 TMIDE	Transmit Buffer IDE 
		0: Standard ID
		1: Extended ID
	14 TMRTR 	Transmit Buffer RTR 
		0: Data frame
		1: Remote frame
	13 THLEN 	Transmit History Data Store Enable 
		0: Transmit history data is not stored in the buffer.
		1: Transmit history data is stored in the buffer.
	12 to 0 TMID[28:16] 	Transmit Buffer ID Data H 
		Set standard ID or extended ID.
		For standard ID, write 0 to these bits.
	*/
	/*CANi Transmit Buffer Register pBH (TMPTRp) (i = 0) (p = 0 to 3)
	15 to 12 TMDLC[3:0] 	Transmit Buffer DLC Data 
		b15 b14 b13 b12
		0 0 0 0 : 0 data bytes
		0 0 0 1 : 1 data byte
		0 0 1 0 : 2 data bytes
		0 0 1 1 : 3 data bytes
		0 1 0 0 : 4 data bytes
		0 1 0 1 : 5 data bytes
		0 1 1 0 : 6 data bytes
		0 1 1 1 : 7 data bytes
		1 X X X : 8 data bytes
	7 to 0 TMPTR[7:0] 	Transmit Buffer Label Data
		Set the label information to be stored in the transmit history buffer.
	*/
	/*CANi Transmit Buffer Register pCL (TMDF0p) (i = 0) (p = 0 to 3)
		15 to 8 TMDB1[7:0] 	Transmit Buffer Data Byte 1 
		7 to 0 TMDB0[7:0] 	Transmit Buffer Data Byte 0
	*/
	/*CANi Transmit Buffer Register pCL (TMDF1p) (i = 0) (p = 0 to 3)
		15 to 8 TMDB3[7:0] 	Transmit Buffer Data Byte 3 
		7 to 0 TMDB2[7:0] 	Transmit Buffer Data Byte 2
	*/
	/*CANi Transmit Buffer Register pCL (TMDF2p) (i = 0) (p = 0 to 3)
		15 to 8 TMDB5[7:0] 	Transmit Buffer Data Byte 5
		7 to 0 TMDB4[7:0] 	Transmit Buffer Data Byte 4
	*/
	/*CANi Transmit Buffer Register pCL (TMDF3p) (i = 0) (p = 0 to 3)
		15 to 8 TMDB7[7:0] 	Transmit Buffer Data Byte 7 
		7 to 0 TMDB6[7:0] 	Transmit Buffer Data Byte 6
	*/
        p_TxMsgSfr = CAN_ADDR_TMIDLp(txbuf_idx);
        p_TxMsgSfr->IDL = ((can_frame_sfr_t *)pFrame)->IDL;
        p_TxMsgSfr->IDH = ((can_frame_sfr_t *)pFrame)->IDH;
        p_TxMsgSfr->PTR = ((can_frame_sfr_t *)pFrame)->PTR;
        p_TxMsgSfr->DF0 = ((can_frame_sfr_t *)pFrame)->DF0;
        p_TxMsgSfr->DF1 = ((can_frame_sfr_t *)pFrame)->DF1;
        p_TxMsgSfr->DF2 = ((can_frame_sfr_t *)pFrame)->DF2;
        p_TxMsgSfr->DF3 = ((can_frame_sfr_t *)pFrame)->DF3;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    	/* ---- Set transmission request ---- */
    	/*CANi Transmit Buffer Control Register p (TMCp) (i = 0) (p = 0 to 3)
    	2 TMOM 	One-Shot Transmission Enable 
		0: One-shot transmission is disabled.	---set
		1: One-shot transmission is enabled.
	1 TMTAR Transmit Abort Request 
		0: Transmit abort is not requested.	---set
		1: Transmit abort is requested.
	0 TMTR 	Transmit Request 
		0: Transmission is not requested.
		1: Transmission is requested.		---set
    	*/
    	TMCp(txbuf_idx) = BIT_ON(0);

    	return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_AbortTrm_CH0
* Description  : Abort a CAN transmission (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
* Return Value : none
******************************************************************************/
void R_CAN_AbortTrm_CH0(can_txbuf_t txbuf_idx)
{

    	/* ---- Set transmission abort request ---- */
	/*CANi Transmit Buffer Control Register p (TMCp) (i = 0) (p = 0 to 3)
    	2 TMOM 	One-Shot Transmission Enable 
		0: One-shot transmission is disabled.
		1: One-shot transmission is enabled.
	1 TMTAR Transmit Abort Request 
		0: Transmit abort is not requested.
		1: Transmit abort is requested.		---set
	0 TMTR 	Transmit Request 
		0: Transmission is not requested.
		1: Transmission is requested.		
    	*/
    	TMCp(txbuf_idx) |= BIT_ON(1);

}

/****************************************************************************** 
* Function Name: R_CAN_CheckTxBufResult_CH0
* Description  : Read the result of transmission from Tx buffer (Channel 0)
* Arguments    : txbuf_idx -
*                    Tx buffer index
* Return Value : CAN_RTN_TRANSMITTING -
*                    Transmission is in progress
*                    or no transmit request is present.
*                CAN_RTN_TX_ABORT_OVER -
*                    Transmit abort has been completed.
*                CAN_RTN_TX_END -
*                    Transmission has been completed
*                    (without transmit abort request).
*                CAN_RTN_TX_END_WITH_ABORT_REQ -
*                    Transmission has been completed
*                    (with transmit abort request).
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_CheckTxBufResult_CH0(can_txbuf_t txbuf_idx)
{
   	Can_RtnType rtn_value;

    	volatile __near uint8_t * p_TMSTSp;

	/*CANi Transmit Buffer Status Register p (TMSTSp) (i = 0) (p = 0 to 3)
	4 TMTARM 	Transmit Buffer Transmit Abort Request Status Flag
		0: No transmit abort request is present.	
		1: A transmit abort request is present.
	3 TMTRM 	Transmit Buffer Transmit Request Status Flag
		0: No transmit request is present.		
		1: A transmit request is present.
	2, 1 TMTRF[1:0] Transmit Buffer Transmit Result Flag b2 b1
		0 0 : Transmission is in progress or no transmit request is present.
								
		0 1 : Transmit abort has been completed.
		1 0 : Transmission has been completed (without transmit abort request).
		1 1 : Transmission has been completed (with transmit abort request).
	0 TMTSTS 	Transmit Buffer Transmit Status Flag 
		0: Transmission is not in progress.		
		1: Transmission is in progress.
	*/
    	p_TMSTSp = CAN_ADDR_TMSTSp(txbuf_idx);

    	rtn_value = (Can_RtnType)((*p_TMSTSp & BITS_2_ON(1)) >> 1);

    	/* ---- Tx transmission completed/abort? ---- */
    	if (rtn_value != 0)
    	{
        	/* Clear Tx buffer status */
        	*p_TMSTSp = 0x0U;
    	}

    	return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_TrmByTRFIFO0_CH0
* Description  : Transmit a frame by common (Tx/Rx) FIFO 0 (Channel 0)
* Arguments    : ch_idx -
*                    channel index
*                trfifo_idx -
*                    Tx/Rx FIFO index
*                pFrame -
*                    pointer to frame to be transmitted
* Return Value : CAN_RTN_OK -
*                    Frame is successfully pushed into FIFO.
*                CAN_RTN_FIFO_FULL -
*                    Specified FIFO is full.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_TrmByTRFIFO0_CH0(const can_frame_t * pFrame)
{

    /* ---- Return if Tx/Rx FIFO is full ---- */
    if ((CFSTS0 & CAN_TRFIFO_FULL_BIT_ON) != 0)
    {
        return CAN_RTN_FIFO_FULL;
    }

    /* ---- Send message into Tx/Rx FIFO if it is not full ---- */
    {
        uint16_t temp_rpage;

        /* ---- save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Set frame data ---- */
        CFIDL0 = ((can_frame_sfr_t *)pFrame)->IDL;
        CFIDH0 = ((can_frame_sfr_t *)pFrame)->IDH;
        CFPTR0 = ((can_frame_sfr_t *)pFrame)->PTR;
        CFDF00 = ((can_frame_sfr_t *)pFrame)->DF0;
        CFDF10 = ((can_frame_sfr_t *)pFrame)->DF1;
        CFDF20 = ((can_frame_sfr_t *)pFrame)->DF2;
        CFDF30 = ((can_frame_sfr_t *)pFrame)->DF3;

        /* ---- restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Tx/Rx FIFO buffer pointer ---- */
    CFPCTR0 = CAN_FIFO_PTR_INC;

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxBuffer
* Description  : Read message from Rx buffer
* Arguments    : p_rxbuf_idx -
*                    pointer to Rx buffer that receives frame
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_STS_ERROR -
*                    failure to clear Rx complete flag
*                CAN_RTN_OVERWRITE -
*                    A frame is overwritten.
******************************************************************************/
Can_RtnType R_CAN_ReadRxBuffer(uint8_t * p_rxbuf_idx, can_frame_t * pFrame)
{
	uint8_t  buf_idx;
	uint16_t temp_rbrcf;
	uint16_t pattern;
	volatile __near can_frame_sfr_t * p_RxBufSfr;
        uint16_t temp_rpage;
	
	
    	/* ---- Judge if new messages are available ---- */
	/*CAN Receive Buffer Receive Complete Flag Register (RMND0)
	15 to 0 RMNS[15:0] 	Receive Buffer Receive Complete Flag n
		0: Receive buffer n contains no new message (n = 0 to 15).
		1: Receive buffer n contains a new message.
		
		--In this case, write 0 to bits to be cleared and write 1 to other bits by using an 8-bit or 
		16-bit data transfer instruction
	*/
    	temp_rbrcf = RMND0;
	if (temp_rbrcf == 0)
	{
		//BIT_CLR(P12,5);
		return CAN_RTN_BUFFER_EMPTY;
	}
	else
	{
	/* ---- Get Rx buffer that has new message ---- */
	
		pattern = 1;
		for (buf_idx = 0U; buf_idx < 16U; ++buf_idx)
		{
			if ((temp_rbrcf & pattern) != 0)
			{
				*p_rxbuf_idx = buf_idx;
				break;
			}
			pattern <<= 1;
		}
	}

	/* ---- Clear Rx complete flag of corresponding Rx buffer ---- */
	RMND0 &= (uint16_t)~pattern;
	if ((RMND0 & pattern) != 0)
	{
		return CAN_RTN_STS_ERROR;
	}

    	/* ---- Read out message from Rx buffer ---- */    
        /* ---- Save value of GRWCR register ---- */
	/*CAN Global RAM Window Control Register (GRWCR)
	0 RPAGE 	RAM Window Select
		0: Selects window 0 (receive rule entry registers, CAN RAM test registers)
		1: --Selects window 1 (receive buffer, receive FIFO buffer, transmit/receive FIFO buffer, transmit buffer, 
		  transmit history data access register)
	*/
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= BIT_ON(0);

        /* ---- Read frame data ---- */
	/*CAN Receive Buffer Register nAL (RMIDLn) (n = 0 to 15)
	15 to 0 RMID[15:0] 	Receive Buffer ID Data L
		The standard ID or extended ID of received message can be read.
		Read bits 10 to 0 for standard ID. Bits 15 to 11 are read as 0.
	*/
	/*CAN Receive Buffer Register nAH (RMIDHn) (n = 0 to 15)
	15 RMIDE 	Receive Buffer IDE 
		0: Standard ID
		1: Extended ID
	14 RMRTR 	Receive Buffer RTR 
		0: Data frame
		1: Remote frame
	12 to 0 RMID[28:16] 	Receive Buffer ID Data H
		The standard ID or extended ID of received message can be read.
		For standard ID, these bits are read as 0.
	*/
	/*CAN Receive Buffer Register nBL (RMTSn) (n = 0 to 15)
	15 to 0 RMTS[15:0] 	Receive Buffer Timestamp Data
		Timestamp value of the received message can be read.
	*/
	/*CAN Receive Buffer Register nBH (RMPTRn) (n = 0 to 15)
	15 to 12 RMDLC[3:0] 	Receive Buffer DLC Data
		b15 b14 b13 b12
		0 0 0 0 : 0 data bytes
		0 0 0 1 : 1 data byte
		0 0 1 0 : 2 data bytes
		0 0 1 1 : 3 data bytes
		0 1 0 0 : 4 data bytes
		0 1 0 1 : 5 data bytes
		0 1 1 0 : 6 data bytes
		0 1 1 1 : 7 data bytes
		1 X X X : 8 data bytes
	11 to 0 RMPTR[11:0]	Receive Buffer Label Data 
		Label information of the received message can be read.
	*/
	/*CAN Receive Buffer Register nCL (RMDF0n) (n = 0 to 15)
	15 to 8 RMDB1[7:0] 	Receive Buffer Data Byte 1
	7 to 0 	RMDB0[7:0] 	Receive Buffer Data Byte 0
	*/
	/*CAN Receive Buffer Register nCL (RMDF1n) (n = 0 to 15)
	15 to 8 RMDB3[7:0] 	Receive Buffer Data Byte 3
	7 to 0 	RMDB2[7:0] 	Receive Buffer Data Byte 2
	*/
	/*CAN Receive Buffer Register nCL (RMDF2n) (n = 0 to 15)
	15 to 8 RMDB5[7:0] 	Receive Buffer Data Byte 5
	7 to 0 	RMDB4[7:0] 	Receive Buffer Data Byte 4
	*/
	/*CAN Receive Buffer Register nCL (RMDF3n) (n = 0 to 15)
	15 to 8 RMDB7[7:0] 	Receive Buffer Data Byte 7
	7 to 0 	RMDB6[7:0] 	Receive Buffer Data Byte 6
	*/
        p_RxBufSfr = CAN_ADDR_RMIDLp(*p_rxbuf_idx);
        ((can_frame_sfr_t *)pFrame)->IDL = p_RxBufSfr->IDL;
        ((can_frame_sfr_t *)pFrame)->IDH = p_RxBufSfr->IDH;
        ((can_frame_sfr_t *)pFrame)->TS  = p_RxBufSfr->TS; 
        ((can_frame_sfr_t *)pFrame)->PTR = p_RxBufSfr->PTR;
        ((can_frame_sfr_t *)pFrame)->DF0 = p_RxBufSfr->DF0;
        ((can_frame_sfr_t *)pFrame)->DF1 = p_RxBufSfr->DF1;
        ((can_frame_sfr_t *)pFrame)->DF2 = p_RxBufSfr->DF2;
        ((can_frame_sfr_t *)pFrame)->DF3 = p_RxBufSfr->DF3;

        /* ---- restore value of GRWCR register ---- */
        GRWCR = temp_rpage;

	/* ---- Judge if current message is overwritten ---- */
	if ((RMND0 & pattern) != 0)
	{
		return CAN_RTN_OVERWRITE;
	}

	return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO
* Description  : Read message from Rx FIFO
* Arguments    : rxfifo_idx -
*                    Rx FIFO index
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
extern Can_RtnType R_CAN_ReadRxFIFO0(can_frame_t *);
extern Can_RtnType R_CAN_ReadRxFIFO1(can_frame_t *);
Can_RtnType R_CAN_ReadRxFIFO(can_rxfifo_t rxfifo_idx, can_frame_t * pFrame)
{
    	if (rxfifo_idx == 0) 
    	{
        	return R_CAN_ReadRxFIFO0(pFrame);
    	} 
	else if (rxfifo_idx == 1) 
	{
        	return R_CAN_ReadRxFIFO1(pFrame);
#if defined(__CHECK__)
    	} 
	else 
	{
        	return CAN_RTN_ARG_ERROR;
#endif
    	}

    return CAN_RTN_OK;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO0
* Description  : Read message from Rx FIFO 0
* Arguments    : pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadRxFIFO0(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Rx FIFO 0 mode ---- */
    if ((g_rxfifo0_use_mode == CAN_NOUSE))
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in Rx FIFO ---- */
    temp_status = RFSTS0;
    if ((temp_status & CAN_RFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }
	BIT_CLR(P12,5);
    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if Rx FIFO has message lost ---- */
    if ((temp_status & CAN_RFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        RFSTS0 = CAN_CLR_WITHOUT_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from Rx FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = RFIDL0;
        ((can_frame_sfr_t *)pFrame)->IDH = RFIDH0;
        ((can_frame_sfr_t *)pFrame)->TS  = RFTS0; 
        ((can_frame_sfr_t *)pFrame)->PTR = RFPTR0;
        ((can_frame_sfr_t *)pFrame)->DF0 = RFDF00;
        ((can_frame_sfr_t *)pFrame)->DF1 = RFDF10;
        ((can_frame_sfr_t *)pFrame)->DF2 = RFDF20;
        ((can_frame_sfr_t *)pFrame)->DF3 = RFDF30;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Rx FIFO buffer pointer ---- */
    RFPCTR0 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadRxFIFO1
* Description  : Read message from Rx FIFO 1
* Arguments    : pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadRxFIFO1(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Rx FIFO 1 mode ---- */
    if ((g_rxfifo1_use_mode == CAN_NOUSE))
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in Rx FIFO ---- */
    temp_status = RFSTS1;
    if ((temp_status & CAN_RFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if Rx FIFO has message lost ---- */
    if ((temp_status & CAN_RFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        RFSTS1 = CAN_CLR_WITHOUT_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from Rx FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = RFIDL1;
        ((can_frame_sfr_t *)pFrame)->IDH = RFIDH1;
        ((can_frame_sfr_t *)pFrame)->TS  = RFTS1; 
        ((can_frame_sfr_t *)pFrame)->PTR = RFPTR1;
        ((can_frame_sfr_t *)pFrame)->DF0 = RFDF01;
        ((can_frame_sfr_t *)pFrame)->DF1 = RFDF11;
        ((can_frame_sfr_t *)pFrame)->DF2 = RFDF21;
        ((can_frame_sfr_t *)pFrame)->DF3 = RFDF31;

        /* ---- Restore value of GRWCR register ---- */
        GRWCR = temp_rpage;
    }

    /* ---- Increment Rx FIFO buffer pointer ---- */
    RFPCTR1 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadTRFIFO
* Description  : Read message from common (Tx/Rx) FIFO
* Arguments    : ch_idx -
*                    channel index
*                trfifo_idx -
*                    common (Tx/Rx) FIFO index
*                pFrame -
*                    pointer to stored frame position
* Return Value : CAN_RTN_OK -
*                    A frame is successfully read out.
*                CAN_RTN_OK_WITH_LOST -
*                    A frame is successfully read out (with message lost).
*                CAN_RTN_BUFFER_EMPTY -
*                    No frame is read out.
*                CAN_RTN_ARG_ERROR -
*                    invalid argument specification
******************************************************************************/
Can_RtnType R_CAN_ReadTRFIFO0_CH0(can_frame_t * pFrame)
{
    uint16_t    temp_status;
    Can_RtnType rtn_value;

#if defined(__CHECK__)
    /* ----  Check Tx/Rx FIFO 0 mode ---- */
    if (g_trfifo_use_mode != CAN_USE_RX_MODE)
    {
        return CAN_RTN_ARG_ERROR;
    }
#endif

    /* ----  Check if any unread message is available in common (Tx/Rx) FIFO ---- */
    temp_status = CFSTS0;
    if ((temp_status & CAN_TRFIFO_EMPTY_BIT_ON) != 0)
    {
        return CAN_RTN_BUFFER_EMPTY;
    }

    /* ----  Set return value ---- */
    rtn_value = CAN_RTN_OK;

    /* ---- Check if common (Tx/Rx) FIFO has message lost ---- */
    if ((temp_status & CAN_TRFIFO_MSGLST_BIT_ON) != 0)
    {
        /* ---- Clear message lost flag ---- */
        CFSTS0 = CAN_CLR_WITHOUT_TX_RX_INT;

        /* ----  Set return value ---- */
        rtn_value = CAN_RTN_OK_WITH_LOST;
    }

    /* ---- Read out message from common (Tx/Rx) FIFO ---- */
    {
        uint16_t temp_rpage;

        /* ---- Save value of GRWCR register ---- */
        temp_rpage = GRWCR;

        /* ---- Select window 1 ---- */
        GRWCR |= CAN_RAM_WINDOW_BIT_ON;

        /* ---- Read frame data ---- */
        ((can_frame_sfr_t *)pFrame)->IDL = CFIDL0;
        ((can_frame_sfr_t *)pFrame)->IDH = CFIDH0;
        ((can_frame_sfr_t *)pFrame)->TS  = CFTS0;
        ((can_frame_sfr_t *)pFrame)->PTR = CFPTR0;
        ((can_frame_sfr_t *)pFrame)->DF0 = CFDF00;
        ((can_frame_sfr_t *)pFrame)->DF1 = CFDF10;
        ((can_frame_sfr_t *)pFrame)->DF2 = CFDF20;
        ((can_frame_sfr_t *)pFrame)->DF3 = CFDF30;

        /* ---- Restore value of GRWCR register ---- */
	//BIT_SET(P12,5);//MZGK
        GRWCR = temp_rpage;
    }

    /* ---- Increment common (Tx/Rx) FIFO buffer pointer ---- */
    CFPCTR0 = CAN_FIFO_PTR_INC;

    return rtn_value;
}

/****************************************************************************** 
* Function Name: R_CAN_ReadChStatus
* Description  : Read channel status
* Arguments    : none
* Return Value : channel status (<= 0xFF) -
*                    normal completion
******************************************************************************/
Can_RtnType R_CAN_ReadChStatus_CH0(void)
{
	/*CANi Status Register L (CiSTSL) (i = 0)
	After Reset :0x05
	7 COMSTS 	Communication Status Flag 
		0: Communication is not ready.
		1: Communication is ready.
	6 RECSTS Receive Status Flag 
		0: Bus idle, in transmission or bus off state
		1: In reception
	5 TRMSTS 	Transmit Status Flag 
		0: Bus idle or in reception
		1: In transmission or bus off state
	4 BOSTS 	Bus Off Status Flag 0: Not in bus off state
		1: In bus off state
	3 EPSTS 	Error Passive Status Flag 
		0: Not in error passive state
		1: In error passive state
	2 CSLPSTS 	Channel Stop Status Flag 
		0: Not in channel stop mode
		1: In channel stop mode
	1 CHLTSTS 	Channel Halt Status Flag 
		0: Not in channel halt mode
		1: In channel halt mode
	0 CRSTSTS 	Channel Reset Status Flag 
		0: Not in channel reset mode
		1: In channel reset mode
	*/
    	return (Can_RtnType)(C0STSL & 0x00FFU);
}

/*****************************************************************************
	INTCAN0TRM 	CAN0 channel transmit 	0054H
*****************************************************************************/
//#pragma interrupt R_CAN0_transmit_interrupt(vect=0x0054)
//static void __near R_CAN0_transmit_interrupt(void)
__interrupt static void R_CAN0_transmit_interrupt(void)
{
	/*clear INTWDTI interrupt flag
	21.3.1 Interrupt request flag registers (IF0L, IF0H, IF1L, IF1H, IF2L, IF2H, IF3L)
	IFxx 	Interrupt request flag
	0 	No interrupt request signal is generated		---set
	1 	Interrupt request is generated, interrupt request status
	*/
	CAN0TRMIF = 0U; 
}

/**************************************************************************
	INTCANGERR 	CAN global error 	0058H
**************************************************************************/
//#pragma interrupt R_CAN_Global_error_interrupt(vect=0x0058)
//static void __near R_CAN_Global_error_interrupt(void)
__interrupt  static  void  R_CAN_Global_error_interrupt(void)
{
	CANGERRIF = 0U;
}

/************************************************************************
INTCAN0ERR 	CAN0 channel error Internal 	004EH
************************************************************************/
//#pragma interrupt R_CAN0_Global_channel_interrupt(vect=0x004E)
//static void __near R_CAN0_Global_channel_interrupt(void)
__interrupt  static  void  R_CAN0_Global_channel_interrupt(void)
{
	CAN0ERRIF = 0U;
	/*CANi Error Flag Register L (CiERFLL) (i = 0)
	14 ADERR 	ACK Delimiter Error Flag 
		0: No ACK delimiter error is detected.	---
		1: ACK delimiter error is detected.
	13 B0ERR 	Dominant Bit Error Flag 
		0: No dominant bit error is detected.	---
		1: Dominant bit error is detected.
	12 B1ERR 	Recessive Bit Error Flag 
		0: No recessive bit error is detected.	---
		1: Recessive bit error is detected.
	11 CERR 	CRC Error Flag 
		0: No CRC error is detected.		---
		1: CRC error is detected.
	10 AERR 	ACK Error Flag 
		0: No ACK error is detected.		---
		1: ACK error is detected.
	9 FERR 		Form Error Flag 
		0: No form error is detected.		---
		1: Form error is detected.
	8 SERR 	Stuff Error Flag 
		0: No stuff error is detected.		---
		1: Stuff error is detected.
	7 ALF 	Arbitration Lost Flag 
		0: No arbitration lost is detected.
		1: Arbitration lost is detected.	---check
	6 BLF	Bus Lock Flag 
		0: No channel bus lock is detected.	---
		1: Channel bus lock is detected.
	5 OVLF 	Overload Flag 
		0: No overload is detected.		---
		1: Overload is detected.
	4 BORF 	Bus Off Recovery Flag 
		0: No bus off recovery is detected.	---
		1: Bus off recovery is detected.
	3 BOEF 	Bus Off Entry Flag 
		0: No bus off entry is detected.	---
		1: Bus off entry is detected.
	2 EPF 	Error Passive Flag 
		0: No error passive is detected.	---
		1: Error passive is detected.
	1 EWF 	Error Warning Flag 
		0: No error warning is detected.	---
		1: Error warning is detected.
	0 BEF 	Bus Error Flag 
		0: No channel bus error is detected.	---
		1: Channel bus error is detected.
	*/
	C0ERFLL=0x0000;
	C0ERFLL=0x0000;
	C0ERFLL=0x0000;
}
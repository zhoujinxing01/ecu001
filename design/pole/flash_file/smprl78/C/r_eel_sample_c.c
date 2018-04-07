/*******************************************************************************
* DISCLAIMER                                                                   *
* This software is supplied by Renesas Electronics Corporation and is only     *
* intended for use with Renesas products. No other uses are authorized. This   *
* software is owned by Renesas Electronics Corporation and is protected under  *
* all applicable laws, including copyright laws.                               *
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  *
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      *
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   *
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          *
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       *
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  *
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   *
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  *
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             *
* Renesas reserves the right, without notice, to make changes to this software *
* and to discontinue the availability of this software. By using this software,*
* you agree to the additional terms and conditions found by accessing the      *
* following link:                                                              *
* http://www.renesas.com/disclaimer                                            *
*                                                                              *
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.     *
*******************************************************************************/
/*******************************************************************************
* File Name     : r_eel_sample_c.c                                                   *
* Version       : 1.00                                                         *
* Device(s)     : RL78/G13   ( R5F100LEA )                                     *
* Tool-Chain    : CubeSuite+ ( V2.00 )                                         *
* OS            : none                                                         *
* H/W Platform  : QB-R5F100LE-TB                                               *
* Description   : This is the EEL sample code.                                 *
* Operation     :                                                              *
* Limitations   :                                                              *
*******************************************************************************/
/*******************************************************************************
* History       : Version Description                                          *
*               : 05.31.2013 1.00 First Releases                               *
*******************************************************************************/

/******************************************************************************
 Expanded function
******************************************************************************/
#pragma sfr                          /* declare sfr area                     */
#pragma DI                           /* declare disable interrupt function   */
#pragma EI                           /* declare enable interrupt function    */
#pragma NOP                          /* declare CPU control instruction(NOP) */


/******************************************************************************
 Interrupt function
******************************************************************************/
//#pragma interrupt INTWDTI samIntWdt    /* Definition of WDT interrupt vector */

/******************************************************************************
 Include <System Includes>, "Project Includes"
******************************************************************************/
/* Header file for standard Library */
#include <string.h>             

/* Header files for FDL/EEL */
#include "fdl.h"                    /* FDL library header file               */
#include "fdl_types.h"              /* FDL types definition header file      */
#include "fdl_descriptor.h"         /* FDL descriptor header file            */
#include "eel.h"                    /* EEL library header file               */
#include "eel_types.h"              /* EEL types definition header file      */
#include "eel_descriptor.h"         /* EEL descriptor header file            */
#include "eel_user_types.h"         /* EEL user types definition header file */


/******************************************************************************
 Macro definitions
******************************************************************************/
#define SAM_WDT_RESET() WDTE = 0xAC /* Declare watchdog timer reset function */
#define SAM_WRITE_SIZE 10           /* Declare write data size */


extern volatile  uint8_t  	EEPROM_WR_buf[8];
extern volatile  uint8_t  	EEPROM_RD_buf[8];

/*==============================================================================*/
/* Function name : main                                                         */
/* IN            : void                                                         */
/* OUT           : void                                                         */
/* Description   : Main function                                                */
/* Date          : 05,31,2013                                                   */
/*==============================================================================*/
//void main( void )

void EEPROM_Manipulate(
		eel_u08 EEPROM_CMD,	
		eel_u08 EEPROM_ID,	
		eel_u08 * EEPROM_WRbuf,	
		eel_u08 * EEPROM_RDbuf	
		)
{
    /* Definition of variables */
    eel_request_t   dtyEelReq;
    //eel_u08         dubWriteBuffer[ SAM_WRITE_SIZE ] = {0,1,2,3,4,5,6,7,8,9};
    //eel_u08         dubReadBuffer[ SAM_WRITE_SIZE ];
    fdl_status_t    fdlStatus = 0; 
    eel_u08         err_flag = 0;

    /* Reset WDT */
  //  SAM_WDT_RESET();
    
    /* Enable interrupt */
  //  EI();
    
    /* Initialize EEL requester */
    dtyEelReq.address_pu08   = 0;
    dtyEelReq.identifier_u08 = 0;
    dtyEelReq.command_enu    = 0;
    dtyEelReq.status_enu     = 0;
    
    /*== EEL / uninitialized =*/
    /***************************************************************************/
    /* FDL initialization processing                                           */
    /***************************************************************************/
    /* Initialize FDL parameters */
    fdlStatus = FDL_Init( &fdl_descriptor_str );
    
    if( fdlStatus == FDL_OK )
    {
        /*== EEL / uninitialized =*/
        /***************************************************************************/
        /* FDL preparation processing                                              */
        /***************************************************************************/
        FDL_Open();
       
        /*== -> EEL / uninitialized -> ( Change EEL state ) ========*/
        /*******************************************************************************/
        /* EEL initialization processing( state : EEL / uninitialized -> EEL / closed )*/
        /*******************************************************************************/
        dtyEelReq.status_enu = EEL_Init();
        
        if( dtyEelReq.status_enu == EEL_OK )
        {
            /*== EEL / closed -> ( Change EEL state ) ===============*/
            /***************************************************************************/
            /* EEL preparation processing ( state : EEL / closed -> EEL / opened )     */
            /***************************************************************************/
            EEL_Open();
            
            /*== ->  EEL / opened -> ( Change EEL state ) ===============*/
            /***************************************************************************/
            /* EEL execution start processing ( state : EEL / opened -> EEL / started )*/
            /***************************************************************************/
            do {
                /* Set parameter of EEL_CMD_STARTUP command   */
                dtyEelReq.command_enu = EEL_CMD_STARTUP;
                
                /* Execute EEL_CMD_STARTUP command */
                EEL_Execute( &dtyEelReq ); 
                
                /* Wait of command processing end */
                while( dtyEelReq.status_enu == EEL_BUSY )
                {
                     EEL_Handler(); /* Check of command end */
                }
                    
                /* EEPROM emulation block inconsistency error */
                if( dtyEelReq.status_enu == EEL_ERR_POOL_INCONSISTENT )
                {
                    /* Set parameter of EEL_CMD_FORMAT command   */
                    dtyEelReq.command_enu = EEL_CMD_FORMAT;
                    
                    /* Execute EEL_CMD_FORMAT command */
                    EEL_Execute( &dtyEelReq );
                    
                    /* Wait of command processing end */
                    while( dtyEelReq.status_enu == EEL_BUSY )
                    {
                        EEL_Handler();  /* Check of command end */
                    }
                }
            /* If EEL_CMD_FORMAT command is successful,       */
            /* re-execute EEL_CMD_STARTUP command.            */
            } while( ( dtyEelReq.command_enu == EEL_CMD_FORMAT ) &&
                     ( dtyEelReq.status_enu  == EEL_OK         )  );
            
            /* EEL_CMD_FORMAT command or EEL_CMD_STARTUP command is successful. */
            /* If command is abnormal end,  execute EEL_CMD_SHUTDOWN command.   */
            
            if( dtyEelReq.status_enu == EEL_OK )
	    
            {
		if((EEPROM_CMD == EEPROM_WR)||(EEPROM_CMD == EEPROM_WR_RD))
		{
                do {
                    /**************************/
                    /* data write processing  */
                    /**************************/
		    dtyEelReq.address_pu08   = EEPROM_WRbuf;
                    dtyEelReq.identifier_u08 = EEPROM_ID; 
                    dtyEelReq.command_enu    = EEL_CMD_WRITE;
                    
                    /* Execute EEL_CMD_WRITE command */
                    EEL_Execute( &dtyEelReq );
                    
                    /* Wait of command processing end */
                    while( dtyEelReq.status_enu == EEL_BUSY )
                    {
                        /* Check of command end */
                        EEL_Handler();
                    }
                    
                     /* Pool full error */
                    if (dtyEelReq.status_enu == EEL_ERR_POOL_FULL)
                    {
                       /* Set parameter of EEL_CMD_REFRESH command */
                        dtyEelReq.command_enu    = EEL_CMD_REFRESH;
                        
                        /* Execute EEL_CMD_REFRESH command */
                        EEL_Execute( &dtyEelReq );
                        
                        /* Wait of command processing end */
                        while( dtyEelReq.status_enu == EEL_BUSY )
                        {
                            /* Check of command end */
                            EEL_Handler();
                        }
                    }
                /* If EEL_CMD_REFRESH command is  successful, */
                /* re-execute EEL_CMD_WRITE command..         */
                } while( ( dtyEelReq.command_enu == EEL_CMD_REFRESH ) &&
                         ( dtyEelReq.status_enu  == EEL_OK          )  );
                
		    }
                /* EEL_CMD_WRITE command or EEL_CMD_REFRESH command is successful. */
                /* If command is abnormal end, execute EEL_CMD_SHUTDOWN command.   */
		if((EEPROM_CMD == EEPROM_RD)||(EEPROM_CMD == EEPROM_WR_RD))
		{
                if ( dtyEelReq.status_enu == EEL_OK )   
                {
                    /************************/
                    /* data read processing */
                    /************************/
		    dtyEelReq.address_pu08   = EEPROM_RDbuf;
		    dtyEelReq.identifier_u08 = EEPROM_ID; //6 bytes
                    dtyEelReq.command_enu    = EEL_CMD_READ;
                    
                    /* Execute EEL_CMD_READ command */
                    EEL_Execute( &dtyEelReq );
                    
                    /* Wait of command processing end */
                    while( dtyEelReq.status_enu == EEL_BUSY )
                    {
                        /* Check of command end */
                        EEL_Handler();
                    }
		    
                }
		}
                /*== ->  EEL / started -> ( Change EEL state ) ==========*/
                /***************************************************************************/
                /* EEL Execution stop processing ( state : EEL / started -> opened )       */
                /***************************************************************************/
                /* Set parameter of EEL_CMD_SHUTDOWN command */
                dtyEelReq.command_enu = EEL_CMD_SHUTDOWN;
                
                /* Execute EEL_CMD_SHUTDOWN */
                EEL_Execute( &dtyEelReq );
                
                /* Wait of command processing end */
                while( dtyEelReq.status_enu == EEL_BUSY )
                {
                    /* Check of command end */
                    EEL_Handler();
                }
            }
            /*== ->  EEL / opened -> ( Change EEL state ) ===============*/
            /***************************************************************************/
            /* EEL end processing ( state : EEL / opened -> closed )                   */
            /***************************************************************************/
            EEL_Close();
        }
        /*== ->  EEL / closed -> ( Change EEL state )  ===============*/
        /***************************************************************************/
        /* FDL end processing  ( state : EEL / closed -> uninitialized )            */
        /***************************************************************************/
        FDL_Close();    
     }
    /*== -> EEL / uninitialized   ====================================*/
    
    /*== end loop ==*/
}






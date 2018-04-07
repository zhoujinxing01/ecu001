;*******************************************************************************
; Library       : EEPROM Emulation Library (T02)
;
; File Name     : $Source: eel.inc $
; Lib. Version  : $RL78_EEL_LIB_VERSION_T02: V1.00 $
; Mod. Revision : $Revision: 1.11 $
; Mod. Date     : $Date: 2012/12/04 16:09:40JST $
; Device(s)     : RL78
; Description   : API definition of the EEPROM Emulation Library T02 (EEL)
;******************************************************************************
; DISCLAIMER
; This software is supplied by Renesas Electronics Corporation and is only
; intended for use with Renesas products. No other uses are authorized. This
; software is owned by Renesas Electronics Corporation and is protected under
; all applicable laws, including copyright laws.
; THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
; THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
; LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
; AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
; TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
; ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
; FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
; ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
; BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; Renesas reserves the right, without notice, to make changes to this software
; and to discontinue the availability of this software. By using this software,
; you agree to the additional terms and conditions found by accessing the
; following link:
; http://www.renesas.com/disclaimer
;
; Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
;*******************************************************************************



$IF (__EEL_INC_INCLUDED)

$ELSE
$SET (__EEL_INC_INCLUDED)


;*******************************************************************************
; Function name:    EEL_Init
;*******************************************************************************
;
; Initialization of the EEPROM Emulation Library (EEL).
;
; @destructed_reg   none
; @return           C - Initialization status (see type eel_status_t)
;*******************************************************************************
EXTRN EEL_Init



;*******************************************************************************
; Function name:    EEL_Open
;*******************************************************************************
;
; Activate the EEL environment.
;
; @destructed_reg   none
;*******************************************************************************
EXTRN EEL_Open



;*******************************************************************************
; Function name:    EEL_Close
;*******************************************************************************
;
; Deactivate the EEL environment.
;
; @destructed_reg   none
;*******************************************************************************
EXTRN EEL_Close



;*******************************************************************************
; Function name:    EEL_Execute
;*******************************************************************************
;
; Starting of execution of the corresponding EEL request.
;
; @param[in,out]    AX - Pointer to the EEL request variable
; @destructed_reg   none
;*******************************************************************************
EXTRN EEL_Execute



;*******************************************************************************
; Function name:    EEL_Handler
;*******************************************************************************
;
; This function takes care for the execution of the running command
;
; @destructed_reg   none
;*******************************************************************************
EXTRN EEL_Handler



;*******************************************************************************
; Function name:    EEL_GetDriverStatus
;*******************************************************************************
;
; This function provides the internal status of the EEL.
;
; @destructed_reg   AX
;*******************************************************************************
EXTRN EEL_GetDriverStatus



;*******************************************************************************
; Function name:    EEL_GetSpace
;*******************************************************************************
;
; Provides the currently available space inside the active block.
;
; @param[in]        AX - address of the space variable
; @destructed_reg   AX
; @return           C  - Return status of the function.
;*******************************************************************************
EXTRN EEL_GetSpace



;*******************************************************************************
; Function name:    EEL_GetVersionString
;*******************************************************************************
;
; This function returns the pointer to the "zero-terminated" version string.
;
; @destructed_reg   none
; @return           BC = 16bit loww address of the pointer
;                   DE = 16bit highw address of the pointer
;*******************************************************************************
EXTRN EEL_GetVersionString



$ENDIF

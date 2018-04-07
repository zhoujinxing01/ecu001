;*******************************************************************************
; Library       : EEPROM Emulation Library (T02)
;
; File Name     : $Source: eel_types.inc $
; Lib. Version  : $RL78_EEL_LIB_VERSION_T02: V1.00 $
; Mod. Revision : $Revision: 1.11 $
; Mod. Date     : $Date: 2012/10/26 01:00:00JST $
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


$IF (__EEL_TYPES_INC_INCLUDED)

$ELSE
$SET (__EEL_TYPES_INC_INCLUDED)


; EEL command code definitions
; -----------------------------------------------------
EEL_CMD_UNDEFINED                 EQU     (00H)
EEL_CMD_STARTUP                   EQU     (000H OR 001H)
EEL_CMD_WRITE                     EQU     (000H OR 002H)
EEL_CMD_READ                      EQU     (000H OR 003H)
EEL_CMD_REFRESH                   EQU     (000H OR 004H)
EEL_CMD_VERIFY                    EQU     (000H OR 005H)
EEL_CMD_FORMAT                    EQU     (000H OR 006H)
EEL_CMD_SHUTDOWN                  EQU     (000H OR 007H)


; EEL driver operation status code definition
; -----------------------------------------------------
EEL_DRIVER_PASSIVE                EQU     (000H OR 000H)
EEL_DRIVER_IDLE                   EQU     (030H OR 001H)
EEL_DRIVER_BUSY                   EQU     (030H OR 002H)


; EEL error code definitions
; -----------------------------------------------------
EEL_OK                            EQU     (000H OR 000H)
EEL_BUSY                          EQU     (000H OR 001H)
EEL_ERR_CONFIGURATION             EQU     (080H OR 002H)
EEL_ERR_INITIALIZATION            EQU     (080H OR 003H)
EEL_ERR_ACCESS_LOCKED             EQU     (080H OR 004H)
EEL_ERR_PARAMETER                 EQU     (080H OR 005H)
EEL_ERR_VERIFY                    EQU     (080H OR 006H)
EEL_ERR_REJECTED                  EQU     (080H OR 007H)
EEL_ERR_NO_INSTANCE               EQU     (080H OR 008H)
EEL_ERR_POOL_FULL                 EQU     (080H OR 009H)
EEL_ERR_POOL_INCONSISTENT         EQU     (080H OR 00AH)
EEL_ERR_POOL_EXHAUSTED            EQU     (080H OR 00BH)
EEL_ERR_INTERNAL                  EQU     (080H OR 00CH)


; byte index within EEL-request structure
; -----------------------------------------------------
EEL_REQUEST_ADDR                  EQU     (000H)
EEL_REQUEST_IDENTIFIER            EQU     (002H)
EEL_REQUEST_COMMAND               EQU     (003H)
EEL_REQUEST_ERROR                 EQU     (004H)


$ENDIF

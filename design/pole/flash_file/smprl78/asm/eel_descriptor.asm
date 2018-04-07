;*******************************************************************************
; Library       : EEPROM Emulation Library (T02)
;
; File Name     : $Source: eel_descriptor.asm $
; Lib. Version  : $RL78_EEL_LIB_VERSION_T02: V1.00 $
; Mod. Revision : $Revision: 1.11 $
; Mod. Date     : $Date: 2012/10/26 01:00:03JST $
; Device(s)     : RL78
; Description   : Contains the user defined EEL-variable descriptor
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

 NAME           EEL_DESCRIPTOR


$include    (eel_descriptor.inc)


;*******************************************************************************
; Export list
;*******************************************************************************
 PUBLIC     _eel_descriptor
 PUBLIC     _eel_internal_cfg_cau08


;*******************************************************************************
; Segment name:  EEL_CNST
; Segment type:  CONST
;*******************************************************************************
EEL_CNST    CSEG    PAGE64KP


;*******************************************************************************
; Size definition of variables
;*******************************************************************************
  VAR_SIZE_A              EQU               2
  VAR_SIZE_B              EQU               3
  VAR_SIZE_C              EQU               4
  VAR_SIZE_D              EQU               5
  VAR_SIZE_E              EQU               6
  VAR_SIZE_F              EQU               10
  VAR_SIZE_X              EQU               20
  VAR_SIZE_Z              EQU               255



;*******************************************************************************
; BEGIN OF CUSTOMIZABLE DECLARATION AREA
;*******************************************************************************
_eel_descriptor:
  DB  EEL_VAR_NO  ; variable count
  DB  VAR_SIZE_A  ; id=1
  DB  VAR_SIZE_B  ; id=2
  DB  VAR_SIZE_C  ; id=3
  DB  VAR_SIZE_D  ; id=4
  DB  VAR_SIZE_E  ; id=5
  DB  VAR_SIZE_F  ; id=6
  DB  VAR_SIZE_X  ; id=7
  DB  VAR_SIZE_Z  ; id=8
  DB  00H         ; zero terminator
;*******************************************************************************
; END OF CUSTOMIZABLE DECLARATION AREA
;*******************************************************************************



;*******************************************************************************
; BEGIN OF UNTOUCHABLE DECLARATION AREA
;*******************************************************************************
_eel_internal_cfg_cau08:
  DB  040H
;*******************************************************************************
; END OF UNTOUCHABLE DECLARATION AREA
;*******************************************************************************


  END



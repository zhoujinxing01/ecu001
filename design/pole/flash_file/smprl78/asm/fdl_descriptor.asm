;*******************************************************************************
; Library       : Data Flash Access Library T02
;
; File Name     : $Source: fdl_descriptor.asm $
; Lib. Version  : $RL78_FDL_LIB_VERSION_T02_REN: V1.01 $
; Mod. Revision : $Revision: 1.15 $
; Mod. Date     : $Date: 2013/06/25 22:44:34JST $
; Device(s)     : RL78
; Description   : Physical allocation of the FDL descriptor.
;                 Please do NOT change this file !!!!
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
; Copyright (C) 2011-2013 Renesas Electronics Corporation. All rights reserved.
;*******************************************************************************


 NAME           fdl_descriptor                        ; module name


                                                      ;===========================================
                                                      ; INCLUDE LIST
                                                      ;===========================================
$include    (fdl_descriptor.inc)
                                                      ;===========================================
                                                      ; EXPORT LIST
                                                      ;===========================================
 PUBLIC     fdl_descriptor_str                        ; FDL descriptor
 PUBLIC     _fdl_descriptor_str                       ;



;=================================================================================================
; Segment name:  FDL_CNST
; Segment type:  CONST, location of the descriptor
;=================================================================================================
FDL_CNST  CSEG  PAGE64KP
;=================================================================================================


; -----------------------------------------------------------------------------------------------------
; ------                                                                                          -----
; ------     B E G I N    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A       -----
; ------                                                                                          -----
; -----------------------------------------------------------------------------------------------------


; specify the flash block size expressed in bytes
DATA_FLASH_BLOCK_SIZE       EQU               (1*1024)


; calculate the EEL-pool size expressed in bytes
$_IF ( EEL_POOL_BLOCKS > 0 )
  EEL_POOL_BYTES            EQU               (DATA_FLASH_BLOCK_SIZE*EEL_POOL_BLOCKS)
$ELSE
  EEL_POOL_BYTES            EQU               0
$ENDIF


; calculate the FDL-pool size expressed in bytes
$_IF ( FDL_POOL_BLOCKS > 0 )
  FDL_POOL_BYTES            EQU               (DATA_FLASH_BLOCK_SIZE * FDL_POOL_BLOCKS)
$ELSE
  FDL_POOL_BYTES            EQU               0
$ENDIF


; calculate f_MHz = round-up(FDL_SYSTEM_FREQUENCY)
FDL_FX_MHZ                  EQU               ((FDL_SYSTEM_FREQUENCY+999999)/1000000)

; calculate 10us delay as a function f(fx)
FDL_DELAY                   EQU               ((10*FDL_FX_MHZ)/6)

$IF ( FDL_WIDE_VOLTAGE_MODE )
  FDL_VOLTAGE_MODE          EQU               1
$ELSE
  FDL_VOLTAGE_MODE          EQU               0
$ENDIF

;-------------------------------------------------------------------------------------------------
; descriptor
;-------------------------------------------------------------------------------------------------
fdl_descriptor_str:
_fdl_descriptor_str:
  DW  LOWW(EEL_POOL_BYTES)
  DW  LOWW(FDL_POOL_BYTES)
  DW  LOWW(FDL_DELAY)
  DB  EEL_POOL_BLOCKS
  DB  FDL_POOL_BLOCKS
  DB  FDL_FX_MHZ
  DB  FDL_VOLTAGE_MODE

; -----------------------------------------------------------------------------------------------------
; ------                                                                                          -----
; ------     E N D    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A           -----
; ------                                                                                          -----
; -----------------------------------------------------------------------------------------------------

  END



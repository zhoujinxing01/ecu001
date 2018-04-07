;*******************************************************************************
; Library       : Data Flash Access Library T02
;
; File Name     : $Source: fdl.inc $
; Lib. Version  : $RL78_FDL_LIB_VERSION_T02_REN: V1.01 $
; Mod. Revision : $Revision: 1.18 $
; Mod. Date     : $Date: 2013/06/25 22:44:36JST $
; Device(s)     : RL78
; Description   : Assembler API definition of the Flash Access Library
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



$IF (__FDL_INC_INCLUDED)

$ELSE
$SET (__FDL_INC_INCLUDED)




;==============================================================================================
; constant definitions
;==============================================================================================


; FDL command code definitions
; -----------------------------------------------------
FDL_CMD_READ_BYTES              EQU     (00H)
FDL_CMD_IVERIFY_BYTES           EQU     (01H)
FDL_CMD_BLANKCHECK_BYTES        EQU     (02H)
FDL_CMD_WRITE_BYTES             EQU     (03H)
FDL_CMD_ERASE_BLOCK             EQU     (04H)


; status code definitions returned by the FDL functions

; operation related status code
FDL_OK                  EQU       000H
FDL_BUSY                EQU       0FFH
FDL_ERR_INITIALIZATION  EQU       002H
FDL_ERR_REJECTED        EQU       003H
FDL_ERR_ABORTED         EQU       004H
FDL_ERR_PARAMETER       EQU       005H
FDL_ERR_STANDBY         EQU       006H
; flash access related status code
FDL_ERR_ERASE           EQU       01AH
FDL_ERR_BLANK_VERIFY    EQU       01BH
FDL_ERR_WRITE           EQU       01CH
; configuration related status code
FDL_ERR_CONFIGURATION   EQU       001H




; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;   Purpose:      Initialization of the Flash Data Library.
;                 Several checks are performed during the initialization:
;                 - plausibility check of the pool configuration
;                 - frequency parameter check against supported device-specific range
;                 - initialization of all internal variables
;                 - initialization of the flash firmware
;                 - configuration of HOCO
;                 After initialization the FDL remains passive.
;                 FDL_Open() has to be executed to open access to the FDL pool.
;  Input:         AX = LOWW(descriptor)
;                 BC = HIGHW(descriptor)
;  Changed:       AX, BC
;  Return:        C  = status: configuration related error code
;                 - FDL_OK:                 initialization done without problems
;                 - FDL_ERR_CONFIGURATION:  FDL pool-configuration error or HOCO isn't started
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Init




; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Enable data flash access.
;                Pre-condition: The FDL initialization must be done before however no check is performed here.
;                If FDL is not yet initialized, FDL_Open() has no functionality.
;  Input:        -
;  Output:       -
;  Changed:      -
;  Return:       -
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Open


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Disable data flash access.
;                Pre-condition: The initialization of the library must "opened" by the function FDL_Open.
;                If FDL is not yet "opened", the FDL_Close() has no functionality.
;                If any firmware command is just executed, FDL_Close will stop it by FW command first.
;                After that the flash clock is switched OFF.
;  Input:        -
;  Output:       -
;  Changed:      -
;  Return:       -
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Close


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Initiating execution of the FDL request related to the FDL pool part only.
;                The corresponding request variable has to be parameterized before.
;
;                my_request->index_u16        :   byte-index or block-number within FDL-pool
;                my_request->data_pu08        :   start address of the RAM data that should be read/written
;                my_request->bytecount_u16    :   number of bytes has to be passed in case of byte
;                                                 oriented commands
;                my_request->command_enu      :   command code
;                my_request->status_enu       :   request status
;
;  Input:        AX - pointer to request variable
;  Changed:      AX
;  Output:       status inside the request variable
;  Return:       -
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Execute



; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Continuation and status update of requests being under execution.
;                When no FDL request is being executed, this function has no functionality (idle).
;  Input:        -
;  Output:       status inside the request variable for command executed via FDL_Execute
;  Changed:      -
;  Return:       -
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Handler

; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      This function aborts immediately a potentially running block command (ERASE) independent
;                of the affected pool.
;  Input:        -
;  Output:       -
;  Changed:      C
;  Return:       C  = status:
;                - FDL_OK:                Either no command is running or erase has been aborted.
;                - FDL_BUSY:              Byte oriented command is still running.
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_Abort


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      This function stops the clock of the data flash and enters the library into StandBy mode.
;                Note :  It is not allowed to call any FDL function other than FDL_WakeUp when FDL is in
;                        StandBy mode.
;  Input:        -
;  Output:       -
;  Changed:      C
;  Return:       C  = status:
;                - FDL_OK:                FDL entered StandBy mode.
;                - FDL_BUSY:              Any command is still running.
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_StandBy

; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Continuation and status update of requests being under execution.
;                When no FDL request is being executed, this function has no functionality (idle).
;  Input:        -
;  Output:       -
;  Changed:      C
;  Return:       C  = status:
;                - FDL_OK:                FDL is up and running.
;                - FDL_ERR_REJECTED:      WakeUp cannot be performed.
;                                         REASON: Library isn't in StandBy mode.
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_WakeUp


; ---------------------------------------------------------------------------------------------------------------
;  Block type:   standard function
; ---------------------------------------------------------------------------------------------------------------
;  Purpose:      Return the start address of the library version string
;
;  Input:        -
;  Output:       -
;  Changed:      DE, BC
;  Return:       starting pointer to the zero-terminated version string
;                DE - upper address of string
;                BC - lower address of string
; ---------------------------------------------------------------------------------------------------------------
EXTRN FDL_GetVersionString




$ENDIF

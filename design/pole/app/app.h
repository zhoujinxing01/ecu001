#ifndef __PoleAPP__
#define __PoleAPP__

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"

#include "iodefine.h"
#include "r_rl78_can_drv.h"

#include "eel_user_types.h"         
#include "r_cg_userdefine.h"

#include "Pole_EEPROM.h"
#include "config.h"


#define SW_OFF	0
#define SW_ON	1

#define Tick_PWR_ms	200
#define Tick_CAN_ms	5
#define Tick_ADC_ms	11
#define Tick_HOL_ms	10
#define Tick_BUZ_ms	150
//#define Tick_DIN_ms	6
#define Tick_DIN_ms	3
#define Tick_ECO_ms	1000

#define	DOOR_STATUS_Open	0
#define	DOOR_STATUS_Opening	1
#define	DOOR_STATUS_Close	2
#define	DOOR_STATUS_Closing	3

extern volatile  uint8_t  	LOCK_STATUS;
extern volatile  uint8_t  	LOCK_STEP;
extern volatile  uint16_t  	LOCK_Counter;
extern volatile  uint8_t  	DOOR_STATUS;
extern volatile  uint16_t  	HollCommandPosition;
extern volatile  uint8_t  	HollCommandMode;
extern volatile  uint8_t  	Buzzer_Mode;
extern volatile  uint8_t  	System_ECO_TIM;
extern volatile  uint8_t  	CanLoopBack;


extern volatile  	uint8_t  	dddiiinnn;


extern	volatile  uint8_t  	Tick_Base;
extern 	volatile  uint8_t  	Tick_5ms;
extern 	volatile  uint16_t  	Tick_LED;
extern 	volatile  uint16_t  	Tick_CAN;
extern 	volatile  uint16_t  	Tick_ADC;
extern 	volatile  uint16_t  	Tick_ADJ;
extern 	volatile  uint16_t  	Tick_HOL;
extern 	volatile  uint8_t  	Tick_PWR;
extern 	volatile  uint16_t  	Tick_BUZ;
extern 	volatile  uint16_t  	Tick_LOC;
extern 	volatile  uint16_t  	Tick_COM;
extern 	volatile  uint16_t  	Tick_UART;
extern 	volatile  uint16_t  	Tick_ECO;
extern 	volatile  uint16_t  	Tick_DIN;

extern can_frame_t Flash_Data;

typedef struct LockPlanBase
{
    uint8_t 	StepA; 
    uint8_t 	StepB;
    uint8_t 	Counter; 
    uint8_t 	Flag; 
    uint8_t 	Book;//001:pull,010:push,100:open
};
extern	volatile	struct	LockPlanBase	LockPlan;

typedef struct DIN_base//Digital input
{
    uint8_t 	Value; 
    uint8_t 	Filter[8]; 
    uint16_t 	Time[8];//T=Time*6,<=2000=12000ms=12s
};
extern volatile   struct DIN_base 	DIN;

typedef struct Lock_base
{
	uint8_t		Present;
    	uint16_t 	Time;
    	uint16_t 	TimeOut;
    	uint8_t 	Task;
	uint8_t 	Pole;//0:disable,1:enable
    	uint8_t 	Error;
};
extern volatile   struct Lock_base 	LockUp;
extern volatile   struct Lock_base 	LockDown;


typedef struct HOLL_base
{   
    uint16_t 	LPreCNT;
    uint16_t 	LCNT;  
    uint16_t 	LSPD;
    uint8_t 	LDIR;     
    uint8_t	LegalityTIM;
    uint8_t	SenseTIM;
};
extern volatile   struct HOLL_base 	HOLL;

typedef struct AntiNip_base
{      
    uint16_t 	Value;
    uint16_t 	DIR;
    uint8_t 	PID;
    uint8_t 	Angle;    
    
    uint8_t	Flag;
    uint8_t	FlagTIM;//Only change in HollLegalityCheck();
    uint8_t	AntiNipCheck1;
    uint16_t	AntiNipPosition1;
};
extern volatile   struct AntiNip_base 	AntiNip;

typedef struct PoleKEY_base
{
    uint8_t 	STA0; 
    uint16_t 	TIM0;   
    uint8_t 	STA1; 
    uint16_t 	TIM1;  
    uint8_t 	STA2; 
    uint16_t 	TIM2;     
};
extern volatile   struct PoleKEY_base 	PoleKEY;

typedef struct PWM_base
{
    uint8_t 	Flag; 	
    uint16_t 	DIR; 
    uint16_t 	Present; 
};
extern volatile   struct PWM_base 	PWM;


typedef struct PoleSign_base
{
    uint8_t 	CANid0;
    uint8_t 	CANid1; 
    uint8_t 	CANid2;
    uint8_t 	CANid3;
    uint8_t 	KEY0;
    uint8_t 	KEY1;
    uint8_t 	KEY2;
    uint8_t 	ADC0;
    uint8_t 	ADC1;
    uint8_t 	ADC2;
    uint8_t 	HollL;
    uint8_t 	HollR;
    uint8_t 	PWM;
    uint8_t 	Lock;
};
extern volatile   struct PoleSign_base 	PoleSign;


typedef struct ADCbase
{
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
};
extern volatile   struct ADCbase 	PoleADC;

typedef struct JOJO_base
{
    uint16_t 	Next;
    uint8_t 	Track;
    uint8_t 	Ratio;
    uint8_t 	Data[8];
};
extern	volatile   struct JOJO_base 	JOJO;

extern volatile  uint16_t  	Buzzer_NUM;
extern volatile  uint8_t  	CAN_TXD_CNT;
extern volatile  uint8_t  	CAN_RXD_CNT;

void LED_Tick(void);
void TJA1041_Normal(void);
void TJA1041_Sleep(void);
void Pole_System_ECO(void);

void Digital_Input(void);
void delay_ms(uint16_t ms);
void delay_100us(uint16_t us);

void CH1_Prohibit(void);
void CH2_Prohibit(void);
void CH3_Prohibit(void);
void CH4_Prohibit(void);
void CH5_Prohibit(void);
void CH1_PMOS_OUT(void);
void CH2_PMOS_OUT(void);
void CH3_PMOS_OUT(void);
void CH4_PMOS_OUT(void);
void CH5_PMOS_OUT(void);
void CH1_NMOS_OUT(uint16_t val);
void CH2_NMOS_OUT(uint16_t val);
void CH3_NMOS_OUT(uint16_t val);
void CH4_NMOS_OUT(uint16_t val);
void CH5_NMOS_OUT(uint16_t val);
void CH6_Speaker_PWM(uint16_t val);
void Brige_ALL_Prohibit(void);
void Brige1_Forward(uint16_t val);
void Brige2_Forward(uint16_t val);
void Brige3_Forward(uint16_t val);
void Brige4_Forward(uint16_t val);
void Brige1_Reverse(uint16_t val);
void Brige2_Reverse(uint16_t val);
void Brige3_Reverse(uint16_t val);
void Brige4_Reverse(uint16_t val);
void Brige1_Stop(void);
void Brige2_Stop(void);
void Brige3_Stop(void);
void Brige4_Stop(void);


void CAN_Logic(void);
void ADC_Logic(void);
void LED_ON(void);
void LED_OFF(void);

void Holl_Logic(void);
void Holl_Power_ON(void);
void Holl_Power_OFF(void);
void HollRefresh(void);

void PWR_Switch(void);
void PWR_Switch_ON(void);
void PWR_Switch_OFF(void);

void Buzzer_Logic(void);
void Buzzer_4KHz(void);
void Lock_KEY0_Logic(void);
void Lock_KEY1_Logic(void);
void Lock_KEY2_Logic(void);

void Lock_Pull_Push(void);

void Lock_Reset(void);
void Pole_Procedure(void);
void Lock_Opening(void);
void Door_Procedure(void);
void DoorNip_Procedure(void);
void LOCK_STATUS_Init(void);

void Pole_CAN_TXD(void);
void Pole_CAN_RXD_1(void);
void Pole_CAN_RXD_2(void);
void Overcurrent_Protect(void);
void CurrentProtect_Opening(void);
void CurrentProtect_Closing(void);
void PoleSign_Clear(void);
void HollLegalityCheck(void);
void HollPositionCheck(void);
void Holl_EEPROM_Command(void);

void CRXD_ID0(void);//id = 392
void CRXD_ID1(void);//id = 58b
void CRXD_ID2(void);//id = 393,Lock and Unlock
void CAN_BaseIO(void);

void Pole_Uart(void);
void Uart_SendByASCII(uint8_t *tx_buf, uint8_t tx_num);

void Halt(void);

//**************
void Pole_Procedure_Up(void);
void Pole_Procedure_Down(void);

void Lock_Open(void);
void Lock_OpenStop(void);
void Lock_Pull(void);
void Lock_PullStop(void);
void Lock_Push(void);
void Lock_PushStop(void);

void X3_Lock_Procedure(void);
void X3_Lock_Opening(void);
//***************************************
void DigitalInput(void);
void DigitalInput_8ch(void);

void LockPlan_Call(void);
void LockPlan_Open(void);
void LockPlan_Pull(void);
void LockPlan_Push(void);

void Lock_Procedure(void);

void MCU_Status(void);

void LockUp_Logic(void);
void LockUpLogic_Open(void);
void LockUpLogic_Half(void);
void LockUpLogic_All_Pull(void);
void LockUpLogic_All_Push(void);
void LockUp_Error(void);

void LockDown_Logic(void);
void LockDownLogic_Open(void);
void LockDownLogic_Pull(void);
void LockDownLogic_Push(void);

void PWM_Adjust(void);
void PWM_Adjust_Refresh(void);
void PWM_Adjust_PID();
void PWM_Stop(void);
void Angle_Catch(void);

void BaseTick(void);

//**************BaseIO*****************************************
void BaseIO_Motor(uint8_t ch,uint8_t dir,uint16_t val);
//**************BaseIO*****************************************

void Lock_KEY0_Logic2(void);
void SpringUp_Start(void);
void SpringUp_Stop(void);

void HollCheck_Logic(void);

void JOJO_Procedure(void);
void JOJO_Track(void);
void JOJO_CAN(void);//qiao 2018-4

#endif




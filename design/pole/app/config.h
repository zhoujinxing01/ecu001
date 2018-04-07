#ifndef 	Config_H
#define 	Config_H


static const uint16_t PWMOpen[16][2]={
//{1,900},//0
//{1,800},//1
{1,1100},//0
{1,1050},//1
{1,1000},//2
{1,950},//3
{1,900},//4
{1,850},//5
{1,850},//6
{1,850},//7
{1,850},//8
{1,850},//9
{1,850},//10
{1,850},//11
{1,850},//12
{1,800},//13
{1,750},//14
{1,700},//15
};

static const uint16_t  PWMClose[16][2]={
	/*
{0,900},//0
{0,800},//1
{0,800},//2
{0,800},//3
{0,800},//4
{0,800},//5
*/
{0,950},//0
{0,950},//1
{0,900},//2
{0,870},//3
{0,850},//4
{0,850},//5
{0,850},//6
{0,850},//7
{0,850},//8
{0,850},//9
{0,800},//10
{0,800},//11
{0,800},//12
{0,750},//13
{0,750},//14
{0,700},//15
};

#define	PWM_STOP_PID		30
#define	PWMPID			3
#define	PWMOpenRatio		10//0-10,open speed ratio
#define	PWMCloseRatio		8//0-10,close speed ratio
#define	HaltDIR			2//0,1,2
#define	HaltPWM			8

//#define	HOLL_POSITION_MAX	22000
//#define	HOLL_POSITION_MIN	21500

//#define		HOLL_POSITION_MAX	20510
//#define		HOLL_POSITION_MIN	20300

#define		HOLL_POSITION_MAX	20600
#define		HOLL_POSITION_MIN	20400

//#define		HOLL_POSITION_MAX	20900//DongJian pole
//#define		HOLL_POSITION_MIN	20500

//#define		HOLL_POSITION_MAX	20400
//#define		HOLL_POSITION_MIN	20200

//#define 	HOLL_SENSE		5//50ms
#define 	HOLL_SENSE		10//100ms
//#define 	HOLL_SENSE		20//200ms
#define 	HOLL_DIRerrorTIM	10//100ms
#define 	HOLL_SPDerrorTIM	10//100ms
//#define 	HOLL_DIRerrorTIM	100//1s
//#define 	HOLL_SPDerrorTIM	100//1s

#define 	AntiNipPWM		200
#define 	AntiNipPID		35
	
#define LOCK_Type		1//1:Up,0:Down


//#define LockUp_Open		12
//#define LockUp_Half		9
//#define LockUp_All_Pull		3
//#define LockUp_All_Push		11

//1:half,2:all,3:pull-push
#define LockUpMask		0x07
#define LockUp_Open		4
#define LockUp_Half		5
#define LockUp_All_Pull		3
#define LockUp_All_Push		7


/*
//edge and kuga
#define LockDownMask		0x1C
#define LockDown_Open		0x0C
#define LockDown_Push		0x08
#define LockDown_Pull		0x10
*/

//edge and kuga
#define LockDownMask		0x1C
#define LockDown_Open		0x08
#define LockDown_Push		0x0C
#define LockDown_Pull		0x14



#define LockPlan_Pull_TIM	250//2S
#define LockPlan_Push_TIM	250//2S
#define LockPlan_Open_TIM	50//0.5S
//#define LockPlan_DLY_TIM	10//100ms
#define LockPlan_DLY_TIM	30//300ms
#define SpringUp_EN		1//1:enable,0:disable
#define SpringUp_VAL		1000

#define zjx01 10
#define zjx02 15


#endif




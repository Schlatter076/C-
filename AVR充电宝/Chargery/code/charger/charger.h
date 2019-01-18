#ifndef _CHARGER_H
#define _CHARGER_H

//-----------------------------------------------------------------------------
// Balance Grade Definitions
//-----------------------------------------------------------------------------
#define BALANCE_VOLT 				10
//-----------------------------------------------------------------------------
// Charger State Definitions
//-----------------------------------------------------------------------------
#define RESET_INITIAL 				0
#define READY_START 				1
#define NORM_RUN	 				2
#define NORM_PAUSE	 				3
#define NORM_OVER	 				4
#define WARN_OVER	 				5
#define NORM_STOP	 				6
#define CYCLE_WAIT					7
#define FORCE_STOP	 				8
#define LOOP_SHUT		 			9
#define PWM_OUT_VOLT				10
#define DISCHARGE_ON 				11
#define BAL_OVER	 				12
//-----------------------------------------------------------------------------
// PMU Stage Definitions
//-----------------------------------------------------------------------------
#define PMU_IDLE	 				0
#define PMU_START	 				1
#define PMU_RUN		 				2
#define PMU_PAUSE	 				3
#define PMU_STOP	 				4
//-----------------------------------------------------------------------------
// Struct Type Definitions
//-----------------------------------------------------------------------------
typedef	struct _STPMU
{
	unsigned int  IA;
	unsigned int  Volt;
	unsigned char Serial;
	unsigned char BattT;
	unsigned char VMOST;
	unsigned int  mAh;
	unsigned int  Time;

	unsigned char CellH;
	unsigned char CellL;				

	unsigned char Stage;
	unsigned char State;
	
	unsigned char PWM	: 1;
	unsigned char LOOP	: 1;
	unsigned char DCHG	: 1;
	unsigned char BAL	: 1;			

	unsigned char ASK	: 1;
	unsigned char RUN	: 1;
	unsigned char OUT	: 1;
	unsigned char WARN	: 1;

	struct
	{
		unsigned int Time;
		unsigned int IA;
		unsigned int Volt;

	} Out;

} STPMU, *PSTPMU;

#endif

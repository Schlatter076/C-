#ifndef _MAIN_H
#define _MAIN_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "battery/battery.h"
#include "menu/menu.h"
#include "cpu/key.h"
#include "cpu/adc.h"
#include "cpu/pwm.h"
#include "charger/charger.h"
//-----------------------------------------------------------------------------
// Constants Definitions
//-----------------------------------------------------------------------------
#define Recv_Buf_Len  	   	20
#define Send_Buf_Len       	20
//-----------------------------------------------------------------------------
// Task Struct Type Definitions
//-----------------------------------------------------------------------------
typedef struct _STFLAG
{
	char Task0 	: 1;
	char Task1 	: 1;
	char Task2 	: 1;
	char Task3 	: 1;
	char Task4 	: 1;
	char Task5 	: 1;
	char Task6 	: 1;
	char Task7 	: 1;

} STFLAG,*PSTFLAG;
//-----------------------------------------------------------------------------
// Struct Definitions
//-----------------------------------------------------------------------------
typedef struct _STMSG
{
	struct 
	{
		unsigned char Li_Type;

		char Key_Beep		: 1;
		char Buzzer			: 1;		
		char Complete_Ring	: 1;
		char Back_Light		: 1;		
		char Cooling_Fan	: 1;

	} Set;

	struct 
	{
		struct 
		{
			char Beep	: 1;
			char Charge	: 1;
			char Blance	: 1;
			char PWM_Out: 1;
			char PWM_Up	: 1;

		} Status;

		struct 
		{
			char Key	: 1;
			char Fan	: 1;
			char BL		: 1;

		} Ctrl;
	} Run;

	STMENU Menu;

 	STBATT Batt;

	STCYCLE Cycle;

	STPMU  Pmu;
	
	STPWM  Pwm;

	STADC  Adc;

	STKEY  Key;

	volatile STFLAG bFlag;

    volatile unsigned char Count_ms;

	struct 
	{
		struct 
		{
			char TimeOut;
			char State;
			char Num;
			char Buf[Recv_Buf_Len];
		} Recv;

		struct 
		{
			char State;
			char Num;
			char Buf[Send_Buf_Len];
		} Send;

	} UART;

} STMSG, *PSTMSG;
//-----------------------------------------------------------------------------
// Variables Definitions
//-----------------------------------------------------------------------------
STMSG msg;

#endif

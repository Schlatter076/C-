#ifndef _BATTERY_H
#define _BATTERY_H

//-----------------------------------------------------------------------------
// Type Constant Definitions
//-----------------------------------------------------------------------------
#define LIPO					0
#define LIIO					1
#define LIFE					2
#define NIMH					3
#define NICD					4
#define PB						5
#define BATUD					6
//-----------------------------------------------------------------------------
// I Unit Falg Constant Definitions
//-----------------------------------------------------------------------------
#define ImA						0x00
#define ICA						0x80
//-----------------------------------------------------------------------------
// Serial Constant Definitions
//-----------------------------------------------------------------------------
#define PACK_MAX				6
#define SERIAL_MAX				12
//-----------------------------------------------------------------------------
// Cycle Constant Definitions
//-----------------------------------------------------------------------------
#define TIMES_MAX				5
#define TIMES					0x07
#define D_C						0x80
#define C_D						0x00
#define AUTO_MODE				0x40
#define MANUAL_MODE				0x00
//-----------------------------------------------------------------------------
// Li Function Mode Constant Definitions
//-----------------------------------------------------------------------------
#define MEASRUEMAN 				1
#define AUTO_BALANCE			2
#define BALANCE_CHARGE			3
#define CHARGE					4
#define FAST_CHARGE				5
#define DISCHARGE				6
#define STORAGE					7
#define CYCLE					8
#define MONITOR					9
#define AUTO_CHARGE				10
#define MANUAL_CHARGE			11
#define FORMING_CHARGE			13
//-----------------------------------------------------------------------------
// Menu String Item Constant Definitions
//-----------------------------------------------------------------------------
#define CUR_IMA					1
#define I_MA					2
#define I_CA					3
#define VOLT_SERIAL				4
#define VOLT					5
#define TEMPERATURE				6
#define CYCLE_CHARGE			7
#define CYCLE_DISCHARGE			8
#define SERIAL_NUMBER			9
#define SERIAL					10
#define CAPACITY				11
#define CAPACITY_LMT			12
#define TIME					13
#define VOLT_METE				14
#define BALANCE_METE			15
#define BALANCE_GRADE			16
#define PERCENT					17
#define TIME_CAPACITY			18
#define VOLT_CELLS				19
#define HIGH_LOW_CELL			20
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define OVER_DISCHARGE			17
#define OVER_CHARGE				18
//-----------------------------------------------------------------------------
// Struct Type Definitions
//-----------------------------------------------------------------------------
typedef	struct _STLIMIT
{
	unsigned char Volt_Level;
	unsigned char Charge_End_Volt;
	unsigned char Storge_Volt;
	unsigned char Discharge_End_Volt;
	unsigned char Charge_Max_ICA;

} STLIMIT, *PSTLIMIT;

typedef	struct _STCYCLE
{	
	unsigned char Form;

	struct
	{
		unsigned char Charge;
		unsigned char Discharge;

	} Volt;

	struct
	{
		unsigned int Time;
		unsigned int mAh;

	} Record[TIMES_MAX];

	unsigned char n;

} STCYCLE, *PSTCYCLE;

typedef	struct _STBATT
{
	unsigned char Type;
	unsigned char Mode;

	unsigned int  IA;
	unsigned int  mAh;
	unsigned int  Volt;
	unsigned char Pack;
	unsigned char Serial;

	STCYCLE Cycle;

	STLIMIT Limit;

} STBATT, *PSTBATT;

#endif

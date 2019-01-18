#ifndef _MENU_H
#define _MENU_H

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define START_LAYER				0
#define BOTTOM_LAYER			1
#define FUNCTION_LAYER			2
#define OPERATE_LAYER			3
#define END_LAYER				255
#define ONE_LAYER				1

#define BOTTOM_NUM				1
#define END_NUM					255
#define ONE_NUM					1

#define MENU_NODELAY			0
#define MENU_DELAY1				1
#define MENU_DELAY2				2
#define MENU_DELAY3				3
#define START_DELAY				3

#define NO_SET					0
#define CUR_LAYER				0			
#define CUR_NUM					0
//-----------------------------------------------------------------------------
// Menu Class Constant Definitions
//-----------------------------------------------------------------------------
#define LI_BATT_PROCESS			1
#define NI_BATT_PROCESS			2
#define PB_BATT_PROCESS			3
#define UD_BATT_PROCESS			4
#define SETTING_PARAMETE		5
#define SETTING_RESUME			6
#define BATT_SET_SAVE			7
#define BATT_SET_LOAD			8
#define DCPOWER_SET				9
//-----------------------------------------------------------------------------
// Struct Definitions
//-----------------------------------------------------------------------------
typedef struct _STMENU
{
	unsigned char Class;
	unsigned char Layer;
	unsigned char Num;
	unsigned char Sub;
	unsigned char Delay;

	unsigned char Layer_Top		: 1;
	unsigned char Num_Top		: 1;
	unsigned char Seq_End		: 1;
	unsigned char Seq_Inc		: 1;
	unsigned char Seq_Dec		: 1;
	unsigned char Toggle		: 1;
	unsigned char Update		: 1;
	unsigned char Key			: 1;

} STMENU, *PSTMENU;

#endif

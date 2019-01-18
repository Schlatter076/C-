//-----------------------------------------------------------------------------
// param.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "misc/misc.h"
#include "lcd/lcd.h"
#include "battery/battery.h"
#include <string.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void LCD_Update ( unsigned char column, unsigned char line, char *str );
void Chargery_Parameter_Set ( char * str, char flag );
char Chargery_Parameter_Resume ( void );
//-----------------------------------------------------------------------------
// Chargery Parameter Set Rountine
//-----------------------------------------------------------------------------
void Chargery_Parameter_Set ( char * str, char flag )
{
	switch ( msg.Menu.Num )
	{
		case 1:
			LCD_Update ( COL6, ROW1, "Li Type" );
			LCD_Update ( COL1, ROW2, "LiXX        X.XV" );
			break;

		case 2:
			LCD_Update ( COL6, ROW1, "Lithium battery" );
			LCD_Update ( COL1, ROW2, "Check Time XXmin" );
			break;

		case 3:
			LCD_Update ( COL6, ROW1, "Wait Time" );
			LCD_Update ( COL1, ROW2, "CHG/DCHG XXmin" );
			break;

		case 4:
			LCD_Update ( COL6, ROW1, "NiMH/NiCd/Pb" );
			LCD_Update ( COL1, ROW2, "Trickle    XXXmA" );
			break;

		case 5:
			LCD_Update ( COL6, ROW1, "NiMH Sensitivity" );
			LCD_Update ( COL1, ROW2, "DeltaV XXmV/Cell" );
			break;

		case 6:
			LCD_Update ( COL6, ROW1, "NiCd Sensitivity" );
			LCD_Update ( COL1, ROW2, "DeltaV XXmV/Cell" );
			break;

		case 7:
			LCD_Update ( COL6, ROW1, "NiMh/NiCd Check" );
			LCD_Update ( COL1, ROW2, "Delay       XXmin" );
			break;

		case 8:
			LCD_Update ( COL6, ROW1, "Temp. Cut-Off" );
			LCD_Update ( COL1, ROW2, "XXX          XX¡æ" );
			break;

		case 9:
			LCD_Update ( COL6, ROW1, "Safety timer" );
			LCD_Update ( COL1, ROW2, "XXX       XXXmin" );
			break;

		case 10:
			LCD_Update ( COL6, ROW1, "Capacity Cut-Off" );
			LCD_Update ( COL1, ROW2, "XXX    XXXXXmAh" );
			break;

		case 11:
			LCD_Update ( COL6, ROW1, "Input power low" );
			LCD_Update ( COL1, ROW2, "Cut-Off   XX.XXV" );
			break;

		case 12:
			LCD_Update ( COL6, ROW1, "Watt. Limit  (W)" );
			LCD_Update ( COL1, ROW2, "CHG:AUTO DCHG:30" );
			break;

		case 13:
			LCD_Update ( COL6, ROW1, "Key Beep     XXX" );
			LCD_Update ( COL1, ROW2, "Buzzer       XXX" );
			break;

		case 14:
			LCD_Update ( COL6, ROW1, "Completion Ring" );
			LCD_Update ( COL1, ROW2, "  Beep   5times" );
			break;

		case 15:
			LCD_Update ( COL6, ROW1, "Back-light   XX%" );
			LCD_Update ( COL1, ROW2, "Cooling fan XXXX" );
			break;

		default:
			break;
	}
}
//-----------------------------------------------------------------------------
// Chargery Parameter Resume Rountine
//-----------------------------------------------------------------------------
char Chargery_Parameter_Resume ( void )
{
	LCD_Update ( COL1, ROW1, "Resume Default?" );
	LCD_Update ( COL1, ROW2, "CONFIRM( ENTER)" );

	LCD_Update ( COL1, ROW1, "PLEASE WAIT..." );

	return 1;
}

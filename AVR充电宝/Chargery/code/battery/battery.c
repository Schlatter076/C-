//-----------------------------------------------------------------------------
// battery.c
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
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void LCD_Update ( unsigned char col, unsigned char row, char* str );
char Battery_Set_Save ( PSTMSG p );
char Battery_Set_Load ( PSTMSG p );
//-----------------------------------------------------------------------------
// Battery Set Save Rountine
//-----------------------------------------------------------------------------
char Battery_Set_Save ( PSTMSG p )
{
	LCD_Update ( COL1, ROW1, "LiPo    Charge" );
	LCD_Update ( COL1, ROW2, "10A  11.1V(3S)" );
	switch ( p->Menu.Num )
	{
		case 1:
			LCD_Update ( COL4, ROW1, "Saving No." );
			LCD_Update ( COL1, ROW2, "LiPo06CHG01" );
			break;
						
		case 2:
			LCD_Update ( COL4, ROW1, "Re Bat Setting" );
			LCD_Update ( COL1, ROW2, "LiPo06CHG01" );
			break;

		default:
			break;
	}

	switch ( p->Menu.Num )
	{
		case 1:
			LCD_Update ( COL1, ROW1, "Wait please..." );
			if ( Battery_Set_Save ( p ) == SUCCESS )
			{
				//memcset ( msg.Menu.Num, 0, sizeof (msg.Menu.Num));
				//p->Menu.ID |= (1<<4) & MENU_LAYER;
				//p->Menu.ID |= 2 & MENU_NUM;											
			}
			else ++p->Menu.Num;
			break;

		case 2:
			LCD_Update ( COL1, ROW1, "Memory limit" ); 
			LCD_Update ( COL1, ROW2, "CONFIRM(ENTER)" );
			++p->Menu.Num;
			break;

		case 3:
			LCD_Update ( COL1, ROW1, "Memory limit" ); 
			LCD_Update ( COL1, ROW2, "CANCEL(STOP)" );
			--p->Menu.Num;
			break;

		default:
			break;
	}
	return SUCCESS;
}
//-----------------------------------------------------------------------------
// Battery Set Load Rountine
//-----------------------------------------------------------------------------
char Battery_Set_Load ( PSTMSG p )
{
	LCD_Update ( COL1, ROW1, "Bat Saving No." );
	//Battery_Set_String ( str );
	//LCD_Update ( COL1, ROW2, str );
	LCD_Update ( COL10, ROW2, "" );

	return SUCCESS;
}

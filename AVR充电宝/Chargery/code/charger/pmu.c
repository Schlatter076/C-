//-----------------------------------------------------------------------------
// pmu.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "cpu/cpu.h"
#include "cpu/key.h"
#include "misc/misc.h"
#include "menu/menu.h"
#include "lcd/lcd.h"
#include "battery/battery.h"
#include "charger/charger.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern unsigned char FIFO_Clear ( unsigned char * FIFO, unsigned char Len );
extern unsigned char FIFO_Pop ( unsigned char * FIFO, unsigned char Len );
extern void LCD_Update ( unsigned char col, unsigned char row, char* str );
extern char * Get_Item ( PSTMSG p, char * str, unsigned char Type );
extern void Menu_Jump ( PSTMSG p, unsigned char Layer, unsigned char Num );
extern void Menu_Scroll ( PSTMSG p, unsigned char Top_Num );
void PMU_Out ( PSTMSG p );
//-----------------------------------------------------------------------------
// PMU Out Rountine
//-----------------------------------------------------------------------------
void PMU_Out ( PSTMSG p )
{
	char Line[COLUMN_MAX +1] = "";
	char str [COLUMN_MAX +1] = "";

	switch ( p->Menu.Layer )
	{
		case BOTTOM_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				Menu_Scroll ( p, 1 );
				if ( p->Menu.Update == UNNEED ) { p->Menu.Update = NEED;  break; }
			} 
			else ;
			p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
			p->Menu.Update = UNNEED;
			p->Menu.Key	= OPEN;

			sprintf ( Line, "%s", "DC Power" );
			sprintf ( str, "%dmin", p->Pmu.Out.Time );
			sprintf ( str +8, "%8dmin", p->Pmu.Out.Time );
			LCD_Update ( COL1, ROW1, Line );
			sprintf ( Line, "%s", Get_Item ( p, str, I_MA ));
			sprintf ( Line +4, "%12d", p->Batt.Volt );
			LCD_Update ( COL1, ROW2, Line );
			break;

		case FUNCTION_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				if ( p->Key.FIFO[0] & (ON<<KEY_STOP))
				{
					if 		( p->Pmu.State == NORM_RUN ) p->Pmu.State = FORCE_STOP;
					else if ( p->Pmu.State == NORM_STOP ) 
						Menu_Jump ( p, FUNCTION_LAYER - ONE_LAYER, 1 );
					else ;
				}
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
			} 
			if ( p->Menu.Toggle == OFF ) 	sprintf ( Line, "%s", "DCPS" );	else ;
			if ( p->Pmu.Out.Time == ( p->Pmu.Time /60 ))
			{
				p->Pmu.State = FORCE_STOP;
				if (( p->Menu.Toggle == ON ) && ( p->Pmu.State == NORM_STOP )) 
											sprintf ( Line, "%s", "DONE" );
				p->Menu.Toggle = ~p->Menu.Toggle;
			}
			sprintf ( Line + 4,  "%s", Get_Item ( p, str, CUR_IMA ));
		  	sprintf ( Line + 9, " %s", Get_Item ( p, str, VOLT ));
			LCD_Update ( COL1, ROW1, Line );

			sprintf ( Line, "%s", "OUT " );
			sprintf ( Line + 4, "%s", Get_Item ( p, str, TIME ));
			sprintf ( Line + 10, "%6dW", p->Pmu.Out.Volt * p->Pmu.Out.IA /100 );
			LCD_Update ( COL1, ROW2, Line );
			if ( p->Pmu.Time++ == 0 ) p->Pmu.State = READY_START;
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
// Pb.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "menu/menu.h"
#include "misc/misc.h"
#include "cpu/cpu.h"
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
extern void Battery_Limit_Set ( PSTLIMIT p, unsigned char Type );
extern char * Get_Item ( PSTMSG p, char * str, unsigned char Type );
extern void Menu_Jump ( PSTMSG p, unsigned char Layer, unsigned char Num );
extern void Menu_Scroll ( PSTMSG p, unsigned char Top_Num );
void Pb_Battery_Function ( PSTMSG P );
//-----------------------------------------------------------------------------
// Pb Battery Operate Rountine
//-----------------------------------------------------------------------------
void Pb_Battery_Function ( PSTMSG p )
{
	char Line[COLUMN_MAX +1] = "";
	char str [COLUMN_MAX +1] = "";

	switch ( p->Menu.Layer )
	{
		case FUNCTION_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				Menu_Scroll ( p, 2 );
				if (( p->Menu.Update == UNNEED ) && ( p->Menu.Sub == 0 ))
				    { p->Menu.Update = NEED;  break; }
			} 
			else ;
			//p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
			if ( p->Menu.Sub ) p->Menu.Update = NEED; 	
			else p->Menu.Update = UNNEED;
			p->Menu.Key	= OPEN;

			if ( p->Batt.Type == PB ) sprintf ( Line, "%s", "Pb  " );
			else return;
			Battery_Limit_Set ((PSTLIMIT) &p->Batt.Limit, p->Batt.Type );
			switch ( p->Menu.Num )
			{
				case 1:
					sprintf ( Line +4, "%12s", "CHARGE" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%d.%dAH ", p->Batt.mAh /1000, ( p->Batt.mAh %1000 )/100 );
					sprintf ( str, "%2d.%d(%dS)", (	p->Batt.Limit.Volt_Level * p->Batt.Serial )/100,
												 (( p->Batt.Limit.Volt_Level * p->Batt.Serial )%100 )/10,
													p->Batt.Serial );
					sprintf ( Line +6, "%10s", str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = CHARGE;
					break;

				case 2:
					sprintf ( Line +4, "%12s", "DISCHARGE" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", Get_Item ( p, str, I_MA ));
					sprintf ( Line +4, "%12d", p->Batt.Volt );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = DISCHARGE;
					p->Menu.Num_Top = ON;
					break;
				default:
					break;
			}
			break;

		case OPERATE_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				if ( p->Key.FIFO[0] & (ON<<KEY_STOP))
				{
					if 		( p->Pmu.State == NORM_RUN ) p->Pmu.State = FORCE_STOP;
					else if ( p->Pmu.State == NORM_STOP ) 
						Menu_Jump ( p, OPERATE_LAYER - ONE_LAYER, p->Batt.Mode );
				}
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
			} 
			else ;
			p->Menu.Update = NEED;
			p->Menu.Key	= OPEN;
			switch ( p->Menu.Num )
			{
				case 1:
					if ( p->Menu.Toggle == OFF ) sprintf ( Line, "Pb%02d", p->Batt.Serial );
					if ( p->Pmu.State == NORM_OVER ) 
					{
						if ( p->Menu.Toggle == ON ) sprintf ( Line, "%s", "DONE" );
						p->Menu.Toggle = ~p->Menu.Toggle;
					}
					sprintf ( Line + 4,  "%s", Get_Item ( p, str, CUR_IMA ));
  			    	sprintf ( Line + 9, " %s", Get_Item ( p, str, VOLT ));
					LCD_Update ( COL1, ROW1, Line );

					if		( p->Batt.Mode == CHARGE  	) sprintf ( Line, "%s", "CHG" );
					else if ( p->Batt.Mode == DISCHARGE	) sprintf ( Line, "%s", "DSC" );
					else ;
					sprintf ( Line + 3, "%s",  Get_Item ( p, str, CAPACITY ));
					sprintf ( Line + 9, "%7s", Get_Item ( p, str, TIME ));
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Num_Top = ON;
					break;
			default:
				break;
		}
		break;
		default:
			break;
	}
}

//-----------------------------------------------------------------------------
// Ni.c
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
void Ni_Battery_Function ( PSTMSG p );
//-----------------------------------------------------------------------------
// Ni Battery Function Rountine
//-----------------------------------------------------------------------------
void Ni_Battery_Function ( PSTMSG p )
{
	char Line[COLUMN_MAX +1] = "";
	char str [COLUMN_MAX +1] = "";

	switch ( p->Menu.Layer )
	{
		case FUNCTION_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				Menu_Scroll ( p, 5 );
				if (( p->Menu.Update == UNNEED ) && ( p->Menu.Sub == 0 )) break;
			}
			//p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
			if ( p->Menu.Sub ) p->Menu.Update = NEED;
			else p->Menu.Update = UNNEED;
			p->Menu.Key	= OPEN;
			if 	 ( p->Batt.Type == NIMH ) sprintf ( Line, "%s", "NiMH" );
			else						  sprintf ( Line, "%s", "NiCd" );
			Battery_Limit_Set ((PSTLIMIT) &p->Batt.Limit, p->Batt.Type );
			switch ( p->Menu.Num )
			{
				case 1:
					sprintf ( Line +4, "%12s", "CHARGE Aut" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", Get_Item ( p, str, I_MA ));
					sprintf ( Line +4, "%12s", "CUR LIMIT" );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = AUTO_CHARGE;
					break;

				case 2:
					sprintf ( Line +4, "%12s", "CHARGE Man" );
					LCD_Update ( COL1, ROW1, Line );
					if ( p->Menu.Seq_Inc == ON )
					{
						p->Menu.Seq_Inc = OFF;
						p->Batt.IA++;
					}
					else if ( p->Menu.Seq_Dec == ON )
					{
						p->Menu.Seq_Dec = OFF;
						p->Batt.IA--;
					}
					else;
					if ( p->Menu.Toggle == OFF )
						sprintf ( Line, "%s", Get_Item ( p, str, I_MA ));
					else
						sprintf ( Line, "%4s", " " );
					sprintf ( Line +4, "%12s", "CURRENT" );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = MANUAL_CHARGE;
					break;

				case 3:
					sprintf ( Line +4, "%12s", "FORMING CHG" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%d.%dAH ", p->Batt.mAh /1000, ( p->Batt.mAh %1000 )/100 );
					sprintf ( str, "%2d.%d(%dS)", (	p->Batt.Limit.Volt_Level * p->Batt.Serial )/100,
												 (( p->Batt.Limit.Volt_Level * p->Batt.Serial )%100 )/10,
													p->Batt.Serial );
					sprintf ( Line +6, "%10s", str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = FORMING_CHARGE;
					break;

				case 4:
					sprintf ( Line +4, "%12s", "DISCHARGE" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", Get_Item ( p, str, I_MA ));
					sprintf ( Line +4, "%12d", p->Batt.Volt );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = DISCHARGE;
					break;

				case 5:
					sprintf ( Line +4, " %s", "CYCLE" );
					sprintf ( str, "    %d", ( TIMES & p->Batt.Cycle.Form ));
					LCD_Update ( COL1, ROW1, Line );
					if ( D_C & p->Batt.Cycle.Form )
					{
						sprintf ( Line, "%s", "DCHG" );
						if ( AUTO_MODE & p->Batt.Cycle.Form ) sprintf ( Line +4, "%s", "(AUT)" );
						else  								  sprintf ( Line +4, "%s", "(MAN)" );
						sprintf ( Line, "%s", "->CHG" );
					}
					else 
					{
						sprintf ( Line, "%s", "CHG" );
						if ( AUTO_MODE & p->Batt.Cycle.Form ) sprintf ( Line +3, "%s", "(AUT)" );
						else  								  sprintf ( Line +3, "%s", "(MAN)" );
						sprintf ( Line, "%s", "->DCHG" );
					}
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = CYCLE;
					p->Menu.Num_Top = ON;
					break;

				default:
					break;
			}
			break;

		case OPERATE_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				p->Menu.Update = NEED;
				if ( p->Key.FIFO[0] & (ON<<KEY_STOP))
				{
					if 		( p->Pmu.State == NORM_RUN ) p->Pmu.State = FORCE_STOP;
					else if ( p->Pmu.State == NORM_STOP ) 
						Menu_Jump ( p, OPERATE_LAYER - ONE_LAYER, p->Batt.Mode );
					else  p->Menu.Update = UNNEED;
				}
				else if ( p->Key.FIFO[0] == (ON<<KEY_INC))
				{
					if (( p->Batt.Mode == CYCLE ) && ( p->Pmu.State == CYCLE_WAIT ))
						if ( p->Menu.Num == 1 ) { p->Menu.Num = 2;  p->Menu.Sub = 0; }
						else if ( p->Menu.Sub < p->Cycle.n ) p->Menu.Sub++;
						else p->Menu.Num = 1;
					else p->Menu.Update = UNNEED;
				}
				else p->Menu.Update = UNNEED;
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
				if (( p->Menu.Update == UNNEED )&&( p->Menu.Num != 1 )) break;  
			}
			p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
			p->Menu.Key	= OPEN;
			switch ( p->Menu.Num )
			{
				case 1:
					if ( p->Menu.Toggle == OFF ) 
						if ( p->Batt.Type == NIMH ) sprintf ( Line, "%s", "NiMH" );
						else					    sprintf ( Line, "%s", "NiCd" );	
					else ;
					if ( p->Pmu.State == NORM_PAUSE ) 
					{
						if ( p->Menu.Toggle == ON ) sprintf ( Line, "%s", "WAIT" );
						p->Menu.Toggle = ~p->Menu.Toggle;
					}
					if ( p->Pmu.State == NORM_OVER  )
					{
						if ( p->Menu.Toggle == ON ) sprintf ( Line, "%s", "DONE" );
						p->Menu.Toggle = ~p->Menu.Toggle;
					}
					sprintf ( Line + 4,  "%s", Get_Item ( p, str, CUR_IMA ));
  			    	sprintf ( Line + 9, " %s", Get_Item ( p, str, VOLT ));
					LCD_Update ( COL1, ROW1, Line );

					if 		( p->Batt.Mode == AUTO_CHARGE	 ) sprintf ( Line, "%s", "Aut" );
					else if	( p->Batt.Mode == MANUAL_CHARGE  ) sprintf ( Line, "%s", "CHG" );
					else if ( p->Batt.Mode == FORMING_CHARGE ) sprintf ( Line, "%s", "FRM" );
					else if ( p->Batt.Mode == DISCHARGE		 ) sprintf ( Line, "%s", "DSC" );
					else if ( p->Batt.Mode == CYCLE			 ) 
						 if ( p->Batt.Cycle.Form & D_C 		 ) sprintf ( Line, "%s", "D>C" );
						 else 					  			   sprintf ( Line, "%s", "C>D" );
					else ;
					sprintf ( Line + 3, "%s",  Get_Item ( p, str, CAPACITY ));
					sprintf ( Line + 9, "%7s", Get_Item ( p, str, TIME ));
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Update = NEED;
					break;

				case 2:
					sprintf ( Line, "%d ", 1+ p->Menu.Sub );
					if ( p->Batt.Cycle.Form & D_C ) sprintf ( Line + 2, "DCHG" );
					else  							sprintf ( Line + 2, "CHG " );
					sprintf ( Line + 6, "%s", Get_Item ( p, str, TIME_CAPACITY ));
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%d ", 1+ p->Menu.Sub );
					if ( p->Batt.Cycle.Form & D_C ) sprintf ( Line + 2, "CHG " );
					else  							sprintf ( Line + 2, "DCHG" );
					sprintf ( Line + 6, "%s", Get_Item ( p, str, TIME_CAPACITY ));;
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Update = UNNEED;
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

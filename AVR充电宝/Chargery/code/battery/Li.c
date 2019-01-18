//-----------------------------------------------------------------------------
// Li.c
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
#include "lcd/lcd.h"
#include "cpu/cpu.h"
#include "cpu/adc.h"
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
extern void Battery_Limit_Set ( PSTLIMIT p, unsigned char Type );
extern void ImA_VoltSerial_Set ( PSTMSG p, unsigned char * Line, unsigned char * str );
extern void Capacity_ICA_Set ( PSTMSG p, unsigned char * Line, unsigned char * str );
extern void Cycle_Items_Set ( PSTMSG p, unsigned char * Line, unsigned char * str );
extern void Cycle_Volt_Set ( PSTMSG p, unsigned char Direct );
extern unsigned char Detect_Batt_Count ( PSTMSG p );
extern unsigned char Detect_Cell_Volt ( PSTMSG p, char * str, unsigned char Volt );
extern void Menu_Jump ( PSTMSG p, unsigned char Layer, unsigned char Num );
extern void Menu_Scroll ( PSTMSG p, unsigned char Top_Num );
void Li_Battery_Function ( PSTMSG p );
//-----------------------------------------------------------------------------
// Li Battery Function Rountine
//-----------------------------------------------------------------------------
void Li_Battery_Function ( PSTMSG p )
{
	char Line[COLUMN_MAX +1] = "";
	char str [COLUMN_MAX +1] = "";

	switch ( p->Menu.Layer )
	{
		case FUNCTION_LAYER:		
			if ( p->Menu.Key == OPEN )
			{
				Menu_Scroll ( p, 8 );
				if (( p->Menu.Update == UNNEED ) && ( p->Menu.Sub == 0 ))
				    { p->Menu.Update = NEED;  break; }
			} 
			else ;
			//p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
			if ( p->Menu.Sub ) p->Menu.Update = NEED; 	
			else p->Menu.Update = UNNEED;
			p->Menu.Key	= OPEN;
			if	 	( p->Batt.Type == LIPO ) sprintf ( Line, "%s", "LiPo" );
			else if	( p->Batt.Type == LIIO ) sprintf ( Line, "%s", "LiIo" );
			else if	( p->Batt.Type == LIFE ) sprintf ( Line, "%s", "LiFe" );
			else return;
			Battery_Limit_Set ((PSTLIMIT) & p->Batt.Limit, p->Batt.Type );
			switch ( p->Menu.Num )
			{
				case BOTTOM_NUM:
					sprintf ( Line + 4, "%12s", "BATTERY" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", "MEASRUEMAN" );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = MEASRUEMAN;
					break;

				case 2:
					sprintf ( Line + 4, "%12s", "BATTERY" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", "AUTO" );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = AUTO_BALANCE;
					break;

				case 3:
					sprintf ( Line + 4, "%12s", "BALANCE CHG" );
					LCD_Update ( COL1, ROW1, Line );
					Capacity_ICA_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = BALANCE_CHARGE;
					break;

				case 4:
					sprintf ( Line + 4, "%12s", "CHARGE" );
					LCD_Update ( COL1, ROW1, Line );
					ImA_VoltSerial_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = CHARGE;
					break;

				case 5:
					sprintf ( Line + 4, "%12s", "FAST CHG" );
					LCD_Update ( COL1, ROW1, Line );
					ImA_VoltSerial_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = FAST_CHARGE;
					break;

				case 6:
					sprintf ( Line + 4, "%5s", "DCHG" );
					sprintf ( Line + 9, "%7s", Get_Item ( p, str, VOLT ));
					LCD_Update ( COL1, ROW1, Line );
					ImA_VoltSerial_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = DISCHARGE;
					break;

				case 7:
					sprintf ( Line + 4, "%12s", "STORAGE" );
					LCD_Update ( COL1, ROW1, Line );
					Capacity_ICA_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = STORAGE;
					break;

				case 8:
					sprintf ( Line + 4,  "%6s", "CYCLE" );
					sprintf ( Line + 10, "%6d", ( p->Batt.Cycle.Form & TIMES )+1 );
					Cycle_Items_Set ( p, Line, str );
					LCD_Update ( COL1, ROW1, Line );
					if ( p->Batt.Cycle.Form & D_C )
						 sprintf ( Line, "%s", Get_Item ( p, str, CYCLE_DISCHARGE ));
					else sprintf ( Line, "%s", Get_Item ( p, str, CYCLE_CHARGE ));
					Cycle_Items_Set ( p, Line, str );
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = CYCLE;
					break;

				case 9:
					sprintf ( Line + 4, "%8s", "MONITOR" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%16s", Get_Item ( p, str, VOLT_SERIAL ));
					LCD_Update ( COL1, ROW2, Line );
					p->Batt.Mode = MONITOR;
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
					if ( p->Pmu.State == ON ) p->Pmu.State = FORCE_STOP;
					else Menu_Jump ( p, OPERATE_LAYER - ONE_LAYER, p->Batt.Mode );
					if     (( p->Menu.Num == 12 )||( p->Menu.Num == 13 )) 
												{ p->Menu.Num = 1;	p->Menu.Sub = 1; }
					else if ( p->Menu.Num == 14 ) p->Menu.Num = 9;
					else  p->Menu.Update = UNNEED;
				}
				else if ( p->Key.FIFO[0] & (ON<<KEY_ENTER))
				{
					if ( p->Menu.Num == 8 ) p->Menu.Num = 9;
					else p->Menu.Update = UNNEED;
				}
				else if ( p->Key.FIFO[0] == (ON<<KEY_INC))
				{
					if		( p->Menu.Num ==  9 )  { p->Menu.Num = 10;  p->Menu.Sub = 0; }
					else if	( p->Menu.Sub ==  0 )    p->Menu.Sub =  1;
					else if ( p->Menu.Num ==  1 )  { p->Menu.Num = 10;  p->Menu.Sub = 0; }
					else if ( p->Menu.Num == 10 )
						 if	( p->Batt.Mode != CYCLE ) p->Menu.Num = 9;
						 else { p->Menu.Num = 11;  p->Menu.Sub = 0; }
					else if ( p->Menu.Num == 11 )
						 if ( p->Menu.Sub < 4 ) p->Menu.Sub ++;
						 else p->Menu.Num = 9;
					else p->Menu.Update = UNNEED;
				}
				else if ( p->Key.FIFO[0] == (ON<<KEY_DEC))
				{
					if ( p->Batt.Mode == MEASRUEMAN )
					{
						if  ( p->Menu.Num == 10 ) 
							{ p->Menu.Num = 1;	p->Menu.Sub = 1; }
						else if (( p->Menu.Num == 1 )&&( p->Menu.Sub == 1 ))
							 p->Menu.Sub = 0;
						else p->Menu.Update = UNNEED;
					}
					else if (( p->Menu.Num == 9 )&&( p->Batt.Mode != AUTO_BALANCE ))
						;//Menu_Jump ( p, Parameter_LAYER, 1 );
					else p->Menu.Update = UNNEED;
				}
				else p->Menu.Update = UNNEED;
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
				if (( p->Menu.Update == UNNEED )&&( p->Menu.Num != 9 ))
				    { p->Menu.Update = NEED;  break; }
			} 
			else ;
			p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );	
			p->Menu.Update = UNNEED;
			p->Menu.Key	= OPEN;
			switch ( p->Menu.Num )
			{
				case MEASRUEMAN:
					if ( p->Menu.Sub == 0 ) 
					{
						sprintf ( Line, "SOC:%s", Get_Item ( p, str, VOLT_METE ));
						LCD_Update ( COL1, ROW1, Line );
						sprintf ( Line, "%s Fuel", Get_Item ( p, str, PERCENT ));
						sprintf ( Line + 8, "%8s", Get_Item ( p, str, VOLT ));
						LCD_Update ( COL1, ROW2, Line );
					}
					else 
					{
						sprintf ( Line,  "BAL:%s", Get_Item ( p, str, BALANCE_METE ));
						sprintf ( Line + 16, "%s", Get_Item ( p, str, BALANCE_GRADE ));
						LCD_Update ( COL1, ROW1, Line );
						sprintf ( Line, "%s", Get_Item ( p, str, HIGH_LOW_CELL ));
						LCD_Update ( COL1, ROW2, Line );
					}
					break;

				case CHARGE:
					 p->Menu.Num = CYCLE;
				case FAST_CHARGE:
					 p->Menu.Num = CYCLE;
				case DISCHARGE:
					 p->Menu.Num = CYCLE;
				case CYCLE:
					sprintf ( Line, "R:%02dSER  S:%02dSER", Detect_Batt_Count ( p ), 
															p->Batt.Pack * p->Batt.Serial );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", "CONFIRM (ENTER)" );
					LCD_Update ( COL1, ROW2, Line );
					break;

				case AUTO_BALANCE:
					 p->Menu.Num = MONITOR;
				case BALANCE_CHARGE:
					 p->Menu.Num = MONITOR;
				case STORAGE:
				 	 p->Menu.Num = MONITOR;
				case MONITOR:
					if ( p->Menu.Toggle == OFF )
					{
						if ( p->Pmu.State != NORM_OVER )
						{
							if	 	( p->Batt.Type == LIPO ) sprintf ( Line, "%s", "LiPo" );
							else if	( p->Batt.Type == LIIO ) sprintf ( Line, "%s", "LiIo" );
							else 							 sprintf ( Line, "%s", "LiFe" );
						}
						else
						{
							if (( p->Batt.Mode != AUTO_BALANCE ) &&
						    	( p->Batt.Mode != MONITOR )) sprintf ( Line, "%s", "DONE" );
						}
					}
					else  
						sprintf ( Line + 2, "%s", Get_Item ( p, str, p->Batt.Pack * p->Batt.Serial ));
					p->Menu.Toggle = ~p->Menu.Toggle;

					if ( p->Batt.Mode == MONITOR )
						 sprintf ( Line + 4, " %s", Get_Item ( p, str, TEMPERATURE ));
					else sprintf ( Line + 4,  "%s", Get_Item ( p, str, CUR_IMA ));
	 					 sprintf ( Line + 9, " %s", Get_Item ( p, str, VOLT ));
					LCD_Update ( COL1, ROW1, Line );

					if 		( p->Batt.Mode == AUTO_BALANCE	 ) sprintf ( Line, "%s", "Aut" );
					else if	( p->Batt.Mode == BALANCE_CHARGE ) sprintf ( Line, "%s", "BAL" );
					else if ( p->Batt.Mode == CHARGE		 ) sprintf ( Line, "%s", "CHG" );
					else if ( p->Batt.Mode == FAST_CHARGE	 ) sprintf ( Line, "%s", "FAS" );
					else if ( p->Batt.Mode == STORAGE		 ) sprintf ( Line, "%s", "STO" );
					else if ( p->Batt.Mode == DISCHARGE		 ) sprintf ( Line, "%s", "DIS" );
					else if ( p->Batt.Mode == CYCLE			 ) 
						 if ( p->Batt.Cycle.Form & D_C 		 ) sprintf ( Line, "%s", "D>C" );
						 else 					  			   sprintf ( Line, "%s", "C>D" );
					else ;
					if 		( p->Batt.Mode == MONITOR 	   ) { sprintf ( Line, "%s", "Mon" );
						 sprintf ( Line + 3, "%d.%02dV", p->Pmu.CellL / 100, 
						 								 p->Pmu.CellL % 100 ); 	
														 	 					}
					else sprintf ( Line + 3, "%s",  Get_Item ( p, str, CAPACITY ));
						 sprintf ( Line + 9, "%7s", Get_Item ( p, str, TIME ));
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Update = NEED;
					break;

				case 10:
					sprintf ( Line +1, "%s", Get_Item ( p, str, VOLT_CELLS ));
					sprintf ( Line, "%d:", 1+ p->Menu.Sub *6 );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line +1, "%s", Get_Item ( p, str, VOLT_CELLS ));
					sprintf ( Line, "%d:", 4+ p->Menu.Sub *6 );
					LCD_Update ( COL1, ROW2, Line );

					if ( p->Batt.Mode == MEASRUEMAN )
					{
						if ( p->Menu.Toggle == OFF )
						{
						    if ( Detect_Cell_Volt ( p, str, OVER_DISCHARGE ))
								p->Menu.Num = 12;
						}
						else	
						{
						    if ( Detect_Cell_Volt ( p, str, OVER_CHARGE ))						
								p->Menu.Num = 13;
						}
						p->Menu.Key	= CLOSE;
						p->Menu.Toggle = ~p->Menu.Toggle;
					}
					else if (( p->Batt.Mode == AUTO_BALANCE )&&( p->Pmu.State == BAL_OVER ))
					{
						p->Menu.Num = 14;
 						p->Menu.Key	= CLOSE;
					}
					else ;
					break;

				case 11:
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
					break;

				case 12:
					sprintf ( Line, "CELL %s", str );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", "Over Discharge" );
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Key	= OPEN;
					break;

				case 13:
					sprintf ( Line, "CELL %s", str );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", "Over Charge" );
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Key	= OPEN;
					break;

				case 14:
					sprintf ( Line, "Battery Balanced" );
					LCD_Update ( COL1, ROW1, Line );
					sprintf ( Line, "%s", Get_Item ( p, str, HIGH_LOW_CELL ));
					LCD_Update ( COL1, ROW2, Line );
					p->Menu.Key	= OPEN;
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

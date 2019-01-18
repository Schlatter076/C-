//-----------------------------------------------------------------------------
// items.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "cpu/adc.h"
#include "battery/battery.h"
#include <string.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
char * Get_Item ( PSTMSG p, char * str, unsigned char Type );
void Cell_Voltage_String ( PSTMSG p, char * str );
//-----------------------------------------------------------------------------
// Get String Rountine
//-----------------------------------------------------------------------------
char * Get_Item ( PSTMSG p, char * str, unsigned char Type )
{
	unsigned char i = 0;

	switch ( Type )
	{
		case I_MA:
			if 		( p->Batt.IA < 100  ) sprintf ( str, ".%02dA", p->Batt.IA );
			else if ( p->Batt.IA < 1000 ) sprintf ( str, "%d.%dA", p->Batt.IA /100, 
																 ( p->Batt.IA %100) /10 );
			else 				   		  sprintf ( str, "10A " );
			break;					

		case CUR_IMA:
			if 		( p->Pmu.IA < 100  ) sprintf ( str, ".%02dA", p->Pmu.IA );
			else if ( p->Pmu.IA < 1000 ) sprintf ( str, "%d.%dA", p->Pmu.IA /100, 
																 ( p->Pmu.IA %100) /10 );
			else 				   		 sprintf ( str, "10A " );
			break;

		case I_CA:  							
			if 		( p->Batt.IA <  10 ) sprintf ( str,  "0.%dC", p->Batt.IA );
			else if ( p->Batt.IA <= 40 ) sprintf ( str, "%d.%dC", p->Batt.IA /10, p->Batt.IA %10 );
			else ;
			break;

		case VOLT_SERIAL:  
			p->Batt.Volt = p->Batt.Limit.Volt_Level *( p->Batt.Pack +1 )*( p->Batt.Serial +1 );									
			sprintf ( str, " %2d.%dV", p->Batt.Volt /100, ( p->Batt.Volt %100 ) /10 );
			if ( p->Batt.Pack ) sprintf ( str + strlen (str), "(%dX", p->Batt.Pack +1 );
			sprintf ( str +strlen (str), "%dS)", p->Batt.Serial +1 );
			break;

		case VOLT:  							
			sprintf ( str, " %2d.%02dV", p->Pmu.Volt /100, p->Pmu.Volt %100 );
			break;

		case TEMPERATURE:  							
			sprintf ( str, " %05d", p->Pmu.BattT );
			break;

		case CAPACITY:  							
			sprintf ( str, " %05d", p->Pmu.mAh );
			break;

		case TIME:  							
			sprintf ( str, " %03d:%02d", p->Pmu.Time /60, p->Pmu.Time %60 );
			break;

		case CYCLE_CHARGE:
			sprintf ( str, "C:%d.%02dV->D:%d.%02dV", ( 420 - p->Batt.Cycle.Volt.Charge ) /10, 
												   ( 420 - p->Batt.Cycle.Volt.Charge ) %10,
												   ( p->Batt.Cycle.Volt.Discharge +300 ) /10, 
												   ( p->Batt.Cycle.Volt.Discharge +300 ) %10 );
			break;

		case CYCLE_DISCHARGE:
			sprintf ( str, "D:%d.%02dV->C:%d.%02dV", ( p->Batt.Cycle.Volt.Discharge +300 ) /10,
												   ( p->Batt.Cycle.Volt.Discharge +300 ) %10,
											       ( 420 - p->Batt.Cycle.Volt.Charge ) /10, 
												   ( 420 - p->Batt.Cycle.Volt.Charge ) %10 );
			break;

		case SERIAL:
			sprintf ( str, "%02d", 	( p->Batt.Pack +1 )*( p->Batt.Serial +1 ));
			break;

		case CAPACITY_LMT:  							
			sprintf ( str, "%d0mAh", p->Batt.mAh );
			break;

		case VOLT_METE:
			for ( ; i < (p->Pmu.Volt /( p->Batt.Limit.Volt_Level /10 )); i++ )
				sprintf ( str, "%c", 0xFF );
			break;

		case BALANCE_METE:  
			for ( ; i < (( p->Pmu.CellH - p->Pmu.CellL ) /10 ); i++ )
				sprintf ( str, "%c", 0xFF );
			break;

		case PERCENT:  							
			sprintf ( str, "%d%d%%", ( p->Pmu.Volt /( p->Batt.Limit.Volt_Level /10 )),
									 ( p->Pmu.Volt %( p->Batt.Limit.Volt_Level /10 )) /10);
			break;

		case BALANCE_GRADE:
			if 		(( p->Pmu.CellH - p->Pmu.CellL ) < BALANCE_VOLT    ) sprintf ( str, "A" );
			else if (( p->Pmu.CellH - p->Pmu.CellL ) < BALANCE_VOLT *2 ) sprintf ( str, "B" );
			else if (( p->Pmu.CellH - p->Pmu.CellL ) < BALANCE_VOLT *3 ) sprintf ( str, "C" );
			else if (( p->Pmu.CellH - p->Pmu.CellL ) < BALANCE_VOLT *4 ) sprintf ( str, "D" );
			else    								      sprintf ( str, "E" );
			break;

		case TIME_CAPACITY:
			sprintf ( str, " %3d %5d", p->Batt.Cycle.Record[ p->Menu.Sub ].Time /60, 
									   p->Batt.Cycle.Record[ p->Menu.Sub ].mAh );
			break;

		case VOLT_CELLS:
			for ( ; i < 3; i++ ) 
				if ( p->Adc.Volt[CELL(i)] )
					 sprintf ( str +i*4, " %d.%02d", p->Adc.Volt[CELL(i)] /( 100 /CELL_DIV ),
													 p->Adc.Volt[CELL(i)] %( 100 /CELL_DIV ));
				else sprintf ( str +i*4, "----" );
			break;

		case HIGH_LOW_CELL:
			sprintf ( str, "H:%d.%02dV  L:%d.%02dV", 
							p->Pmu.CellH /100, p->Pmu.CellH %100,
							p->Pmu.CellL /100, p->Pmu.CellL %100 );
			break;
		default:
			break;
	}
	return str;
}


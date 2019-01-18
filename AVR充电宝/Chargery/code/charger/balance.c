//-----------------------------------------------------------------------------
// balance.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "battery/battery.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
unsigned char Detect_Batt_Count ( PSTMSG p );
unsigned char Detect_Cell_Volt ( PSTMSG p, char * str, unsigned char Volt );
//-----------------------------------------------------------------------------
// Detect Battery Count Rountine
//-----------------------------------------------------------------------------
unsigned char Detect_Batt_Count ( PSTMSG p )
{
	unsigned char i = 0;
	unsigned char n = 0;

	for ( ; i < SERIAL_MAX; i++ ) if ( p->Adc.Volt[CELL(i)] != 0 ) n++;
	return n;
}
//-----------------------------------------------------------------------------
// Detect Cell Volt Rountine
//-----------------------------------------------------------------------------
unsigned char Detect_Cell_Volt ( PSTMSG p, char * str, unsigned char Volt )
{
	unsigned char i = 0;
	unsigned char n = 0;

	n = p->Batt.Pack * p->Batt.Serial;
	if ( Volt == OVER_DISCHARGE )
	{
		for ( ; i < n; i++ )
			if ( p->Adc.Volt[CELL(i)] < p->Batt.Limit.Discharge_End_Volt ) break;
		if ( i == n ) return FALSE;
		sprintf ( str, "%02d", i +1 );	
	}
	else if ( Volt == OVER_CHARGE )
	{
		for ( ; i < n; i++ )
			if ( p->Adc.Volt[CELL(i)] > p->Batt.Limit.Charge_End_Volt ) break;
		if ( i == n ) return FALSE;
		sprintf ( str, "%02d", i +1 );				
	}
	else ;
	return ( i + 1 );
}

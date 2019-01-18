//-----------------------------------------------------------------------------
// Charger.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "battery/battery.h"
#include "charger/charger.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
void Charger_Initial ( PSTMSG p );
void Charger_Operate ( PSTMSG p );
//-----------------------------------------------------------------------------
// Charger Initial Rountine
//-----------------------------------------------------------------------------
void Charger_Initial ( PSTMSG p )
{

}
//-----------------------------------------------------------------------------
// Charger Operate Rountine
//-----------------------------------------------------------------------------
void Charger_Operate ( PSTMSG p )
{
	switch ( p->Pmu.Stage )
	{
		case PMU_IDLE:

			break;

		case PMU_START:

			break;

		case PMU_RUN:

			break;

		case PMU_PAUSE:

			break;

		case PMU_STOP:

			break;

		default:
			break;
	}
}

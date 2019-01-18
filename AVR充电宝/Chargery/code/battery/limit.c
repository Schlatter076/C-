//-----------------------------------------------------------------------------
// limit.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "battery/battery.h"
//-----------------------------------------------------------------------------
// LiPo Battery Constant Definitions
//-----------------------------------------------------------------------------
#define LIPO_VOLT_LEVEL						370
#define LIPO_CHARGE_END_VOLT				420
#define LIPO_STORGE_VOLT					385
#define LIPO_DISCHARGE_END_VOLT				300
#define LIPO_CHARGE_MAX_ICA					10
//-----------------------------------------------------------------------------
// LiIon Battery Constant Definitions
//-----------------------------------------------------------------------------
#define LIIO_VOLT_LEVEL						360
#define LIIO_CHARGE_END_VOLT				410
#define LIIO_STORGE_VOLT					375
#define LIIO_DISCHARGE_END_VOLT				250
#define LIIO_CHARGE_MAX_ICA					10
//-----------------------------------------------------------------------------
// LiFe Battery Constant Definitions
//-----------------------------------------------------------------------------
#define LIFE_VOLT_LEVEL						330
#define LIFE_CHARGE_END_VOLT				360
#define LIFE_STORGE_VOLT					330
#define LIFE_DISCHARGE_END_VOLT				200
#define LIFE_CHARGE_MAX_ICA					40
//-----------------------------------------------------------------------------
// NiMh Battery Constant Definitions
//-----------------------------------------------------------------------------
#define NIMH_VOLT_LEVEL						120
#define NIMH_CHARGE_END_VOLT				0
#define NIMH_STORGE_VOLT					0
#define NIMH_DISCHARGE_END_VOLT				100
#define NIMH_CHARGE_MAX_ICA					20
//-----------------------------------------------------------------------------
// NiCd Battery Constant Definitions
//-----------------------------------------------------------------------------
#define NICD_VOLT_LEVEL						120
#define NICD_CHARGE_END_VOLT				0
#define NICD_STORGE_VOLT					0
#define NICD_DISCHARGE_END_VOLT				85
#define NICD_CHARGE_MAX_ICA					20
//-----------------------------------------------------------------------------
// Pb Acid Battery Constant Definitions
//-----------------------------------------------------------------------------
#define PB_VOLT_LEVEL						200
#define PB_CHARGE_END_VOLT					246
#define PB_STORGE_VOLT						0
#define PB_DISCHARGE_END_VOLT				175
#define PB_CHARGE_MAX_ICA					5
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
void Battery_Limit_Set ( PSTLIMIT p, unsigned char Type );
//-----------------------------------------------------------------------------
// Battery Limit Set Rountine
//-----------------------------------------------------------------------------
void Battery_Limit_Set ( PSTLIMIT p, unsigned char Type )
{
	switch ( Type )
	{
		case LIPO:
			p->Volt_Level 		  = LIPO_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt 	  = LIPO_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt 		  = LIPO_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = LIPO_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = LIPO_CHARGE_MAX_ICA / CELL_DIV;
			break;

		case LIIO:
			p->Volt_Level         = LIIO_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt    = LIIO_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt        = LIIO_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = LIIO_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = LIIO_CHARGE_MAX_ICA / CELL_DIV;
			break;

		case LIFE:
			p->Volt_Level         = LIFE_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt    = LIFE_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt        = LIFE_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = LIFE_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = LIFE_CHARGE_MAX_ICA / CELL_DIV;
			break;

		case NIMH:
			p->Volt_Level         = NIMH_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt    = NIMH_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt        = NIMH_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = NIMH_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = NIMH_CHARGE_MAX_ICA / CELL_DIV;
			break;

		case NICD:
			p->Volt_Level         = NICD_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt    = NICD_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt        = NICD_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = NICD_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = NICD_CHARGE_MAX_ICA / CELL_DIV;
			break;

		case PB:
			p->Volt_Level         = PB_VOLT_LEVEL / CELL_DIV;
			p->Charge_End_Volt    = PB_CHARGE_END_VOLT / CELL_DIV;
			p->Storge_Volt        = PB_STORGE_VOLT / CELL_DIV;
			p->Discharge_End_Volt = PB_DISCHARGE_END_VOLT / CELL_DIV;
			p->Charge_Max_ICA 	  = PB_CHARGE_MAX_ICA / CELL_DIV;
			break;

		default:
			break;
	}
}

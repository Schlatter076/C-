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
#include "cpu/cpu.h"
#include "battery/battery.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern unsigned char FIFO_Pop ( unsigned char * FIFO, unsigned char Len );
extern void Menu_Jump ( PSTMSG p, unsigned char Layer, unsigned char Num );
void Menu_Scroll ( PSTMSG p, unsigned char Top_Num );
//-----------------------------------------------------------------------------
// Menu Scroll Rountine
//-----------------------------------------------------------------------------
void Menu_Scroll ( PSTMSG p, unsigned char Top_Num )
{
	p->Menu.Update = NEED;
	if ( p->Key.FIFO[0] == (ON<<KEY_STOP))
		Menu_Jump ( p, FUNCTION_LAYER - ONE_LAYER, p->Menu.Class );
	else if ( p->Key.FIFO[0] == ((ON<<KEY_ENTER)|(ON<<KEY_CON)))
		Menu_Jump ( p, FUNCTION_LAYER + ONE_LAYER, CUR_NUM );
	else if ( p->Key.FIFO[0] == (ON<<KEY_ENTER))
	{
		if (( p->Batt.Mode == MEASRUEMAN )||( p->Batt.Mode = AUTO_BALANCE ))
			Menu_Jump ( p, FUNCTION_LAYER + ONE_LAYER, CUR_NUM  );
		if ( p->Menu.Seq_End != ON ) p->Menu.Sub++;
		else p->Menu.Sub = 0;
	}
	else if ( p->Key.FIFO[0] == (ON<<KEY_INC))
	{
		if ( p->Menu.Sub != 0 ) p->Menu.Seq_Inc = ON;
		else if ( p->Menu.Num_Top != ON ) 
			 Menu_Jump ( p, CUR_LAYER, p->Menu.Num +1 );
		else Menu_Jump ( p, CUR_LAYER, BOTTOM_NUM );
	}
	else if ( p->Key.FIFO[0] == (ON<<KEY_DEC))
	{
		if ( p->Menu.Sub != 0 ) p->Menu.Seq_Dec = ON;
		else if ( p->Menu.Num != BOTTOM_NUM ) 
			 Menu_Jump ( p, CUR_LAYER, p->Menu.Num -1 );
		else Menu_Jump ( p, CUR_LAYER, Top_Num );
	}
	else p->Menu.Update = UNNEED;
	p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
}

//-----------------------------------------------------------------------------
// key.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "main/main.h"
#include "misc/general.h"
#include "cpu/cpu.h"
#include "cpu/key.h"
//-----------------------------------------------------------------------------
// Key Constant Definitions
//-----------------------------------------------------------------------------
#define KEY_JITTER    		4
#define KEY_BEEPER			2
#define KEY_CONSTANT		10
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
unsigned char Get_Key_State ( PSTKEY p );
void Get_Key_Code ( PSTKEY p, unsigned char Beep );
//-----------------------------------------------------------------------------
// Get Key Code Rountine
//-----------------------------------------------------------------------------
unsigned char Get_Key_State ( PSTKEY p )
{
	unsigned char rtn = SUCCESS;
	unsigned char Current = KEY_PIN & KEY_MASK;

	if ( Current == p->State.Previous )
	{
		if ( ++p->State.Counter >= KEY_JITTER ) p->State.Counter = 0;
		else rtn = FAILURE;
	}
	else
	{
		p->State.Previous = Current;
		p->State.Counter = 0;
		rtn = FAILURE;
	}
	return rtn;
}
//-----------------------------------------------------------------------------
// Get Key Operate Rountine
//-----------------------------------------------------------------------------
void Get_Key_Code ( PSTKEY p, unsigned char Beep )
{
	unsigned char Pressed;
	//unsigned char Released;
	unsigned char n = p->n;
	unsigned char Current  = 0;
	unsigned char Previous = 0;
	unsigned char Counter  = 0;
	unsigned char Beeper   = 0;

	if ( Beeper ) Beeper--;
	else BELL_Off;
	if ( !Get_Key_State ( p )) return;
	Current  = p->State.Previous;
	Previous = p->Code.Previous;
	Counter  = p->Code.Counter;
	Beeper   = p->Code.Beeper;
	// Released = ( Current ^ Previous ) & Current;
	if (( Pressed = ( Current ^ Previous ) & ( ~Current )))
		if ( n < KEY_FIFO_LENGTH ) p->FIFO[n] = Pressed;
	if (( Current != KEY_MASK ) && ( Current == Previous ))
	{
		if ( Counter < KEY_CONSTANT ) ++Counter;
		else if (( Counter == KEY_CONSTANT )&&( p->FIFO[n] == Current )) 
			p->FIFO[n] |= 1<<KEY_CON;
		else;
	}
	else Counter = 0;
	if ( Pressed && ( Beep == OPEN ))
	{
		Beeper = KEY_BEEPER;
		BELL_On;
	}
	if ( Pressed ) n++;
	p->n = n;
	p->Code.Previous = Current;
	p->Code.Counter  = Counter;
	p->Code.Beeper   = Beeper;
}



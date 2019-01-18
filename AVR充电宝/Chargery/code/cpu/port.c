//-----------------------------------------------------------------------------
// port.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cpu/cpu.h"
#include "main/main.h"
#include "misc/general.h"
#include "main/configure.h"
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Delay        ( long t );
void Speaker_Beep ( void );
//-----------------------------------------------------------------------------
// Delay Routine
//-----------------------------------------------------------------------------
void Delay ( long t )
{
	while ( t-- )  if (( t & 0x0ff ) == 0x00 ) Clear_Dog;
}
//-----------------------------------------------------------------------------
// Routine
//-----------------------------------------------------------------------------



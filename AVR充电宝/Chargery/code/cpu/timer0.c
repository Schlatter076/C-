//-----------------------------------------------------------------------------
// Timer0.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "cpu/cpu.h"
//-----------------------------------------------------------------------------
// Constants Definitions
//-----------------------------------------------------------------------------
#define TIMER0_VALUE		((char)(-CPU_CLK/64/1000))
#define TIMER0_CLK64		((1<<CS01)|(1<<CS00))
#define COUNT_2MS			2
#define COUNT_20MS			10
#define COUNT_100MS			5
#define COUNT_500MS			5
#define COUNT_1SEC			2
#define COUNT_3SEC			3
#define COUNT_1MIN			20
#define TASK0_COUNT			COUNT_2MS
#define TASK1_COUNT			COUNT_20MS
#define TASK2_COUNT			COUNT_100MS
#define TASK3_COUNT			COUNT_500MS
#define TASK4_COUNT			COUNT_1SEC
#define TASK5_COUNT			COUNT_3SEC
#define TASK6_COUNT			COUNT_1MIN
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void Get_Key_Code ( void );
Timer0_ISR ( TIMER0_OVF );
void Timer0_Initial ( void );
void Delay_ms ( char N );
//-----------------------------------------------------------------------------
// Initial Rountine
//-----------------------------------------------------------------------------
void Timer0_Initial ( void )
{
   TCNT0 = TIMER0_VALUE;
   TCCR0 = ((1<<CS01)|(1<<CS00));
   TIMSK = (1<<TOIE0);
}
//-----------------------------------------------------------------------------
// Timer0 ISR Rountine
//-----------------------------------------------------------------------------
Timer0_ISR ( TIMER0_OVF )
{
    static char Task0 = TASK0_COUNT;
    static char Task1 = TASK1_COUNT;
    static char Task2 = TASK2_COUNT;
    static char Task3 = TASK3_COUNT;
    static char Task4 = TASK4_COUNT;
    static char Task5 = TASK5_COUNT;
    static char Task6 = TASK5_COUNT;

    TCNT0 = TIMER0_VALUE;
	if ( msg.Count_ms ) msg.Count_ms--;
	if ( Task0 ) Task0--;
	else 
	{
		Task0 = TASK0_COUNT;
		msg.bFlag.Task0 = ON;
		if ( Task1 ) Task1--;
		else
		{
			Task1 = TASK1_COUNT;
			msg.bFlag.Task1 = ON;
			if ( Task2 ) Task2--;
			else
			{
				Task2 = TASK2_COUNT;	 
				msg.bFlag.Task2 = ON;
				if ( Task3 ) Task3--;
				else
				{
					Task3 = TASK3_COUNT;	 
					msg.bFlag.Task3 = ON;
					if ( Task4 ) Task4--;
					else
					{
						Task4 = TASK4_COUNT;	 
						msg.bFlag.Task4 = ON;
						if ( Task5 ) Task5--;
						else
						{
							Task5 = TASK5_COUNT;	 
							msg.bFlag.Task5 = ON;
							if ( Task6 ) Task6--;
							else
							{
								Task6 = TASK6_COUNT;	 
								msg.bFlag.Task6 = ON;
							}
						}
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
// Delay N ms Rountine
//-----------------------------------------------------------------------------
void Delay_ms ( char N )
{
	msg.Count_ms = N;
	while ( msg.Count_ms );
}


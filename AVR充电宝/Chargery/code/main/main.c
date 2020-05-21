//-----------------------------------------------------------------------------
// Main.c
//-----------------------------------------------------------------------------
// Coypright By Chargery Tech., Co.
//
// Auth: ZhuXiaoGang
// Date: 10 May. 08
//
// Target: Avr Mega16
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "misc/misc.h"
#include "main/main.h"
#include "cpu/cpu.h"
#include "cpu/adc.h"
#include "cpu/key.h"
#include "lcd/lcd.h"
#include "menu/menu.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void LCD_Write_String ( unsigned char column, unsigned char line, char *str );
extern void Menu_Entry ( PSTMSG p );
extern void CPU_Initial ( void );
extern void LCD_Initial ( void );
extern void	Charger_Initial ( void );
extern void Charger_Operate ( PSTMSG p );
extern void Get_Key_Code ( PSTKEY p, unsigned char Beep );
extern void Deal_Key_FIFO ( PSTMSG p );
extern void ADC_Polling ( PSTADC p, unsigned char direct );
extern void PWM_Control ( PSTPWM p, unsigned int End_Out, unsigned char Step );
//-----------------------------------------------------------------------------
// Main Rountine
//-----------------------------------------------------------------------------
int main ( void )
{
	memcset ((char *) &msg, 0, sizeof (STMSG));

	CPU_Initial ();	

	LCD_Initial ();

	Charger_Initial ();

	while ( 1 )
	{
		if ( msg.bFlag.Task0 )
		{
			msg.bFlag.Task0 = OFF;
			ADC_Polling ((PSTADC) &msg.Adc, msg.Run.Status.Charge );
		}
		if ( msg.bFlag.Task1 )
		{
			msg.bFlag.Task1 = OFF;
			//RunLed_
			if ( msg.Run.Ctrl.Key == OPEN ) Get_Key_Code ((PSTKEY) &msg.Key, msg.Set.Key_Beep );						
		}
		if ( msg.bFlag.Task2 )
		{
			msg.bFlag.Task2 = OFF;
			PWM_Control ((PSTPWM) &msg.Pwm, 10, PWM_STEP_FAST );				
		}
		if ( msg.bFlag.Task3 )
		{
			msg.bFlag.Task3 = OFF;		
			Charger_Operate ((PSTMSG) &msg );
		}
		if ( msg.bFlag.Task4 )
		{
			msg.bFlag.Task4 = OFF;
			Menu_Entry ((PSTMSG) &msg );
			//RunLed_On;
		}
		if ( msg.bFlag.Task5 )
		{
			msg.bFlag.Task5 = OFF;

		}
		if ( msg.bFlag.Task5 )
		{
			msg.bFlag.Task5 = OFF;

		}
		//CPU_Idle;
		//SLEEP;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// dbus.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "cpu/cpu.h"
#include "lcd/lcd.h"
//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------
#define DBUS_AD4_7			((1<<DBUS_AD7)|(1<<DBUS_AD6)|(1<<DBUS_AD5)|(1<<DBUS_AD4))	
#define DBus_Output   		 {PORTB |= DBUS_AD4_7;DDRB  |= DBUS_AD4_7;}
#define DBus_Load(Byte)		 {PORTB &=~DBUS_AD4_7;PORTB |= (DBUS_AD4_7&Byte);}
#define DBus_Stable			 {NOP;NOP;NOP;NOP;NOP;NOP;}
#define DBus_Ctrl_Output	 {DDRB  |= (1<<DBus_EN)|(1<<DBus_RS);}
#define DBus_Enable	 		 {PORTB |= (1<<DBus_EN);}
#define DBus_DisEnable		 {PORTB &=~(1<<DBus_EN);}
#define DBus_Data	 		 {PORTB |= (1<<DBus_RS);}
#define DBus_Instruct	 	 {PORTB &=~(1<<DBus_RS);}
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void Delay ( long t );
void DBus_Initial ( void );
void DBus_Write_Enable ( void );
void DBus_Write_4Bit ( unsigned char dat );
void DBus_Write ( unsigned char flag, unsigned char byte );
//-----------------------------------------------------------------------------
// DBus Initial Rountine
//-----------------------------------------------------------------------------
void DBus_Initial ( void )
{
	DBus_DisEnable;
	DBus_Ctrl_Output;
	DBus_Output;
}
//-----------------------------------------------------------------------------
// DBus Write Enable Rountine
//-----------------------------------------------------------------------------
void DBus_Write_Enable ( void )
{
	DBus_Enable;
	Delay ( LCD_EN_SPACE_TIME );
	DBus_DisEnable;	
	DBus_Stable;	
}
//-----------------------------------------------------------------------------
// DBus Write 4Bit Rountine
//-----------------------------------------------------------------------------
void DBus_Write_4Bit ( unsigned char dat )
{
	DBus_Load ( dat );
	DBus_Write_Enable ();
}
//-----------------------------------------------------------------------------
// DBus Write Data or Instruct Rountine
//-----------------------------------------------------------------------------
void DBus_Write ( unsigned char flag, unsigned char byte )
{
	Delay ( LCD_WRITE_SPACE_TIME );
	if ( flag & DATA ) { DBus_Data; }
	else 			   { DBus_Instruct; }
	DBus_Write_4Bit ( byte );
	if (!( flag & BITS_4 )) DBus_Write_4Bit ( byte << 4 );
}

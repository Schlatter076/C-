//-----------------------------------------------------------------------------
// lcd.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "lcd/lcd.h"
#include "cpu/cpu.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern void Delay     ( long t );
extern void Delay_ms  ( char N );
extern void DBus_Write( unsigned char flag, unsigned char byte );
extern void DBus_Write_Enable ( void );
void LCD_Initial      ( void );
void LCD_Set_Position ( unsigned char col, unsigned char row );
void Lcd_Update ( unsigned char col, unsigned char row, char *str );
void LCD_Clear_Display ( void );
//-----------------------------------------------------------------------------
// LCD Initial Rountine
//-----------------------------------------------------------------------------
void LCD_Initial ( void )
{
	Delay_ms ( LCD_POWERON_WAIT_TIME );   
    DBus_Write ( INSTRUCT, DATA_4BIT | LINE2_MODE );
	Delay ( LCD_INSTRUCT_WAIT_TIME );
	//DBus_Write_Enable ();
	Delay ( LCD_INSTRUCT_WAIT_TIME );
    DBus_Write ( INSTRUCT, DATA_4BIT | LINE2_MODE );
	Delay ( LCD_INSTRUCT_WAIT_TIME );
    DBus_Write ( INSTRUCT, DISPLAY_ON );
	Delay ( LCD_INSTRUCT_WAIT_TIME );
    DBus_Write ( INSTRUCT, DISPLAY_CLEAR );
	Delay_ms ( LCD_INSTRUCT_DEAL_TIME );
    DBus_Write ( INSTRUCT, CHAR_INC );
	Delay_ms ( LCD_INSTRUCT_DEAL_TIME );
	LCD_BackLight_OFF;
}
//-----------------------------------------------------------------------------
// LCD Write Instruct Rountine
//-----------------------------------------------------------------------------
void LCD_Set_Position ( unsigned char col, unsigned char row )
{
    if ( row == ROW1 ) 
		DBus_Write ( INSTRUCT, ROW1_REG0 + col );
    else if ( row == ROW2 )
		DBus_Write ( INSTRUCT, ROW2_REG0 + col );
}
//-----------------------------------------------------------------------------
// LCD Update Rountine
//-----------------------------------------------------------------------------
void LCD_Update ( unsigned char col, unsigned char row, char* str )
{
	unsigned char n = 0;

    LCD_Set_Position ( col, row );    
    while ( * str ) 
	{
		DBus_Write ( DATA, *str++ );
		if ( ++n == COLUMN_MAX ) break;
	}
}
//-----------------------------------------------------------------------------
// LCD Clear Display Rountine
//-----------------------------------------------------------------------------
void LCD_Clear_Display ( void )
{
	DBus_Write ( INSTRUCT, DISPLAY_CLEAR );
	Delay_ms ( LCD_INSTRUCT_DEAL_TIME );
}

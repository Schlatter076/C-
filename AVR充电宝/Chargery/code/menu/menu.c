//-----------------------------------------------------------------------------
// menu.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "cpu/cpu.h"
#include "cpu/key.h"
#include "misc/misc.h"
#include "menu/menu.h"
#include "lcd/lcd.h"
#include "battery/battery.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern unsigned char FIFO_Clear ( unsigned char * FIFO, unsigned char Len );
extern unsigned char FIFO_Pop ( unsigned char * FIFO, unsigned char Len );
extern void LCD_Update ( unsigned char col, unsigned char row, char *str );
extern char * Get_Item ( PSTMSG p, char * str, unsigned char Type );
extern char Battery_Set_Save ( void );
extern char Load_Battery_Set ( void );
extern void Battery_Set_String ( char * p );
extern void Li_Battery_Function ( PSTMSG P );
extern void Ni_Battery_Function ( PSTMSG P );
extern void Pb_Battery_Function ( PSTMSG P );
extern void PMU_Out ( PSTMSG p );
void Menu_Entry ( PSTMSG p );
void Menu_Start ( PSTMSG p );
void Menu_Class ( PSTMSG p );
void Menu_Jump  ( PSTMSG p, unsigned char Layer, unsigned char Num );
//-----------------------------------------------------------------------------
// Menu Entry Rountine
//-----------------------------------------------------------------------------
void Menu_Entry ( PSTMSG p )
{
	if  ( p->Menu.Delay-- ) return ;
	if (( p->Key.n == KEY_NONE ) && ( p->Menu.Update == UNNEED )) return ;	else ;
	switch ( p->Menu.Layer )
	{
		case START_LAYER:
			Menu_Start ( p );						break;
		case BOTTOM_LAYER:
			Menu_Class ( p );						break;
		case FUNCTION_LAYER:
			switch ( p->Menu.Class )
			{
				case 1:
					Li_Battery_Function ( p );		break;
				case NI_BATT_PROCESS:
					Ni_Battery_Function ( p );		break;
				case PB_BATT_PROCESS:
					Pb_Battery_Function ( p );		break;
				case UD_BATT_PROCESS:
					Li_Battery_Function ( p );		break;
				case SETTING_PARAMETE:
					//Charger_Parameter_Set ();		break;
				case SETTING_RESUME:
					//Charger_Parameter_Resume ();	break;
				case BATT_SET_SAVE:
					//Battery_Set_Save ();			break;
				case BATT_SET_LOAD:
					//Battery_Set_Load ();			break;
				case DCPOWER_SET:					
					PMU_Out ( p );					break;
				default:							break;
			}										break;
		default:									break;
	}
}
//-----------------------------------------------------------------------------
// Menu Start Rountine
//-----------------------------------------------------------------------------
void Menu_Start ( PSTMSG p )
{
	switch ( p->Menu.Layer )
	{
		case START_LAYER:
			p->Menu.Delay = START_DELAY;
			if ( p->Key.n && p->Menu.Key == OPEN ) 
			{
				if ( p->Key.FIFO[0] == ((ON<<KEY_STOP)|(ON<<KEY_CON))) 
					p->Menu.Num = 2;
				else if ( p->Key.FIFO[0] == ((ON<<KEY_STOP)|(ON<<KEY_ENTER)|(ON<<KEY_CON)))
					p->Menu.Num = 3;
				else;
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
			}					
			switch ( p->Menu.Num )
			{
				case 0:
					LCD_Update ( COL1, ROW1, "SYSTEM CHECK" );
					LCD_Update ( COL1, ROW2, "PLEASE WAIT..." );
					p->Menu.Num++;
					break;
				case 1:					
					LCD_Update ( COL1, ROW1, "V3.01    AA" );
					LCD_Update ( COL1, ROW2, "1010B" );
					p->Menu.Num = END_NUM;
					p->Menu.Key = OPEN;
					break;
				case 2:
					//Menu_System_Check ();
					p->Menu.Delay = MENU_DELAY1;
					p->Menu.Num = END_NUM;
					break;
				case 3:
					//Default_Set_Resume ();
					p->Menu.Delay = MENU_NODELAY;
					p->Menu.Num = END_NUM;
					break;
				default:
					Menu_Jump ( p, BOTTOM_LAYER, BOTTOM_NUM );
					break;
			}
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------
// Menu Class Rountine
//-----------------------------------------------------------------------------
void Menu_Class ( PSTMSG p )
{
	switch ( p->Menu.Layer )
	{
		case BOTTOM_LAYER:
			if ( p->Menu.Key == OPEN )
			{
				p->Menu.Update = NEED;
				if ( p->Key.FIFO[0] == (ON<<KEY_ENTER))
					Menu_Jump ( p, BOTTOM_LAYER + ONE_LAYER, BOTTOM_NUM );
				else if ( p->Key.FIFO[0] == (ON<<KEY_INC))
				{
					if ( p->Menu.Num_Top != ON ) p->Menu.Num += 1;
					else { p->Menu.Num = BOTTOM_NUM; p->Menu.Num_Top = OFF; }
				}
				else if ( p->Key.FIFO[0] == (ON<<KEY_DEC))
				{
					if ( p->Menu.Num != BOTTOM_NUM ) p->Menu.Num -= 1;
					else { p->Menu.Num = 12; p->Menu.Num_Top = ON; }
				}
				else p->Menu.Update = UNNEED;
				p->Key.n = FIFO_Pop ((unsigned char *) p->Key.FIFO, p->Key.n );
				if ( p->Menu.Update == UNNEED ) { p->Menu.Update = NEED;  break; }
			} 
			else;
			p->Menu.Update = UNNEED;
			p->Menu.Key	  = OPEN;
			switch ( p->Menu.Num )
			{
				case BOTTOM_NUM:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "LiPo Battery" );
					p->Batt.Type = LIPO;
					p->Menu.Class = 1;
					break;
				case 2:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "LiIo Battery" );
					p->Batt.Type = LIIO;
					p->Menu.Class = 2;
					break;
				case 3:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "LiFe Battery" );
					p->Batt.Type = LIFE;
					p->Menu.Class = 3;
					break;
				case 4:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "NiMH Battery" );
					p->Batt.Type = NIMH;
					p->Menu.Class = 4;
					break;
				case 5:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "NiCd Battery" );
					p->Batt.Type = NICD;
					p->Menu.Class = 5;
					break;
				case 6:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "Pb Battery" );
					p->Batt.Type = PB;
					p->Menu.Class = 6;
					break;
				case 7:
					LCD_Update ( COL1, ROW1, "BATTERY TYPE" );
					LCD_Update ( COL1, ROW2, "BaUd Battery" );
					p->Menu.Class = 7;
					break;
				case 8:
					LCD_Update ( COL1, ROW1, "PROGRAM SETTING" );
					LCD_Update ( COL1, ROW2, "Advanced Setting" );
					p->Menu.Class = 8;
					break;
				case 9:
					LCD_Update ( COL1, ROW1, "PROGRAM SETTING" );
					LCD_Update ( COL1, ROW2, "Resume Default" );
					p->Menu.Class = 9;
					break;
				case 10:
					LCD_Update ( COL1, ROW1, "PROGRAM SETTING" );
					LCD_Update ( COL1, ROW2, "Bat Set and Save" );
					p->Menu.Class = 10;
					break;
				case 11:
					LCD_Update ( COL1, ROW1, "PROGRAM SETTING" );
					LCD_Update ( COL1, ROW2, "Load Battery Set" );
					p->Menu.Class = 11;
					break;
				case 12:
					PMU_Out ( p );
					p->Menu.Class = 12;
					p->Menu.Num_Top = ON;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------
// Menu Jump Rountine
//-----------------------------------------------------------------------------
void Menu_Jump ( PSTMSG p, unsigned char Layer, unsigned char Num )
{
	p->Menu.Num_Top = OFF;
	p->Menu.Seq_End = OFF;
	p->Menu.Seq_Dec = OFF;
	p->Menu.Seq_Inc = OFF;
	p->Menu.Toggle  = OFF;
	p->Menu.Sub = CLEAR;
	if ( Num != CUR_NUM ) 
	{
		p->Menu.Num = Num;
		p->Menu.Update = NEED;
	}
	if ( Layer != CUR_LAYER )
	{
		p->Menu.Layer = Layer;
		p->Menu.Update = UNNEED;
		p->Menu.Key = CLOSE;
		p->Key.n = FIFO_Clear ((unsigned char *) p->Key.FIFO, p->Key.n );
	}
}

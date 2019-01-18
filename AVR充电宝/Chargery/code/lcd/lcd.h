#ifndef _LCD_H
#define _LCD_H

//-----------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------
#define LCD_POWERON_WAIT_TIME			200
#define LCD_INSTRUCT_WAIT_TIME			400
#define LCD_INSTRUCT_DEAL_TIME			2
#define LCD_WRITE_SPACE_TIME			400
#define LCD_EN_SPACE_TIME				20
//-----------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------
#define COLUMN_MAX			16
#define DATA				1
#define INSTRUCT			0
#define BITS_4   			2
#define BITS_8   	     	0
#define ROW1				0
#define ROW2				1
#define COL1				0
#define COL3				2
#define COL4				3
#define COL5				4
#define COL6				5
#define COL7				6
#define COL10				9
#define COL16				15
//-----------------------------------------------------------------------
// Instruct Constant Definitions
//-----------------------------------------------------------------------
#define DATA_4BIT			0x20
#define LINE2_MODE			0x08
#define DISPLAY_ON			0x0C
#define CURSOR_ON			0x0A
#define BLINK_ON			0x09
#define DISPLAY_CLEAR		0x01
#define CHAR_INC			0x06
//-----------------------------------------------------------------------
// Register Definitions
//-----------------------------------------------------------------------
#define ROW1_REG0			0x80
#define ROW1_REG16			0x8F
#define ROW2_REG0			0xC0
#define ROW2_REG16			0xCF

#endif

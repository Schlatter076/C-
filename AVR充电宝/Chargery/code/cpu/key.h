#ifndef _KEY_H
#define _KEY_H

//-----------------------------------------------------------------------------
// Key Constant Definitions
//-----------------------------------------------------------------------------
#define KEY_MASK	    	((1<<KEY_STOP)|(1<<KEY_INC)|(1<<KEY_DEC)|(1<<KEY_ENTER))
#define KEY_PIN	    		PINC
#define KEY_CON		 		4
#define KEY_SET		 		8
#define KEY_FIFO_LENGTH		10
#define KEY_NONE			0
//-----------------------------------------------------------------------------
// Struct Definitions
//-----------------------------------------------------------------------------
typedef struct _STKEY
{
	unsigned char FIFO[KEY_FIFO_LENGTH];
	unsigned char n;		

	struct 
	{
		unsigned char Previous;
		unsigned char Counter;

	} State;

	struct
	{
		unsigned char Previous;
		unsigned char Counter;
		unsigned char Beeper;

	} Code;

} STKEY, *PSTKEY;

#endif

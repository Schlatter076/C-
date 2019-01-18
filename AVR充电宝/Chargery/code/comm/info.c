//-----------------------------------------------------------------------------
// Info.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "config/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "info/detect/detect.h"
#include "info/display/display.h"
#include "info/error.h"
//-----------------------------------------------------------------------------
// Variables Definitions
//-----------------------------------------------------------------------------
extern STMSG msg;
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern char Detect_Info_Deal ( char * buf );
extern char Display_Data_Deal ( char * ptr );
extern char Rs485_Send ( char * buf, char n, char timeout );
extern char Uart0_Wait_Receive ( char * buf, unsigned char n, char timeout );
extern void Voice_Table_Index ( char * buf );
char Info_Receive ( char Data );
char Info_Deal ( void );
char Check_Sum ( char * buf, unsigned char n );
//-----------------------------------------------------------------------------
// Info Receive Rountine
//-----------------------------------------------------------------------------
char Info_Receive ( char Data )
{
	msg.Recv.TimeOut = CLEAR;
	msg.Recv.State   = CLEAR;

	switch ( msg.Recv.Buf[MSG_ADDRESS_INDEX] = Data )
	{
		case DETECT_ADDRESS:
			if ( Uart0_Wait_Receive ( msg.Recv.Buf + 1, DETECT_INFO_LENGTH - 1, DETECT_RECV_TIMEOUT ) == FALSE)
			{
				msg.Recv.TimeOut = TRUE;
				return FALSE;
			}
			break;

		case DISPLAY_ADDRESS:
			break;
		
		default:
			return msg.Recv.State = MSG_ADDRESS_ERROR;
	}
	#ifdef RS485_CHECKSUM
    if ( msg.Recv.Buf[msg.Recv_n - 1] != Check_Sum ( msg.Recv.Buf, msg.Recv.Num ))
		return msg.Recv.State = MSG_CHECKSUM_EEROR; 
	#endif
	#ifdef INFO_DEAL_INSTANT
	Info_Deal ();
	#else
	msg.bFlag.Task3 = ON;
	#endif
	return msg.Recv.State = TRUE;
}
//-----------------------------------------------------------------------------
// Info Deal Rountine
//-----------------------------------------------------------------------------
char Info_Deal ( void )
{
	msg.Send.TimeOut = CLEAR;
	msg.Send.State   = CLEAR;
	switch ( msg.Recv.Buf[MSG_ADDRESS_INDEX] )
	{
		case DETECT_ADDRESS:
			#ifdef TEST_RS485_PROTOCOL
			Rs485_Send ( msg.Recv.Buf, DETECT_INFO_LENGTH, DETECT_RECV_PERIOD - DETECT_TIMEOUT_10MS );
			#endif
			//Voice_Table_Index ( msg.Recv.Buf );
			break;
		
		case DISPLAY_ADDRESS:
			break;
 	
		default:
			break;
	}
	return msg.Send.State = TRUE;
}
//-----------------------------------------------------------------------------
// Info Buffer Check Sum Routine
//-----------------------------------------------------------------------------
char Check_Sum ( char * buf, unsigned char n )
{
	unsigned char sum = 0;

	while ( n-- ) sum += *buf++;
	return (~sum + 1);
}

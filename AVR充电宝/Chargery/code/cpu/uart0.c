//-----------------------------------------------------------------------------
// Uart0.c
//-----------------------------------------------------------------------------
#define UART0_8BIT_2STOP
#ifdef  UART0_9BIT_1STOP
//#define UART0_CHECK_ADDRESSBIT
#endif
//#define UART0_CHECK_FRAME
#ifndef UART0_CHECK_ADDRESSBIT
#define UART0_MUTLCPU_MODE
#endif
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
#define UBRR0_RECV_VALUE  		(CPU_CLK/16/28800L-1)
#define UBRR0_SEND_VALUE  		(CPU_CLK/16/28800L-1)
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
void Uart0_Initial ( void );
void Uart0_Receive_Baud ( char sel );
char Uart0_Send ( char * buf, unsigned char n, char timeout );
char Uart0_Wait_Receive ( char * buf, unsigned char n, char timeout );
Uart0_ISR(RECV);
//-----------------------------------------------------------------------------
// Initial Rountine
//-----------------------------------------------------------------------------
void Uart0_Initial ( void )
{
    UCSRA = (1<<TXC)|(1<<UDRE)|(0<<U2X);
    UCSRB = (1<<RXCIE)|(0<<TXCIE)|(1<<RXEN)|(0<<TXEN);
	#if defined(UART0_9BIT_2STOP)
    UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB |= (1<<UCSZ2);
	#elif defined(UART0_9BIT_1STOP)
    UCSRC = (1<<URSEL)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB |= (1<<UCSZ2);
	#elif defined(UART0_8BIT_2STOP)
    UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	#else
    UCSRC = (1<<URSEL)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	#endif
}
//-----------------------------------------------------------------------------
// Uart0 Set Receive Baud Rountine
//-----------------------------------------------------------------------------
void Uart0_Receive_Baud ( char sel )
{
	if ( sel )
	{
		UBRRH = (char)(UBRR0_RECV_VALUE >> 8) & (~(1<<URSEL));
		UBRRL = (char) UBRR0_RECV_VALUE;
		#ifdef  UART0_MUTLCPU_MODE
		UCSRA |= (1<<MPCM);
		#endif
		UCSRB |= (1<<RXCIE)|(1<<RXEN);
		UCSRB &=~(1<<TXEN);
	}
	else
	{
		UBRRH = (char)(UBRR0_SEND_VALUE >> 8) & (~(1<<URSEL));
		UBRRL = (char) UBRR0_SEND_VALUE;
		UCSRB &=~((1<<RXCIE)|(1<<RXEN));
		UCSRB |= (1<<TXEN);			 
	}
}
//-----------------------------------------------------------------------------
// Uart0 Receive ISR Rountine
//-----------------------------------------------------------------------------
Uart0_ISR(RECV)
{
	unsigned char Status, Data;
	Status = UCSRB;
	Data = UDR;
	#ifdef UART0_CHECK_FRAME
	unsigned char temp;
	if ((temp = UCSRA & ((1<<FE)|(1<<DOR)))) return;
	#endif
	#ifdef UART0_CHECK_ADDRESSBIT
	if (!( Status & (1<<RXB8))) return;
	#endif
	// Info_Receive ( Data );
	#ifdef  UART0_MUTLCPU_MODE
	UCSRA |= (1<<MPCM);
	#endif
	UCSRB |= (1<<RXCIE)|(1<<RXEN);
}
//-----------------------------------------------------------------------------
// Uart0 Wait Receive Rountine
//-----------------------------------------------------------------------------
char Uart0_Wait_Receive ( char * buf, unsigned char n, char timeout )
{
	#ifdef UART0_CHECK_FRAME
	unsigned char temp;
	#endif
	#ifdef  UART0_MUTLCPU_MODE
	UCSRA &=~(1<<MPCM);
	UCSRB &=~(1<<RXCIE);
	#endif
	msg.Count_ms = timeout;
	while (1) {
		if ( UCSRA & (1<<RXC) )
		{
			*buf++ = UDR;
			#ifdef UART0_CHECK_FRAME
 			if ((temp = UCSRA & ((1<<FE)|(1<<DOR)))) return FALSE;
			#endif
			if ( --n == ZERO ) break;
		}
 		if ( msg.Count_ms == ZERO ) return FALSE;
	}
	UCSRB &=~(1<<RXEN);
	return TRUE;
}
//-----------------------------------------------------------------------------
// Uart0 Send Rountine
//-----------------------------------------------------------------------------
char Uart0_Send ( char * buf, unsigned char n, char timeout )
{
	msg.Count_ms = timeout;
    do {
	    while (!( UCSRA & (1<<UDRE)));
		UCSRA |= (1<<TXC);
		#ifdef RS485_LOOP_SEND
		UDR = n;
		#else
		UDR = *buf++;
		#endif
 		if ( msg.Count_ms == ZERO ) return FALSE;
	} while ( --n );
	while (!( UCSRA & (1<<TXC)));
	return TRUE;
}



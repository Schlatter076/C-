#ifndef _CONFIGURE_H
#define _CONFIGURE_H
	
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define GCC_BUILD
//-----------------------------------------------------------------------------
// Configure Definitions
//-----------------------------------------------------------------------------
#ifdef GCC_BUILD
	#include <avr\io.h>
	#include <avr\signal.h>
	#define  Timer0_ISR(OVF)    SIGNAL(SIG_OVERFLOW0)
	#define  Timer1_ISR(OVF)    SIGNAL(SIG_OVERFLOW1)
	#define  Uart0_ISR(RECV)   	SIGNAL(SIG_UART_RECV)
	#define  ADC_ISR(COMPLETE)  SIGNAL(SIG_ADC)
	#define  CAN_ISR(INT0)      SIGNAL(SIG_INTERRUPT0)
	#define  ISD_ISR(INT1)      SIGNAL(SIG_INTERRUPT1)
	#define  SLEEP          	{asm("SLEEP");}
	#define  SEI            	{asm("SEI");}
	#define  CLI            	{asm("CLI");}
	#define  NOP            	{asm("nop");}
	#define  _FLASH         	__attribute__ ((progmem))
	#define  _READ_FLASH        {asm("LPM");}

#elif ICC_BUILD
#endif

#endif
 

#ifndef _CPU_H
#define _CPU_H

//-----------------------------------------------------------------------------
// CPU Constant Definitions
//-----------------------------------------------------------------------------
#define CPU_CLK				16000000L        // 2mHz => max uart rate 115200bps
//-----------------------------------------------------------------------------
// CPU Macro Definitions
//-----------------------------------------------------------------------------
#define Global_Interrupt_Enable       {SREG  |= (1<<SREG_I);}
#define Global_Interrupt_Disable      {SREG  &=~(1<<SREG_I);}
#define CPU_Sleep_Disable             {MCUCR &=~(1<<SE);}
#define CPU_Idle                      {MCUCR |= (1<<SE)|(0<<SM2)|(0<<SM1)|(0<<SM0);}
#define CPU_Power_Save                {MCUCR |= (1<<SE)|(0<<SM2)|(1<<SM1)|(1<<SM0);}
#define CPU_OSC_CALIBRATE             {OSCCAL = 0xB0;}
#define CPU_INT0_ENABLE				  {GICR  |=  (1 << INT0);}
#define CPU_INT1_ENABLE				  {GICR  |=  (1 << INT1);}
#define CPU_INT2_ENABLE				  {GICR  |=  (1 << INT2);}
#define CPU_UART0_RECV_OFF			  {UCSRB &=~((1<<RXCIE)|(1<<RXEN));}
#define CPU_UART0_RECV_ON			  {UCSRB |= ((1<<RXCIE)|(1<<RXEN));}
#define Clear_Dog
//-----------------------------------------------------------------------------
// Port Definitions
//-----------------------------------------------------------------------------
#define CURP				PA0
#define CURN				PA1
#define VOUTN				PA2
#define VOUTP				PA3
#define VT2				    PA4
#define DCIN				PA5
#define VBATT			    PA6
#define VT1				    PA7

#define DBus_RS				PB0
#define DBus_EN	 		   	PB2
#define DBUS_AD4			PB4
#define DBUS_AD5			PB5
#define DBUS_AD6			PB6
#define DBUS_AD7			PB7

#define FAN			        PB1
#define CHG			        PB3

#define BAL1				PD2
#define BAL2				PD3
#define BAL3				PC2
#define BAL4		        PC3
#define BAL5		        PC4
#define BAL6		        PC5

#define BELL				PD0
#define PWM1				PD4
#define PWM2				PD5
#define LCD_BL		        PD6
#define DSC			        PD7

#define KEY_STOP	    	PC7
#define KEY_INC				PC6
#define KEY_DEC				PC1
#define KEY_ENTER			PC0
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  MUX_CELL1			0
#define  MUX_CELL2			1
#define  MUX_CELL3			2
#define  MUX_CELL4			3
#define  MUX_CELL5			4
#define  MUX_CELL6			5
#define  MUX_VIN			6
//-----------------------------------------------------------------------------
// Port Macro Definitions
//-----------------------------------------------------------------------------
#define Port_PullUp_Disable			  {SFIOR |= (1<<PUD);}
#define Port_Initial                  {PORTD |= (1<<BELL);DDRD |= (1<<BELL);\
									   DDRD  |= (1<<PWM1);\
									   PORTC |= (1<<KEY_STOP)|(1<<KEY_INC)|(1<<KEY_DEC)|(1<<KEY_ENTER);\
									   DDRC &=~((1<<KEY_STOP)|(1<<KEY_INC)|(1<<KEY_DEC)|(1<<KEY_ENTER));}
#define BELL_On						  {PORTD &=~(1<<BELL);}
#define BELL_Off 	  				  {PORTD |= (1<<BELL);}
#define LCD_BackLight_ON	 		  {PORTD &=~(1<<LCD_BL);DDRD |= (1<<LCD_BL);}
#define LCD_BackLight_OFF	 		  {PORTD |= (1<<LCD_BL);DDRD |= (1<<LCD_BL);}

#endif

#ifndef _PWM_H
#define _PWM_H

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
// Waveform Generation Mode
#define PWM_Normal_Mode0			((0<<WGM13)|(0<<WGM12)|(0<<WGM11)|(0<<WGM10))
#define PWM_Phase_8bit_Mode1		((0<<WGM13)|(0<<WGM12)|(0<<WGM11)|(1<<WGM10))
#define PWM_Phase_9bit_Mode2		((0<<WGM13)|(0<<WGM12)|(1<<WGM11)|(0<<WGM10))
#define PWM_FAST_Mode14				((1<<WGM13)|(1<<WGM12)|(1<<WGM11)|(0<<WGM10))
#define PWM_FAST_Mode15				((1<<WGM13)|(1<<WGM12)|(1<<WGM11)|(1<<WGM10))
// Compare Output Mode
#define Normal_Port_Operation		((0<<COM1B1)|(0<<COM1B0))
#define Toggle_OCnA_CompareMatch	((0<<COM1B1)|(1<<COM1B0))
#define OC_ClearUp_SetDownCount	  	((1<<COM1B1)|(0<<COM1B0))
#define OC_SetDown_ClearUpCount	  	((1<<COM1B1)|(1<<COM1B0))
// Timer Clock Select
#define No_Clock_Source			  	((0<<CS12)|(0<<CS11)|(0<<CS10))
#define ClkIO_1_Prescale		  	((0<<CS12)|(0<<CS11)|(1<<CS10))
#define ClkIO_8_Prescale 		  	((0<<CS12)|(1<<CS11)|(0<<CS10))
#define ClkIO_64_Prescale 		  	((0<<CS12)|(1<<CS11)|(1<<CS10))
#define ClkIO_256_Prescale 		  	((1<<CS12)|(0<<CS11)|(0<<CS10))
#define ClkIO_1024_Prescale		  	((1<<CS12)|(0<<CS11)|(1<<CS10))
//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------
#define Timer1_Set(Mode)			{TCCR1A |= Mode & ((1<<WGM11)|(1<<WGM10));\
									 TCCR1B |= Mode & ((1<<WGM13)|(1<<WGM12));}
#define Timer1_Stop				    {TCCR1B &=~((1<<CS12)|(1<<CS11)|(1<<CS10));}
#define Timer1_Clock(Prescale)		{TCCR1B |= Prescale;}
#define Timer1_Count_Load(Word)		{TCNT1   = Word;}
#define Timer1_OutCompA_Load(Word)	{OCR1A   = Word;}
#define Timer1_OutCompB_Load(Word)	{OCR1B   = Word;}
#define Timer1_Clear_Overflow		{TIFR   |= (1<<TOV1);}
#define Timer1_Overflow_INT_EN		{TIMSK  |= (1<<TOIE1);}
#define Timer1_Overflow_INT_DIS    	{TIMSK  &=~(1<<TOIE1);}
#define TIMER1_OVERFLOW				(TIFR & (1<<TOV1))
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define	PWM_60KHZ			 266
#define PWM_FREQUENCE		 266
#define PWM_STOP			 266
#define PWM_VOLT_MAX		 100
#define PWM_STEP_MIN		 10
#define PWM_STEP_FAST		 1
#define PWM_STEP_CLASS1		 2
#define PWM_STEP_CLASS2		 2
#define PWM_STEP_CLASS3		 3
//-----------------------------------------------------------------------------
// Struct Definitions
//-----------------------------------------------------------------------------
typedef struct _STPWM
{
	unsigned int  In_Buff;
	unsigned int  Cur_Out;
	unsigned int  End_Out;
			
} STPWM,*PSTPWM;

#endif

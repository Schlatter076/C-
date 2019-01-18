//-----------------------------------------------------------------------------
// pwm.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "cpu/pwm.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
void PWM_Initial ( void );
void PWM_LoadBuf ( unsigned int t );
void PWM_Control ( PSTPWM p, unsigned int End_Out, unsigned char Step );
//-----------------------------------------------------------------------------
// Initial Rountine
//-----------------------------------------------------------------------------
void PWM_Initial ( void )
{
    Timer1_Count_Load ( PWM_STOP );
    Timer1_OutCompA_Load ( PWM_FREQUENCE );
    Timer1_OutCompB_Load ( PWM_FREQUENCE );
	Timer1_Set ( PWM_FAST_Mode15 );
	Timer1_Clock ( No_Clock_Source );                      
}
//-----------------------------------------------------------------------------
// PWM Update Rountine
//-----------------------------------------------------------------------------
void PWM_Update ( unsigned int t )
{
	if ( t < PWM_VOLT_MAX || t > PWM_FREQUENCE ) return;
	//while ( !TIMER1_OVERFLOW );
	Timer1_Clear_Overflow;
	Timer1_Count_Load ( t );
}
//-----------------------------------------------------------------------------
// PWM Control Rountine
//-----------------------------------------------------------------------------
void PWM_Control ( PSTPWM p, unsigned int End_Out, unsigned char Step )
{
	unsigned int t = 0;

	p->End_Out = End_Out;
	if ( End_Out == 0 )	t = PWM_STOP;
	else if ( End_Out > p->Cur_Out ) t += PWM_STEP_MIN * Step;
	else if ( End_Out < p->Cur_Out ) t -= PWM_STEP_MIN * Step;
	else return;
	PWM_Update ( t );
}


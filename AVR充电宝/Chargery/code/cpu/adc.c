//-----------------------------------------------------------------------------
// adc.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
#include "misc/general.h"
#include "main/main.h"
#include "cpu/adc.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
extern unsigned char Data_Average ( unsigned char * buf, unsigned char n );
void ADC_Initial ( void );
void ADC_Polling ( PSTADC p, unsigned char direct );
//-----------------------------------------------------------------------------
// ADC Initial Rountine
//-----------------------------------------------------------------------------
void ADC_Initial ( void )
{
	ADC_Set(ADC_Channel0);
	ADC_Auto_Trigger(ADC_Free_Running_Mode);
	ADC_Continue;
}
//-----------------------------------------------------------------------------
// ADC Conversion Complete Polling Rountine
//-----------------------------------------------------------------------------
void ADC_Polling ( PSTADC p, unsigned char direct )
{
	unsigned char ch = 0;
	unsigned char n = p->n;
	unsigned char Channel = p->Channel;
	unsigned int  Volt;

	if ( ADC_Is_Complete == FALSE ) return;
	ADC_Clear_Complete;
	if ( n++ == ADC_FIRST_CONVERSION ) return;
	p->Buf[n-1] = ADC_WORD_VAlUE;
	if ( n < ADC_BUF_LEN ) return;
	n = 0;

	if ( Channel == ADC_Channel0 )
		if ( direct == POSITIVE ) ch = ADC_DIFF1_ADC10;
		else ch = ADC_DIFF3_ADC01;
	else if ( Channel == ADC_CHANNEL_NUM - 1 ) ch = ADC_Channel0;
	else ch = Channel + ADC_DIFF_CHANNEL;
	ADC_Set ( ch );

	Volt = Data_Average ( p->Buf, ADC_BUF_LEN );
	p->Volt[Channel] = Volt / 4;
	if ( Volt & 0x0002 ) p->Volt[Channel]++;
	if ( ++Channel == ADC_CHANNEL_NUM ) Channel = ADC_Channel0;

	p->Channel = Channel;
	p->n = n;
}

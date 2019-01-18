#ifndef _ADC_H
#define _ADC_H

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  ADC_Aligh_Left		     (1 << ADLAR)
#define  ADC_Aligh_Right         (0 << ADLAR)

#define  ADC_Extern_AVREF   	 (0 << REFS1)|(0 << REFS0)
#define  ADC_Extern_AVCC    	 (0 << REFS1)|(1 << REFS0)
#define  ADC_Intern_VREF    	 (1 << REFS1)|(1 << REFS0)

#define  ADC_Channel_Bits  		 0x1f
#define  ADC_Channel0	     	 0
#define  ADC_Channel1	     	 1
#define  ADC_Channel2	     	 2
#define  ADC_Channel3	     	 3
#define  ADC_Channel4	     	 4
#define  ADC_Channel5	     	 5
#define  ADC_Channel6	     	 6
#define  ADC_Channel7	     	 7
#define  ADC_VBG     			 30
#define  ADC_GND     			 31
#define  ADC_DIFF1_ADC10		 9
#define  ADC_DIFF2_ADC10		 11
#define  ADC_DIFF3_ADC01		 16
#define  ADC_DIFF1_X10A			 10
#define  ADC_DIFF2_X200A		 200
#define  ADC_DIFF3_X1A			 1
#define  ADC_WORD_VAlUE			 ADCW
//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------
#define  ADC_Set(channel)        {{ADMUX  = ADC_Intern_VREF|ADC_Aligh_Right|(channel);}}
#define  ADC_Select(channel)	 {{ADMUX &= ~ADC_Channel_Bits;ADMUX |= channel;}}
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  ADC_Prescaler_Bits      0x07
#define  ADC_CLK2    			 (0 << ADPS2)|(0 << ADPS1)|(1 << ADPS0)
#define  ADC_CLK4    			 (0 << ADPS2)|(1 << ADPS1)|(0 << ADPS0)
#define  ADC_CLK8   			 (0 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)
#define  ADC_CLK16   			 (1 << ADPS2)|(0 << ADPS1)|(0 << ADPS0)
#define  ADC_CLK32   			 (1 << ADPS2)|(0 << ADPS1)|(1 << ADPS0)
#define  ADC_CLK64   			 (1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0)
#define  ADC_CLK128  			 (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)

#define  ADC_Trigger_Source_Bits 0xe0
#define  ADC_Free_Running_Mode	 (0 << ADTS2)|(0 << ADTS1)|(0 << ADTS0)
#define  ADC_Analog_Comparator	 (0 << ADTS2)|(0 << ADTS1)|(1 << ADTS0)
#define  ADC_INT0		    	 (0 << ADTS2)|(1 << ADTS1)|(0 << ADTS0)
#define  ADC_T0_Compare_Match	 (0 << ADTS2)|(1 << ADTS1)|(1 << ADTS0)
#define  ADC_T0_Overflow		 (1 << ADTS2)|(0 << ADTS1)|(0 << ADTS0)
#define  ADC_T1_Compare_Match_B	 (1 << ADTS2)|(0 << ADTS1)|(1 << ADTS0)
#define  ADC_T1_Overflow		 (1 << ADTS2)|(1 << ADTS1)|(0 << ADTS0)
#define  ADC_T1_Capture_Event    (1 << ADTS2)|(1 << ADTS1)|(1 << ADTS0)
//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------
#define  ADC_Auto_Trigger(Source)    {SFIOR	|= Source;}
//-----------------------------------------------------------------------------
// Macro Definitions
//-----------------------------------------------------------------------------
#define  ADC_Enable         	 {ADCSRA |=  (1 << ADEN);}
#define  ADC_Disable        	 {ADCSRA &= ~(1 << ADEN);}
#define  ADC_Start_Conversion    {ADCSRA |=  (1 << ADSC);}
#define  ADC_Stop_Conversion   	 {ADCSRA &= ~(1 << ADSC);}
#define  ADC_Auto_Trigger_Enable {ADCSRA |=  (1 << ADATE);}
#define  ADC_Interrupt_Enable    {ADCSRA |=  (1 << ADIE);}
#define  ADC_Single         	 {ADCSRA  =  (1 << ADEN)|(1 << ADSC)|ADC_CLK8;}
#define  ADC_Continue	         {ADCSRA  =  (1 << ADEN)|(1 << ADSC)|ADC_CLK8;}
#define  ADC_Continue_Int        {ADCSRA  =  (1 << ADEN)|(1 << ADSC)|(1 << ADIE)|ADC_CLK8;}
#define  ADC_Is_Complete		 (ADCSRA &   (1 << ADIF))
#define  ADC_Clear_Complete		 {ADCSRA |=  (1 << ADIF);}
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  ADC_BUF_LEN      		 10
#define  ADC_CHANNEL_NUM		 (6+SERIAL_MAX)
#define  ADC_DIFF_CHANNEL		 2
#define  ADC_FIRST_CONVERSION	 0
#define  ADC_VBG_VOLT			 122
#define  ADC_VREF_VOLT			 256
#define  ADC_FULL_SCALE			 1024
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  CELL_DIV				2
#define  I_DIV					50
#define  VOUT_DIV				12
#define  VIN_DIV				11
#define  DCIN_DIV				11
//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  I						0
#define  VOUTN					1
#define  VOUTP					2
#define  VT2					3
#define  VT1					4
#define  DCIN					4
#define  IN						5
#define  CELL(n)				(6+n)
//-----------------------------------------------------------------------------
// Struct Type Definitions
//-----------------------------------------------------------------------------
typedef struct _STADC
{
	unsigned char Buf[ADC_BUF_LEN];
	unsigned char n;

	unsigned char Volt[ADC_CHANNEL_NUM];
	unsigned char Channel;

} STADC,*PSTADC;

#endif

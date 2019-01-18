//-----------------------------------------------------------------------------
// misc.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main/configure.h"
//-----------------------------------------------------------------------------
// Function Prototype
//-----------------------------------------------------------------------------
void Bubble_Sort ( unsigned char * p, unsigned char n );
unsigned char  Data_Average ( unsigned char * buf, unsigned char n );
unsigned char FIFO_Clear ( unsigned char * FIFO, unsigned char Len );
unsigned char FIFO_Press_In ( unsigned char * FIFO, unsigned char Len, unsigned char Data );
unsigned char FIFO_Pop ( unsigned char * FIFO, unsigned char Len );
//-----------------------------------------------------------------------------
// Bubble Sort Rountine
//-----------------------------------------------------------------------------
void Bubble_Sort ( unsigned char * p, unsigned char n )
{
	unsigned char i   = 0;
	unsigned char buf = 0;

	while ( --n )
	{
	   for ( i = 0; i < n; i++)
	      if ( *( p + i) > *( p + i + 1 ))
		  {
		     buf = *( p + i );
			 *( p + i) = *( p + i + 1 );
			 *( p + i +1 ) = buf;
		  }
    }
}
//-----------------------------------------------------------------------------
// Math Data Average Rountine
//-----------------------------------------------------------------------------
unsigned char Data_Average ( unsigned char * buf, unsigned char n )
{
    unsigned char i   = 0;
    unsigned int  sum = 0;

	Bubble_Sort ( buf, n );
	buf[0] = buf[1];
	buf[n-1] = buf[n-2];
	for ( ; i < n; i++ ) sum += buf[i];	
	return (unsigned char)( sum / n );
}
//-----------------------------------------------------------------------------
// FIFO Clear Zero Rountine
//-----------------------------------------------------------------------------
unsigned char FIFO_Clear ( unsigned char * FIFO, unsigned char Len )
{
	while ( Len ) { FIFO[Len] = 0; Len--; } return Len;
}
//-----------------------------------------------------------------------------
// FIFO Press In Rountine
//-----------------------------------------------------------------------------
unsigned char FIFO_Press_In ( unsigned char * FIFO, unsigned char Len, unsigned char Data )
{
	FIFO[Len] = Data; return ++Len;
}
//-----------------------------------------------------------------------------
// FIFO Pop Out Rountine
//-----------------------------------------------------------------------------
unsigned char FIFO_Pop ( unsigned char * FIFO, unsigned char Len )
{
	unsigned char i = 0;

	if ( --Len == 0 ) FIFO[0] = 0;
	else 
		for ( ; i < Len; i++ ) 
		{
			FIFO[i] = FIFO[i + 1]; 
			FIFO[i + 1] = 0;
		}
	return Len;
}

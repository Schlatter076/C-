#ifndef _MISC_H_
#define _MISC_H_

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
extern void memcopy ( char * dest, char * sour, unsigned char len );
extern char memcomp ( char * buf1, char * buf2, unsigned char len );
extern void memcset ( char * buf, char c, unsigned char len);
extern char Buffer_is_Zero ( char * buf, char n );
extern char Buffer_XOR ( char * buf, char n );
extern char Buffer_Sum ( char * buf, char n );
extern long Array_Long ( char * p, char i );
extern long Hex_BCD (long ll, char n);
extern long BCD_Hex (long ll, char n);
extern char* Number_String ( char * p, char c );
extern char  String_Number ( char * p, char c );

#endif

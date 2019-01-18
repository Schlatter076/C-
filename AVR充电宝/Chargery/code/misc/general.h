#ifndef _GENERAL_H
#define _GENERAL_H

//-----------------------------------------------------------------------------
// Constant Definitions
//-----------------------------------------------------------------------------
#define  TRUE          1
#define  FALSE         0
#define  OK            1
#define  ERROR         0
#define  ON            1
#define  OFF           0
#define  HIGH          1
#define  LOW           0
#define  OPEN          1
#define  CLOSE         0
#define  STOP          1
#define  START         0
#define  ZERO          0
#define  CLEAR         0
#define  SUCCESS	   1
#define  FAILURE       0
#define  POSITIVE	   1
#define  NEGATIVE      0
#define  UNNEED		   1
#define  NEED		   0
#define  ACTIVE		   1
#define  DEACTIVE	   0
#define  NULL          (unsigned char)0
#define  MSB           0x80
#define  LSB           0x01
#define  CR            0x0D
#define  LF            0x0A
#define  SPACE         ' '
#define  ASCII_MASK    '0'
#define  WRITE         0x00
#define  READ          0x01
#define  BIT0          0
#define  BIT1          1
#define  BIT2          2
#define  BIT3          3
#define  BIT4          4
#define  BIT5          5
#define  BIT6          6
#define  BIT7          7
//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
#define  BYTE          unsigned char
#define  UCHAR         unsigned char
#define  UINT          unsigned int
#define  ULONG         unsigned long
#define  CHAR          char
#define  INT           int
#define  WORD          int
//-----------------------------------------------------------------------------
// Structure Definitions
//-----------------------------------------------------------------------------
typedef union _ULong {
   unsigned long Long;
   unsigned int  UInt[2];
   unsigned char Char[4];
} ULong;

typedef union _UInt {
   unsigned int  UInt;
   unsigned char Char[2];
} UInt;
//-----------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------
#define _toLower(c)    ((c)-'A'+'a' )
#define _toUpper(c)    ((c)-'a'+'A' )
#define _toAscii(c)    ((c) & 0x7F  )
#define _isDigit(c)	   ((c) >= '0' && (c) <= '9')

#define MSBYTE(word)   (BYTE)((WORD)(word) >> 8)
#define LSBYTE(word)   (BYTE)(WORD)(word)
#define MSHALF(char)   (BYTE)(((char) >> 4) & 0x0f)
#define LSHALF(char)   (BYTE)((char) & 0x0f)

#endif

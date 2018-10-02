/*
--------------------------------------------------------------------------------------------
\name   ECalLiUtils

\brief  ECALLI utility functions

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

/*
 * Generate the pattern (LED group) code from an input string containing an arbitrary 
 * combination of the N,S,E,W symbols. Output is single-byte field.
 * B0: North, B1: East, B2: South, B3: West, B4-7: unused
 * BO is the LSF bit. Any combination of bits can be set.
 */
unsigned char ECalLiPatternId(char * str)
{
  unsigned char code   = 0;

  unsigned char n_used = 0;
  unsigned char e_used = 0;
  unsigned char s_used = 0;
  unsigned char w_used = 0;

  if( (strchr(str,'N') || strchr(str,'n')) && !n_used ) { code +=  1;     }
  if( (strchr(str,'E') || strchr(str,'e')) && !e_used ) { code += (1<<1); }
  if( (strchr(str,'S') || strchr(str,'s')) && !s_used ) { code += (1<<2); }
  if( (strchr(str,'W') || strchr(str,'w')) && !w_used ) { code += (1<<3); }

  return code;
}

/*
 * Convert two bytes into an unsigned short integer
 */
unsigned short int ECalLiToUShort(unsigned char msbyte, unsigned char lsbyte)
{
  unsigned short int us = (msbyte << 8) + lsbyte;
  return us;
}

unsigned int ECalLiToUInt(unsigned short int msb, unsigned short int lsb)
{
  unsigned int i = ((unsigned int)msb << 16) + lsb;
  return i;
}

unsigned long int ECalLiToULong(unsigned short int msb, unsigned short int lsb)
{
  unsigned long int longint = ((unsigned long int)msb << 16) + lsb;
  return longint;
}

/*
 * Extract the most significant byte
 */
unsigned char ECalLiToMSByte(unsigned short int us)
{
  return ((us & k16bitMask_8MS) >> 8);
}

/*
 * Extract the least significant byte
 */
unsigned char ECalLiToLSByte(unsigned short int us)
{
  return (us & k16bitMask_8LS);
}

/*
 * Print a byte field
 */
void ECalLiPrintByteField16(unsigned char * mesg, const char * title)
{
  if(kPrintOutByteFields) {

     if(title) {
       printf("ECalLiPrintByteField16> %s \n", title);
     }

     int nw16=0;
     printf("ECalLiPrintByteField16> \n");
     printf("#    || 8-bit fields || 16-bit \n");
     printf("word ||  [MSB][LSB]  ||   word \n");
     printf("-------------------------------\n");
     for(int i=0; i<kMesgSize-1; i++) { 
         if(i%2==1) continue;
              printf("%4d ||  [%3d][%3d]  ||    %5d  \n", 
              nw16++,
              mesg[i], mesg[i+1],
              ECalLiToUShort(mesg[i], mesg[i+1]));
     }
  }
}


/*
--------------------------------------------------------------------------------------------
\name   ECalLiSystemReset

\brief  Reset LI card info 

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#include <string.h>

#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

void ECalLiSystemReset(void)
{
  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {

     strcpy(gLISystem.Card[icard].IPAddress, "\0");

     gLISystem.Card[icard].PortNum             =  0;
     gLISystem.Card[icard].Socket              = -1;
     gLISystem.Card[icard].IsConnected         =  0;
     gLISystem.Card[icard].IsActive            =  0;
     gLISystem.Card[icard].CurrFlashSeqSetting =  0;
     gLISystem.Card[icard].CurrNTrigInSetting  =  0;
     gLISystem.Card[icard].CurrAmplitude       =  0;
     gLISystem.Card[icard].CurrPattern         =  0;   
     gLISystem.Card[icard].CurrNTrigTotal      =  0;
  }
}

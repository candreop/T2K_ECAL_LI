/*
--------------------------------------------------------------------------------------------
\name   ECalLiSystemPrint

\brief  Print LI system info 

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>

#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

/*
 * Print system configuration
 */
void ECalLiSystemConfigPrint(void)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiSystemPrint","LI cards currently online:");

  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {
     cm_msg(MINFO,"ECalLiSystemPrint",
       "card: %2d, connected?: %2d host: %15s:%4d, socket: %2d, active?: %2d, curr_set: %3u, curr_ntrig = %6u", 
       icard, 
       gLISystem.Card[icard].IsConnected, 
       gLISystem.Card[icard].IPAddress, 
       gLISystem.Card[icard].PortNum, 
       gLISystem.Card[icard].Socket, 
       gLISystem.Card[icard].IsActive, 
       gLISystem.Card[icard].CurrFlashSeqSetting,
       gLISystem.Card[icard].CurrNTrigInSetting);   
  }
#endif
}

/*
 * Print system state
 */
void ECalLiSystemStatePrint(void)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiSystemPrint","LI system state:");

  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {
     cm_msg(MINFO,"ECalLiSystemPrint",
       "card: %2d, active?: %2d, curr_set: %3u, curr_ampl: %3u, curr_pattern: %3u, curr_ntrig = %6u, curr_totntrig = %9u", 
       icard, 
       gLISystem.Card[icard].IsActive, 
       gLISystem.Card[icard].CurrFlashSeqSetting,
       gLISystem.Card[icard].CurrAmplitude,
       gLISystem.Card[icard].CurrPattern,
       gLISystem.Card[icard].CurrNTrigInSetting,
       gLISystem.Card[icard].CurrNTrigTotal);
  }
#endif
}

/*
--------------------------------------------------------------------------------------------
\name   ECalLiCtrlRunStart

\brief  Configure the LI card for a new nd280 run

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   06/05/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>

#include "ECalLiBuildInfo.h"
#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

int ECalLiCtrlRunStart(int run_nu)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlRunStart", "\nECalLiCtrlRunStart> Configuring LI system for run %d ", run_nu);
#endif

  /*
   * Ping the cards to see whether they are alive, and query for their current sequence setting
   */
  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {

     if(gLISystem.Card[icard].IsConnected == 0) continue;

     /*
      * Check whether the LI control card is responsive
      */
#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MINFO, "ECalLiCtrlRunStart", "ECalLiCtrlRunStart> Pinging LI card %d... ", icard);
#endif
     int ping_status = ECalLiPingCard(icard);
     if(ping_status != kCardOK) {
       cm_msg(MERROR, "ECalLiCtrlRunStart", "LI card %d stopped responding!", icard);
       gCtrlStatus = kCtrlErrTcpIpConx;
       return kLIFail;
     }

     /*
      * Querying the LI control card for its flashing progress
      */
#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MINFO, "ECalLiCtrlRunStart", 
            "ECalLiCtrlRunStart> Querying LI card %d for its flashing progress...", icard);
#endif
     int query_status = ECalLiQueryFlashProgress(icard);
     if(query_status != kCardOK) {
        cm_msg(MERROR, "ECalLiCtrlRunStart", "LI card %d doesn't respond to progress query! ", icard);
     }

  } /* loop over control cards */

  /*     
   * Print
   */
   ECalLiSystemStatePrint();
  
  /*
   * Update ECAL LI status at ODB
   */
   ECalLiOdbWriteState();


  /*
   * Read next flash sequence from ODB
   */
  int flseq_read_status = ECalLiOdbReadNextFlashSeq();
  if(flseq_read_status != kODBReadOK) {
    cm_msg(MERROR, "ECalLiCtrlInit", "Can not read the flash sequence!");
    return kLIFail;
  }  

  /*
   * Compare the current and next flash flash sequences
   */
  int flseq_comp = ECalLiFlashSeqComp(&gCurrFlashSequence, &gNextFlashSequence);

  /*
   * If the new run uses the same flash sequence as the previous run, shuffle the settings
   * so that in the new run LI flashing continues from where it stopped 
   */   

   int next_first_setting = 0;

   if(flseq_comp == kFlashSeqCompIdentical) {

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
      cm_msg(MINFO,"ECalLiCtrlRunStart",
             "ECalLiCtrlRunStart> This run uses the same flash sequence as the previous one."); 
      cm_msg(MINFO,"ECalLiCtrlRunStart",
             "ECalLiCtrlRunStart> Reshuffling sequence settings.");
#endif

      int num_of_settings   = gCurrFlashSequence.NS;
      int last_setting_used = gLISystem.Card[0].CurrFlashSeqSetting;

      if(last_setting_used < 0 || last_setting_used >= num_of_settings) {
          next_first_setting = 0;
      }
      else {
         /* 
          * Last setting used was incomplete, step at least one setting back.
          * Step another setting back on Giles' request, in case DPT has dropped the recent LI data.
          */
          next_first_setting = last_setting_used - 2;

          /* cycle: if, say, there are 50 settings (0->49) and we were at setting 1, 
             going 2 steps back actally means going to step 49 */
          if(next_first_setting < 0) {             
              /* 
              cm_msg(MDEBUG, "ECalLiCtrlRunStart", 
                   "ECalLiCtrlRunStart> Next first setting: %d -> %d", next_first_setting, next_first_setting+num_of_settings);
              */
              next_first_setting += num_of_settings;
          }
          /* if number of settings is 1 and we were at setting 0, because of Giles' request (in the general case)
             to step 2 settings back, this may still be negative. If this is the case, set to 0. */
          if(next_first_setting < 0) {
              /* 
              cm_msg(MDEBUG, "ECalLiCtrlRunStart", 
                   "ECalLiCtrlRunStart> Next first setting: %d -> 0", next_first_setting);
              */
              next_first_setting = 0;
          }

      } /* last_setting_used < 0 */

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
      cm_msg(MINFO, "ECalLiCtrlRunStart", 
             "ECalLiCtrlRunStart> Start flashing sequence from setting: %d", next_first_setting);
#endif

   } /* same flash seq */

   else {
      ECalLiFlashSeqCopy(&gNextFlashSequence, &gCurrFlashSequence);
   }

   /*
    * Upload flash sequence to the control cards
    */

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
   cm_msg(MINFO,"ECalLiCtrlRunStart","ECalLiCtrlRunStart> Uploading flash sequence to LI cards...");
   cm_msg(MINFO,"ECalLiCtrlRunStart","ECalLiCtrlRunStart> Flash sequence to be uploaded (starting from setting %d):", next_first_setting);
   ECalLiFlashSeqPrint(&gCurrFlashSequence);
#endif

   for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {

     if(gLISystem.Card[icard].IsConnected == 0) continue;

#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MINFO,"ECalLiCtrlRunStart",
            "ECalLiCtrlRunStart> Uploading flash sequence to LI card %d", icard);
#endif

     ECalLiFlashSeqSend(&gCurrFlashSequence, icard, next_first_setting);

#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MINFO,"ECalLiCtrlRunStart","ECalLiCtrlRunStart> Done configuring LI card %d", icard);
#endif

   } /* loop over control cards */

   gCtrlStatus = kCtrlOK;

   cm_msg(MINFO,"ECalLiCtrlRunStart",
          "ECalLiCtrlRunStart> Done configuring LI system for run %d", run_nu);

   return kLISuccess;
}


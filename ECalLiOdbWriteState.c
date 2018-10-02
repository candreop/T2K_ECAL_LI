/*
--------------------------------------------------------------------------------------------
\name   ECalLiOdbWriteState.c

\brief  

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

int ECalLiOdbWriteState(void)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiOdbWriteState", 
         "ECalLiOdbWriteState> Writing LI state at ODB");
#endif

  unsigned char      is_active      [kNumOfLIControlCards];
  unsigned short int curr_setting   [kNumOfLIControlCards];
  unsigned short int curr_amplitude [kNumOfLIControlCards];
  unsigned short int curr_pattern   [kNumOfLIControlCards];
  unsigned short int curr_ntrigset  [kNumOfLIControlCards];
  unsigned       int curr_ntrigtot  [kNumOfLIControlCards];
#ifdef kODBNoBYTE
  unsigned short int is_active_as_ushort_int [kNumOfLIControlCards];
#endif

  unsigned short int glob_curr_setting  = 0;
  unsigned short int glob_curr_ntrigset = 0;

/*
  Use the CurrFlashSeqSetting and CurrNTrigInSetting from the 1st active card

  int first_active       = -1;
  int first_active_found =  0;
  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
    is_active[icard] = gLISystem.Card[icard].IsActive;
    if(!first_active_found && is_active[icard]) {
      first_active = icard;
    } 
  }
  if(first_active != -1) {
    glob_curr_setting  = gLISystem.Card[first_active].CurrFlashSeqSetting;
    glob_curr_ntrigset = gLISystem.Card[first_active].CurrNTrigInSetting;
  } else {
    cm_msg(MERROR,"ECalLiOdbWriteState", "No LI card is currently active??");
  }
*/

  unsigned short int NF = gCurrFlashSequence.NF;
  unsigned short int NA = gCurrFlashSequence.NA;
  unsigned short int NS = gCurrFlashSequence.NS;

  /*
   * Get current setting id
   */
  unsigned short int min_setting   = 65535;
  unsigned short int max_setting   = 0;
  unsigned short int delta_setting = 0;
  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
     if(!gLISystem.Card[icard].IsActive) continue;
     unsigned short int setting =  gLISystem.Card[icard].CurrFlashSeqSetting;
     if(setting < min_setting) min_setting = setting;
     if(setting > max_setting) max_setting = setting;
  }

  if(min_setting > max_setting) {
    /* what is going on here? */
    cm_msg(MERROR,"ECalLiOdbWriteState", "Querying card flashing status: Min setting > Max setting ??");
    /*exit(1);*/
  }
  else if(min_setting == max_setting) {
    /* all cards at same setting */
    glob_curr_setting  = min_setting;
    delta_setting = 0;
  } else {
    delta_setting = max_setting - min_setting;
    if(delta_setting > 1) {
       /* what is going on here? */
       if(min_setting==0 && max_setting==NS-1) {
          /* ok, I just queried cards while they were crossing the boundary between settings 
             and the flash sequence was restarting from the first setting  */

          glob_curr_setting = max_setting; /* this is the earliest of the two */   

          /* swap around min and max */
          unsigned short int tmp = min_setting;
          min_setting = max_setting;
          max_setting = tmp;

       } else {
          cm_msg(MERROR,"ECalLiOdbWriteState", "Querying card flashing status: Setting spread > 1 ??");
         /*exit(1);*/
       }
    } else {
       /* I queried cards while they were crossing the boundary between settings */
      glob_curr_setting = min_setting; /* this is the earliest of the two */
    }
  }

  /*
   * Get the current number of triggers (smallest number of triggers in lowest setting)
   */
  glob_curr_ntrigset = 65535;
  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
     if(!gLISystem.Card[icard].IsActive) continue;
     if(delta_setting == 1) {
       /* some cards crossed to the next setting, use the ntrig from the ones at the lower setting */
       unsigned short int setting = gLISystem.Card[icard].CurrFlashSeqSetting;
       if(setting != min_setting) continue;
     }
     unsigned short int ntrig =  gLISystem.Card[icard].CurrNTrigInSetting;
     if(ntrig < glob_curr_ntrigset) glob_curr_ntrigset = ntrig;
  }

  /*
   * Get the spread in the current number of triggers
   */  
  if(delta_setting == 1) {
     unsigned short int min_ntrig = 65535;
     unsigned short int max_ntrig = 0;
     for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
        if(!gLISystem.Card[icard].IsActive) continue;
        unsigned short int setting    = gLISystem.Card[icard].CurrFlashSeqSetting;
        unsigned short int ntriginset = gLISystem.Card[icard].CurrNTrigInSetting;
 
        unsigned short int ntrig = 0;
        if(setting == min_setting) ntrig = ntriginset;
        if(setting == max_setting) ntrig = ntriginset + (NA+NF);

        if(ntrig < min_ntrig) min_ntrig = ntrig;
        if(ntrig > max_ntrig) max_ntrig = ntrig;
     }
     unsigned short int delta_ntrig = max_ntrig - min_ntrig;
     if(delta_ntrig > kMaxNTrigSpread) {
          /* what is going on here? */
          cm_msg(MERROR,"ECalLiOdbWriteState", "Querying card flashing status: Trigger spread = %u ??", delta_ntrig);
          /*exit(1);*/
     }
  } else {
     /* I have already warned... */
  }

  /*
   * Copy card info
   */
  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
    is_active      [icard] = gLISystem.Card[icard].IsActive;
    curr_setting   [icard] = gLISystem.Card[icard].CurrFlashSeqSetting;
    curr_amplitude [icard] = gLISystem.Card[icard].CurrAmplitude;
    curr_pattern   [icard] = gLISystem.Card[icard].CurrPattern;
    curr_ntrigset  [icard] = gLISystem.Card[icard].CurrNTrigInSetting;
    curr_ntrigtot  [icard] = gLISystem.Card[icard].CurrNTrigTotal;
/*
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
    cm_msg(MDEBUG,"", "ECalLiOdbWriteState > Writing state for card: %d, set: %d, ampl: %d, pattern: %d, ntrigset: %d, ntrigtot: %d",
           icard, curr_setting[icard], curr_amplitude[icard], curr_pattern[icard],  curr_ntrigset[icard],  curr_ntrigtot[icard]);
#endif
*/
  }

  /*
   * Now save the info in ODB
   */
  int status;
  int size_ushort_array = sizeof(curr_setting);
  int size_uint_array   = sizeof(curr_ntrigtot);
  int size_ushort       = sizeof(unsigned short int);

#ifdef kODBNoBYTE
  /* can not use BYTE type: convert is_active -> is_active_as_short_int */
  for (unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++)
  {
    is_active_as_ushort_int[icard] = (unsigned short int) is_active[icard];
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateIsActive, is_active_as_ushort_int, size_ushort_array, kNumOfLIControlCards, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `is_active_as_ushort_int' array");
    return kODBWriteErr;
  }
#else
  /* can use BYTE type: business as usual */
  int size_char_array = sizeof(is_active);
  status = db_set_value(
     gOdbHandle, 0, kODBpStateIsActive, is_active, size_char_array, kNumOfLIControlCards, TID_BYTE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `is_active' array");
    return kODBWriteErr;
  }
#endif

  status = db_set_value(
     gOdbHandle, 0, kODBpStateCurrSet, curr_setting, size_ushort_array, kNumOfLIControlCards, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `curr_setting' array");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateCurrAmplitude, curr_amplitude, size_ushort_array, kNumOfLIControlCards, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `curr_amplitude' array");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateCurrPattern, curr_pattern, size_ushort_array, kNumOfLIControlCards, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `curr_pattern' array");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateCurrNTrigInSet, curr_ntrigset, size_ushort_array, kNumOfLIControlCards, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `curr_ntrigset' array");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateCurrNTrigTot, curr_ntrigtot, size_uint_array, kNumOfLIControlCards, TID_DWORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the `curr_ntrigtot' array");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateGlobCurrSet, &glob_curr_setting, size_ushort, 1, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the current setting");
    return kODBWriteErr;
  }
  status = db_set_value(
     gOdbHandle, 0, kODBpStateGlobCurrNTrigInSet, &glob_curr_ntrigset, size_ushort, 1, TID_WORD);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbWriteState", "Can't write the number of triggers in the current setting");
    return kODBWriteErr;
  }

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiOdbWriteState", "ECalLiOdbWriteState> Done writing LI state!");
#endif

  return kODBWriteOK;
}


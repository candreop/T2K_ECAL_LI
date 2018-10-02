/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqMesg

\brief  Assemble a mesg, containing the input flash sequence info, to be sent 
        over the tcp/ip socket

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"
#include "ECalLiGlobals.h"
#include "ECalLiFlashSeq.h"

void ECalLiFlashSeqMesg(
  ECalLiFlashSeq_t * flseq, unsigned short int icard,
  int first_setting, int n_failed_prev_attempts, unsigned char *umesg)
{
  if(!flseq) return;

  if(icard >= kNumOfLIControlCards) return;

  unsigned short int hdr = (n_failed_prev_attempts == 0) ? kNewFlashSeq : kResentFlashSeq;
  unsigned short int NF  = flseq->NF;
  unsigned short int NA  = flseq->NA;
  unsigned short int NS  = flseq->NS;
  
  umesg[0] = ECalLiToMSByte(hdr);
  umesg[1] = ECalLiToLSByte(hdr);
  umesg[2] = ECalLiToMSByte(NF);
  umesg[3] = ECalLiToLSByte(NF);
  umesg[4] = ECalLiToMSByte(NA);
  umesg[5] = ECalLiToLSByte(NA);
  umesg[6] = ECalLiToMSByte(NS);
  umesg[7] = ECalLiToLSByte(NS);

  unsigned short int last_byte = 8;

  /* 
   * Add all settings
   * Take into account any decision to re-shuffle the list of settings for the current flash sequence.
   * (do not loop from 0 -> NS, but from 1st setting -> NS and then 0 -> 1st setting)
   */

  for(unsigned short int iset = first_setting; iset < flseq->NS; iset++) {
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"ECalLiFlashSeqMesg",
            "ECalLiFlashSeqMesg> Adding setting %d to flash sequence mesg to be sent", iset);
#endif
      umesg[last_byte++] = flseq->Pattern[icard][iset];
      umesg[last_byte++] = flseq->Amplitude[icard][iset];
  }
  for(unsigned short int iset = 0; iset < first_setting; iset++) {
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"ECalLiFlashSeqMesg",
            "ECalLiFlashSeqMesg> Adding setting %d to flash sequence mesg to be sent", iset);
#endif
      umesg[last_byte++] = flseq->Pattern[icard][iset];
      umesg[last_byte++] = flseq->Amplitude[icard][iset];
  }

  /* 
   * Fill the rest of the buffer with 0's
   */
  for(unsigned short int iset = flseq->NS; iset < kMaxNumOfFlashSeqSet; iset++) {
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"ECalLiFlashSeqMesg",
            "ECalLiFlashSeqMesg> Setting %d (between actual and max possible NS) set to 0", iset);
#endif
      umesg[last_byte++] = 0;
      umesg[last_byte++] = 0;
  }
}


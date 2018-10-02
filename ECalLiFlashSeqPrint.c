/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqPrint

\brief  Print-out the input flash sequence

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>

#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"
#include "ECalLiFlashSeq.h"

void ECalLiFlashSeqPrint(ECalLiFlashSeq_t * flseq)
{
  if(!flseq) return;

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_

  cm_msg(MINFO,"", "  NF: %hu", flseq->NF);
  cm_msg(MINFO,"", "  NA: %hu", flseq->NA);
  cm_msg(MINFO,"", "  NS: %hu", flseq->NS);

  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {

     cm_msg(MINFO,"","  CARD: %4u - Flashing settings:", icard);

     for(unsigned short int iset = 0; iset < flseq->NS; iset++) {
         cm_msg(MINFO,"",
            "  > setting: %4u  ->  pattern: %3hu / amplitude: %3hu", 
            iset, flseq->Pattern[icard][iset], flseq->Amplitude[icard][iset]);
     }
  }

#endif

}


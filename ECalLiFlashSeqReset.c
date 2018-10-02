/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqReset

\brief  Reset the input flash sequence

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>

#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"
#include "ECalLiFlashSeq.h"

void ECalLiFlashSeqReset(ECalLiFlashSeq_t * flseq)
{
  if(!flseq) return;

  flseq->NF = 0;
  flseq->NA = 0;
  flseq->NS = 0;

  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
     for(unsigned short int iset = 0; iset < kMaxNumOfFlashSeqSet; iset++) {
         flseq->Pattern  [icard][iset] = 0;
         flseq->Amplitude[icard][iset] = 0;
     }
  }
}


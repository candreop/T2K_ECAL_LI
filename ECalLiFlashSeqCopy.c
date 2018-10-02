/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqCopy

\brief  Copy a flash sequence

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>

#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"
#include "ECalLiFlashSeq.h"

void ECalLiFlashSeqCopy(ECalLiFlashSeq_t * source, ECalLiFlashSeq_t * target)
{
  if(!source) return;
  if(!target) return;

  ECalLiFlashSeqReset(target);

  target->NF = source->NF;
  target->NA = source->NA;
  target->NS = source->NS;

  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
     for(unsigned short int iset = 0; iset < source->NS; iset++) {
        target->Pattern  [icard][iset] = source->Pattern  [icard][iset];
        target->Amplitude[icard][iset] = source->Amplitude[icard][iset];
     }
  }
}


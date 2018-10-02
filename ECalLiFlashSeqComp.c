/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqComp

\brief  Compare two flash sequences for a given card

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>

#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"
#include "ECalLiFlashSeq.h"

int ECalLiFlashSeqComp(
	ECalLiFlashSeq_t * flseq0, ECalLiFlashSeq_t * flseq1, unsigned short int icard)
{
  if(!flseq0) return kFlashSeqCompErr;
  if(!flseq1) return kFlashSeqCompErr;

  if(icard >= kNumOfLIControlCards) return kFlashSeqCompErr;

  if ( flseq0->NF != flseq1->NF ) return kFlashSeqCompDifferent;
  if ( flseq0->NA != flseq1->NA ) return kFlashSeqCompDifferent;
  if ( flseq0->NS != flseq1->NS ) return kFlashSeqCompDifferent;

  for(unsigned short int iset = 0; iset < flseq0->NS; iset++) {
     if( flseq0->Pattern  [icard][iset] != flseq1->Pattern  [icard][iset] ) return kFlashSeqCompDifferent;
     if( flseq0->Amplitude[icard][iset] != flseq1->Amplitude[icard][iset] ) return kFlashSeqCompDifferent;
  }

  return kFlashSeqCompIdentical;
}

/*
 * same as above but for all cards simultaneously
 */

int ECalLiFlashSeqComp(
	ECalLiFlashSeq_t * flseq0, ECalLiFlashSeq_t * flseq1)
{
  if(!flseq0) return kFlashSeqCompErr;
  if(!flseq1) return kFlashSeqCompErr;

  if ( flseq0->NF != flseq1->NF ) return kFlashSeqCompDifferent;
  if ( flseq0->NA != flseq1->NA ) return kFlashSeqCompDifferent;
  if ( flseq0->NS != flseq1->NS ) return kFlashSeqCompDifferent;

  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {
     for(unsigned short int iset = 0; iset < flseq0->NS; iset++) {
        if( flseq0->Pattern  [icard][iset] != flseq1->Pattern  [icard][iset] ) return kFlashSeqCompDifferent;
        if( flseq0->Amplitude[icard][iset] != flseq1->Amplitude[icard][iset] ) return kFlashSeqCompDifferent;
     }
  }

  return kFlashSeqCompIdentical;
}



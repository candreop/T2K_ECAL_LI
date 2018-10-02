/*
--------------------------------------------------------------------------------------------
\name   ECalLiSystem

\brief  LI system (list of all LI cards currently online)

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_SYSTEM_H_
#define _ECALLI_SYSTEM_H_

#include "ECalLiDefs.h"
#include "ECalLiCard.h"

typedef struct
{
  ECalLiCard_t Card [kNumOfLIControlCards];
} 
ECalLiSystem_t;

#endif



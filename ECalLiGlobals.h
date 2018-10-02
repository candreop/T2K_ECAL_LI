/*
--------------------------------------------------------------------------------------------
\name   ECalLiGlobals

\brief  Globals

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   06/05/2008
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_GLOBAL_VARS_H_
#define _ECALLI_GLOBAL_VARS_H_

#include "ECalLiFlashSeq.h"
#include "ECalLiSystem.h"
#include "ECalLiDefs.h"

#include <midas.h>

/* 
 * Controller status code 
 */
extern int gCtrlStatus;

/* 
 * ODB Handle 
 */
extern HNDLE gOdbHandle;

/* 
 * All available LI control cards
 */
extern ECalLiSystem_t gLISystem;

/* 
 * Current flash-sequence 
 */
extern ECalLiFlashSeq_t gCurrFlashSequence;
extern ECalLiFlashSeq_t gNextFlashSequence;

/*
 *
 */


#endif


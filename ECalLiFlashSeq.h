/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeq

\brief  A flash sequence
        As agreed at the DAQ/LI mtg on Nov 5th, 2008 (Rutherford Lab)

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008  
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_FLASH_SEQUENCE_H_
#define _ECALLI_FLASH_SEQUENCE_H_

#include "ECalLiDefs.h"

typedef struct
{
 /*
  * Number of triggers to flash for each setting.
  * Same for all settings & cards
  */
  unsigned short int NF; 

 /*
  * Number of triggers to append to the end of each setting to allow time 
  * for each control card to be reconfigured.
  * These triggers are not entered in the LI histograms.
  * The total number of triggers at each setting is therefore NF+NA
  * Same for all settings & cards.
  */
  unsigned short int NA;

 /*
  * Number of settings in the list to follow
  */
  unsigned short int NS;

 /*
  * Pattern (LED group flashed).
  * Is a bit-coded single-byte field.
  * Pattern to be interpreted by the LI card.
  * Example 1
  *   For the down-stream ECAL:
  *   B0: North, B1: East, B2: South, B3: West, B4-7: unused 
  *   BO is the LSF bit. 
  *   Any combination of bits can be set.
  */
  unsigned char Pattern[kNumOfLIControlCards][kMaxNumOfFlashSeqSet];

 /*
  * Amplitude (LED intensity).
  * Is a single-byte unsigned int which defines the light-level on a scale of 0-255
  */
  unsigned char Amplitude[kNumOfLIControlCards][kMaxNumOfFlashSeqSet];

} 
ECalLiFlashSeq_t;

#endif

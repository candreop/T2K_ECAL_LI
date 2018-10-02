/*
--------------------------------------------------------------------------------------------
\name   ECalLiOdbReadFlashSeq

\brief  Read next flash sequence from the ODB

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>
#include <string.h>

#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

int ECalLiOdbReadNextFlashSeq(void)
{
#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiOdbReadNextFlashSeq", 
         "ECalLiOdbReadNextFlashSeq> Reading flash sequence from ODB.");
#endif

  int status;

  /* see ECalLiFlashSeq.h for NA,NF,NS definitions*/
  unsigned short int  nf; 
  unsigned short int  na;
  unsigned short int  ns;
  unsigned short int  mask      [kMaxNumOfFlashSeqSet];
  unsigned char       amplitude [kMaxNumOfFlashSeqSet];
  unsigned char       pattern   [kMaxNumOfFlashSeqSet];

#ifdef kODBNoBYTE
  unsigned short int  amplitude_as_short_int [kMaxNumOfFlashSeqSet];
  unsigned short int  pattern_as_short_int   [kMaxNumOfFlashSeqSet];
#endif

  int size_ushort       = sizeof(nf);
  int size_ushort_array = sizeof(mask);

  status = db_get_value(
    gOdbHandle, 0, kODBpSeqNF, &nf, &size_ushort, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbReadNextFlashSeq", "Can't read NF for input flash sequence.");
    return kODBReadErr;
  }
  status = db_get_value(
    gOdbHandle, 0, kODBpSeqNA, &na, &size_ushort, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbReadNextFlashSeq", "Can't read NA for input flash sequence.");
    return kODBReadErr;
  }
  status = db_get_value(
    gOdbHandle, 0, kODBpSeqNS, &ns, &size_ushort, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read NS for input flash sequence.");
    return kODBReadErr;
  }
  status = db_get_value(
     gOdbHandle, 0, kODBpSeqPulserMask, mask, &size_ushort_array, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read pulser mask array.");
    return kODBReadErr;
  }

#ifdef kODBNoBYTE

  /* no BYTE type: read as short int and convert */
  status = db_get_value(
     gOdbHandle, 0, kODBpSeqAmplitude, amplitude_as_short_int, &size_ushort_array, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read flasher amplitude array.");
    return kODBReadErr;
  }
  status = db_get_value(
     gOdbHandle, 0, kODBpSeqPattern, pattern_as_short_int, &size_ushort_array, TID_WORD, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read flasher pattern array.");
    return kODBReadErr;
  }
  for(unsigned int iset = 0; iset < kMaxNumOfFlashSeqSet; iset++) {
       amplitude [iset] = (unsigned char) amplitude_as_short_int [iset];
       pattern   [iset] = (unsigned char) pattern_as_short_int   [iset];
  }

#else

  /* use BYTE type */
  int size_char_array  = sizeof(pattern);
  status = db_get_value(
     gOdbHandle, 0, kODBpSeqAmplitude, amplitude, &size_char_array, TID_BYTE, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read flasher amplitude array.");
    return kODBReadErr;
  }
  status = db_get_value(
     gOdbHandle, 0, kODBpSeqPattern, pattern, &size_char_array, TID_BYTE, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR, "ECalLiOdbReadNextFlashSeq", "Can't read flasher pattern array.");
    return kODBReadErr;
  }

#endif


  /*
   * check NS against maximum possible number of settings
   */
 
   /* ... */


  /*
   * print
   */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG, "ECalLiOdbReadNextFlashSeq", 
         "ECalLiOdbReadNextFlashSeq> Read from ODB: NA = %u, NF = %u, NS = %u", na, nf, ns);
  for(unsigned short int is = 0; is < ns; is++) {
    cm_msg(MDEBUG, "ECalLiOdbReadNextFlashSeq", 
           "ECalLiOdbReadNextFlashSeq> %u --> amplitude: %u, pattern: %u", is, amplitude[is], pattern[is]);
  }
#endif

  /*
   * store read values in gNextFlashSequence
   */
  ECalLiFlashSeqReset(&gNextFlashSequence);

  gNextFlashSequence.NF = (unsigned short int) nf;
  gNextFlashSequence.NA = (unsigned short int) na;
  gNextFlashSequence.NS = (unsigned short int) ns;

  for(unsigned short int icard = 0; icard < kNumOfLIControlCards; icard++) {

       unsigned short int card_mask = (1<<icard);

       for(unsigned short int iset = 0; iset < ns; iset++) {
       if(mask[iset] & card_mask) {
           gNextFlashSequence.Pattern  [icard][iset] = pattern  [iset];
           gNextFlashSequence.Amplitude[icard][iset] = amplitude[iset];
       } else {
           gNextFlashSequence.Pattern  [icard][iset] = 0;
           gNextFlashSequence.Amplitude[icard][iset] = 0;
       } /* if flash this card */
    } /* loop over settings */
  } /* loop over cards */

#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiOdbReadNextFlashSeq", "ECalLiOdbReadNextFlashSeq> Done reading next flash sequence!");
#endif

  return kODBReadOK;
}


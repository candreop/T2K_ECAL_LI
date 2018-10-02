/*
--------------------------------------------------------------------------------------------
\name   ECalLiFlashSeqSend

\brief  Transmit the input flash sequence over the tcp/ip socket

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"
#include "ECalLiGlobals.h"
#include "ECalLiFlashSeq.h"

void ECalLiFlashSeqSend(ECalLiFlashSeq_t * flseq, unsigned short int icard, int first_setting)
{
  if(!flseq) {
     cm_msg(MERROR,"ECalLiFlashSeqSend", "Null flash sequence");
     return;
  }

  if(gLISystem.Card[icard].IsConnected == 0) {
     cm_msg(MERROR, "ECalLiFlashSeqSend", "Card %d isn't connected!", icard);
     return;
  }

  if(icard >= kNumOfLIControlCards) {
     return;
  }

  int success     = 0;
  int n_attempts  = 0;
  int keep_on     = 1;

  /* Get TCP/IP socket for input card */
  int socket = gLISystem.Card[icard].Socket;

  while (keep_on) {

     unsigned char mesg_send[kMesgSize];
     unsigned char mesg_recv[kMesgSize];

     memset(mesg_send, 0, kMesgSize);
     memset(mesg_recv, 0, kMesgSize);

     /*
      * Convert the requested flash sequence into a byte-array
      */
     ECalLiFlashSeqMesg(flseq, icard, first_setting, n_attempts, mesg_send);

     /*
      * Send the byte-array over the TCP/IP socket
      */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MINFO, "ECalLiFlashSeqSend", "ECalLiFlashSeqSend> Sending flash sequence...");
     ECalLiPrintByteField16(mesg_send);
#endif
     ECalLiTCPSend(socket, mesg_send, kMesgSize);

     /*
      * Receive the echo-ed back flash sequence
      */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"ECalLiFlashSeqSend", "ECalLiFlashSeqSend> Waiting for the echoed flash-sequence from the LI...");
     ECalLiPrintByteField16(mesg_recv);
#endif
     ECalLiTCPRecv(socket, mesg_recv, kMesgSize);

     /*
      * Check for errors
      */
     success = ! memcmp(mesg_send, mesg_recv, kMesgSize);

     n_attempts++;
     keep_on = !success && n_attempts < kMaxNumOfFlashSeqErrMesgs;

     if(!success) {
        cm_msg(MERROR, "ECalLiFlashSeqSend", "Flash-sequence was echoed back with errors!");
        if(keep_on) {
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
           cm_msg(MINFO,"ECalLiFlashSeqSend", "ECalLiFlashSeqSend> Attempting to re-send the flash-sequence");
#endif
        }//keep on trying...
      }//success?

  }

  if(!success) {
     cm_msg(MERROR, "ECalLiFlashSeqSend",
            "LI card failed to receive & echo-back a correct flash sequence after %d attempts", n_attempts--);
  } else {
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
     cm_msg(MINFO, "ECalLiFlashSeqSend", 
            "ECalLiFlashSeqSend> The flash sequence has been successfully transmitted.");
#endif
  }
}


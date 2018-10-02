/*
--------------------------------------------------------------------------------------------
\name   ECalLiPingCard

\brief  Check whether the LI control card is responsive

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

int ECalLiPingCard(int icard)
{
  if(!kPingLIControlCard) {
     cm_msg(MINFO,"ECalLiPingCard", "ECalLiPingCard> **Pinging disabled**");
     return kCardOK;
  }

  if(gLISystem.Card[icard].IsConnected == 0) {
     cm_msg(MDEBUG,"ECalLiPingCard","ECalLiPingCard> Card %d isn't connected.", icard);
     return kCardErr;
  }

  unsigned char mesg_send [kMesgSize];
  unsigned char mesg_recv [kMesgSize];

  /* Assemble the ping mesg */
  memset(mesg_send, 0, kMesgSize);
  mesg_send[0] = ECalLiToMSByte(kPingMesg); 
  mesg_send[1] = ECalLiToLSByte(kPingMesg);

  /* Get TCP/IP socket for input card */
  int socket = gLISystem.Card[icard].Socket;

  /* Attempt to ping the LI card & receive the right ping response */
  int n_attempts = 0;
  while(n_attempts < kMaxNumOfPings) {

#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MDEBUG,"ECalLiPingCard","ECalLiPingCard> Pinging attempt: %d", n_attempts);
#endif

     /* Send the 'ping' mesg */
     ECalLiTCPSend(socket, mesg_send, kMesgSize);
     ECalLiPrintByteField16(mesg_send, "PING! Sent mesg:");

     /* Reveive the 'ping' response */
     ECalLiTCPRecv(socket, mesg_recv, kMesgSize);
     ECalLiPrintByteField16(mesg_recv, "PING response:");

     unsigned short int reply = ECalLiToUShort(mesg_recv[0], mesg_recv[1]);
     if(reply==kPingMesgResponse) {
#ifdef _ECALLI_MID_LEVEL_DEBUG_
        cm_msg(MDEBUG,"ECalLiPingCard", "ECalLiPingCard> LI card %d is alive.", icard);
#endif
        return kCardOK;
     } 
     n_attempts++;
  }

  cm_msg(MERROR,"ECalLiPingCard", "LI card %d doesn't respond.", icard);

  return kCardErr;
}


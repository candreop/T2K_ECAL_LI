/*
--------------------------------------------------------------------------------------------
\name   ECalLiQueryFlashProgress

\brief  Query the LI card for the current point in the flashing sequence

\notes  Mesg
			    8-bit fields
	# 16-bit word  |  MS            LS
	...............................................
	0              |  E0            E0
	1	       |  0             CurrentSetting
	2              |  NTrig1        NTrig2
	3              |  0             CurrentAmplitude
	4              |  0             CurrentPattern
        5              |  NTotTrig1     NTotTrig2 
        6              |  NTotTrig3     NTotTrig4
        7              |  0...

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

int ECalLiQueryFlashProgress(int icard)
{
  if(!kQueryFlashProgress) {
     cm_msg(MINFO,"ECalLiQueryFlashProgress", 
            "ECalLiQueryFlashProgress> Querying disabled!");
     return kCardOK;
  }

  if(gLISystem.Card[icard].IsConnected == 0) {
     cm_msg(MERROR,"ECalLiQueryFlashProgress", "Card %d isn't connected", icard);
     return kCardErr;
  }

  unsigned char mesg_send [kMesgSize];
  unsigned char mesg_recv [kMesgSize];

  /* Assemble the query mesg */
  memset(mesg_send, 0, kMesgSize);
  mesg_send[0] = ECalLiToMSByte(kFlashProgressMesg); 
  mesg_send[1] = ECalLiToLSByte(kFlashProgressMesg);

  /* Get TCP/IP socket for input card */
  int socket = gLISystem.Card[icard].Socket;

  /* Attempt to query the LI card  */
  int n_attempts = 0;
  while(n_attempts < kMaxNumOfFlashProgressQueries) {

#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MDEBUG, "ECalLiQueryFlashProgress", 
           "ECalLiQueryFlashProgress> Querying flash progress attempt: %d", n_attempts);
#endif

     /* Send the query mesg */
     ECalLiTCPSend(socket, mesg_send, kMesgSize);
     ECalLiPrintByteField16(mesg_send, "Sent `flashing progress query' mesg:");

     /* Reveive the query response */
     ECalLiTCPRecv(socket, mesg_recv, kMesgSize);
     ECalLiPrintByteField16(mesg_recv, "`flashing progress query' reply mesg:");

     unsigned short int reply = ECalLiToUShort(mesg_recv[0], mesg_recv[1]);
     if(reply == kFlashProgressResponse) {

#ifdef _ECALLI_LOW_LEVEL_DEBUG_
        cm_msg(MDEBUG,"ECalLiQueryFlashProgress", 
               "ECalLiQueryFlashProgress> LI card %d responded correctly!", icard);
#endif

        /*
         * Now get the current position in the flash sequence
         * 2nd 16-bit word is: ns (current flash sequence)
         * 3rd 16-bit word is: nf (number of flashes in current sequence)
         */
        unsigned short int setid     = ECalLiToUShort(mesg_recv[2],  mesg_recv[3]);
        unsigned short int ntrig     = ECalLiToUShort(mesg_recv[4],  mesg_recv[5]);
        unsigned short int amplitude = ECalLiToUShort(mesg_recv[6],  mesg_recv[7]);
        unsigned short int pattern   = ECalLiToUShort(mesg_recv[8],  mesg_recv[9]);
        unsigned short int ntottrig1 = ECalLiToUShort(mesg_recv[11], mesg_recv[10]);
        unsigned short int ntottrig2 = ECalLiToUShort(mesg_recv[13], mesg_recv[12]);
        unsigned       int ntottrig  = ECalLiToUInt(ntottrig2,ntottrig1);

#ifdef _ECALLI_LOW_LEVEL_DEBUG_
       	cm_msg(MDEBUG,"ECalLiQueryFlashProgress",
               "ECalLiQueryFlashProgress> Current setting id: %u", setid);
        cm_msg(MDEBUG,"ECalLiQueryFlashProgress",
               "ECalLiQueryFlashProgress> Num. of triggers in current setting: %u", ntrig);
       	cm_msg(MDEBUG,"ECalLiQueryFlashProgress",
               "ECalLiQueryFlashProgress> Current amplitude: %u", amplitude);
        cm_msg(MDEBUG,"ECalLiQueryFlashProgress",
               "ECalLiQueryFlashProgress> Current pattern: %u", pattern);
        cm_msg(MDEBUG,"ECalLiQueryFlashProgress",
               "ECalLiQueryFlashProgress> Total number of triggers: %u", ntottrig);
#endif

       	/*
       	 * Update (successful query)
       	 */
        gLISystem.Card[icard].IsActive            = 1;
        gLISystem.Card[icard].CurrFlashSeqSetting = setid;
        gLISystem.Card[icard].CurrAmplitude       = amplitude;
        gLISystem.Card[icard].CurrPattern         = pattern;
        gLISystem.Card[icard].CurrNTrigInSetting  = ntrig;
        gLISystem.Card[icard].CurrNTrigTotal      = ntottrig;

        return kCardOK;
     } 
     n_attempts++;
  }

  /*
   * Update (unsuccessful query)
   */
  gLISystem.Card[icard].CurrFlashSeqSetting = 0;
  gLISystem.Card[icard].CurrNTrigInSetting  = 0;
  gLISystem.Card[icard].IsActive            = 0;

  cm_msg(MERROR, "ECalLiQueryFlashProgress", 
         "LI card %d doesn't respond to progress query.", icard);

  return kCardErr;
}


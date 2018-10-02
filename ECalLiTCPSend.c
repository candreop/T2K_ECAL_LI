/*
--------------------------------------------------------------------------------------------
\name   ECalLiTCPSend

\brief  Send mesg

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>      
#include <string.h>      
#include <stdlib.h>     
#include <stdio.h>     
#include <sys/socket.h> 

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

/*
 * Send the input char array (mesg) of the specified length 
 * over the input TCP/IP socket
 */
int ECalLiTCPSend(int sock, char *mesg, int len)
{
  if(len<=0) {
    return kTCPSendErr;
  }

  if(kPrintOutByteCounts) {
    cm_msg(MDEBUG,"ECalLiTCPSend","ECalLiTCPSend> Sending %d bytes.", len);
  }

  /* Send message */
  int len_sent = send(sock, mesg, len, 0);
  if (len_sent != len) {
     cm_msg(MERROR,"ECalLiTCPSend","send() failed.");
     return kTCPSendErr;
  }
  return kTCPSendOK;
}

/*
--------------------------------------------------------------------------------------------
*/

/*
 * Send the input unsigned char array (mesg) of the specified length 
 * over the input TCP/IP socket
 */
int ECalLiTCPSend(int sock, unsigned char *mesg, int len)
{
  if(len<=0) {
    return kTCPSendErr;
  }

  if(kPrintOutByteCounts) {
     cm_msg(MDEBUG,"ECalLiTCPSend","ECalLiTCPSend> Sending %d bytes.", len);
  }

  /* Send message */
  int len_sent = send(sock, mesg, len, 0);
  if (len_sent != len) {
     cm_msg(MERROR,"ECalLiTCPSend", "send() failed.");
     return kTCPSendErr;
  }

  return kTCPSendOK;
}


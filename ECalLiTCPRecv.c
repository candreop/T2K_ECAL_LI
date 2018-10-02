/*
--------------------------------------------------------------------------------------------
\name   ECalLiTCPRecv

\brief  Receive mesg

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>     
#include <string.h>     
#include <stdlib.h>     
#include <sys/socket.h> 
#include <stdio.h> 

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

int ECalLiTCPRecv(int clnt_sock, char * mesg, int len)
{
  if(len<=0) {
    return kTCPRecvErr;
  }

  memset(mesg, '\0', len);

  int len_recv_tot   = 0;
  int nrecv_zero     = 0;

  while(len_recv_tot < len) {

    int len_remaining = len - len_recv_tot;

    char mesg_recv [kMesgSize];
    memset(mesg_recv, '\0', kMesgSize);

    int len_recv = recv(clnt_sock, mesg_recv, len_remaining, 0);
    if(kPrintOutByteCounts) {
      cm_msg(MDEBUG,"ECalLiTCPRecv","ECalLiTCPRecv> Received %d bytes.", len_recv);  
    }
    if (len_recv < 0) {
      cm_msg(MERROR,"ECalLiTCPRecv","recv() failed.");
      return kTCPRecvErr;
    }
    if (len_recv == 0) {
      nrecv_zero++;
    }
    if (len_recv > 0) {
      nrecv_zero = 0;
    }
    if (nrecv_zero == kTCPMaxNumOfZeroByteRecv) {
      cm_msg(MERROR,"ECalLiTCPRecv", "recv() fetched 0 bytes %d times in a row.", 
             kTCPMaxNumOfZeroByteRecv);
      return kTCPRecvErr;
    }
    for(int i=0; i<len_recv; i++) {
      mesg[i+len_recv_tot] = mesg_recv[i];
    }

    len_recv_tot += len_recv;
  }
  return kTCPRecvOK;
}
/*
--------------------------------------------------------------------------------------------
*/
int ECalLiTCPRecv(int clnt_sock, unsigned char * mesg, int len)
{
  if(len<=0) {
    return kTCPRecvErr;
  }

  memset(mesg, '\0', len);

  int len_recv_tot   = 0;
  int nrecv_zero     = 0;

  while(len_recv_tot < len) {

    int len_remaining = len - len_recv_tot;

    unsigned char mesg_recv [kMesgSize];
    memset(mesg_recv, '\0', kMesgSize);

    int len_recv = recv(clnt_sock, mesg_recv, len_remaining, 0);
    if(kPrintOutByteCounts) {
      cm_msg(MDEBUG,"ECalLiTCPRecv","ECalLiTCPRecv> Received %d bytes.", len_recv);  
    }
    if (len_recv < 0) {
      cm_msg(MERROR,"ECalLiTCPRecv","recv() failed.");
      return kTCPRecvErr;
    }
    if (len_recv == 0) {
      nrecv_zero++;
    }
    if (len_recv > 0) {
      nrecv_zero = 0;
    }
    if (nrecv_zero == kTCPMaxNumOfZeroByteRecv) {
      cm_msg(MERROR,"ECalLiTCPRecv", "recv() fetched 0 bytes %d times in a row.", 
             kTCPMaxNumOfZeroByteRecv);
      return kTCPRecvErr;
    }

    for(int i=0; i<len_recv; i++) {
      mesg[i+len_recv_tot] = mesg_recv[i];
    }

    len_recv_tot += len_recv;
  }
  return kTCPRecvOK;
}


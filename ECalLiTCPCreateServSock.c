/*
--------------------------------------------------------------------------------------------
\name   ECalLiTCPCreateServSock

\brief  Create a TCP/IP socket 

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <string.h>    
#include <stdio.h>    

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

int ECalLiTCPCreateServSock(int serv_port)
{
  int serv_sock;                 /* socket descriptor for server */
  struct sockaddr_in serv_addr;  /* server address */

  serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serv_sock < 0) {
    cm_msg(MERROR,"ECalLiTCPCreateServSock", "Failed to create socket.");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));         /* reset */
  serv_addr.sin_family      = AF_INET;              /* address family */
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /* any incoming interface */
  serv_addr.sin_port        = htons(serv_port);     /* set port */

  int exit_code = bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (exit_code < 0) {
    cm_msg(MERROR,"ECalLiTCPCreateServSock", "Failed to bind socket to local address.");
  }

  exit_code = listen(serv_sock, kNMaxPendingMesg);
  if (exit_code < 0) {
    cm_msg(MERROR,"ECalLiTCPCreateServSock",
           "Failed specifying number of pending client messages that can be queued.");
  }

  return serv_sock;
}

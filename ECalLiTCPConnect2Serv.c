/*
--------------------------------------------------------------------------------------------
\name   ECalLiTCPConnect2Serv

\brief  Connect to TCP/IP server

\arguments
        char* serv_ip   ->  server ip address
        int   serv_port ->  server port

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>      
#include <stdio.h>      
#include <stdlib.h>     
#include <sys/socket.h> 
#include <sys/time.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>  
#include <string.h>    
#include <unistd.h>    

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

int ECalLiTCPConnect2Serv(char* serv_ip, int serv_port)
{
  int    serv_sock;              /* socket descriptor */
  struct sockaddr_in serv_addr;  /* server address */

  /*
   * Create the socket
   */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> Creating a server socket.");
#endif
  serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serv_sock < 0) {
    cm_msg(MERROR,"ECalLiTCPConnect2Serv", "Failed to create socket.");
    return -1;
  }
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> ...Done creating a server socket!");
#endif

  /*
   * Set TCP_NODELAY if requested
   */ 
  if(kUseTCP_NODELAY) {
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> Setting socket option: TCP_NODELAY.");
#endif
     int flag   = 1;
     int result = setsockopt(serv_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));   
     if(result < 0) {
       cm_msg(MERROR,"ECalLiTCPConnect2Serv", "Could not set TCP_NODELAY.");
     }
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
     cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> ...Done setting TCP_NODELAY.");
#endif
  }

  /*
   * Set time-outs
   */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> Setting timeouts.");
#endif
  struct timeval tvrecv;
  tvrecv.tv_sec  = kTCPRecvTimeout;
  tvrecv.tv_usec = 5000;
  int to = setsockopt(serv_sock, SOL_SOCKET, SO_RCVTIMEO, &tvrecv, sizeof(tvrecv)); 
  if(to<0) {
    cm_msg(MERROR,"ECalLiTCPConnect2Serv", "Setting timeouts failed.");
  }
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> ...Done setting time-outs!");
#endif

  /* 
   * Set server address
   */
#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> Setting server address.");
#endif
  memset(&serv_addr, 0, sizeof(serv_addr));         /* reset */
  serv_addr.sin_family      = AF_INET;              /* address family */
  serv_addr.sin_addr.s_addr = inet_addr(serv_ip);   /* set server IP */
  serv_addr.sin_port        = htons(serv_port);     /* set port */

#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> ...Done setting server address!");
#endif

  /* 
   * Connect to server 
   */
#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MDEBUG,"","ECalLiTCPConnect2Serv> Connecting....");
#endif
  int err_code = connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (err_code<0) {
    cm_msg(MERROR,"ECalLiTCPConnect2Serv", "connect() failed.\n");
    return -1;
  }
#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MINFO,"","ECalLiTCPConnect2Serv> Connection established!");
#endif

  return serv_sock;
}

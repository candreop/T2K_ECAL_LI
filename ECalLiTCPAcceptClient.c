/*
--------------------------------------------------------------------------------------------
\name   ECalLiTCPClientConnect

\brief  Accept client & return client socket

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>      
#include <stdio.h>      
#include <stdlib.h>     
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <string.h>  
#include <unistd.h>  

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

int ECalLiTCPAcceptClient(int serv_sock)
{
  int clnt_sock;                 /* socket descriptor for client */
  struct sockaddr_in clnt_addr;  /* client address */

  /* Set the size of the in-out parameter */
  unsigned int len = sizeof(clnt_addr);

  /* Wait for a client to connect */
  clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &len);
  if (clnt_sock<0) {
     cm_msg(MERROR, "ECalLiTCPAcceptClient", "accept() failed.");
  }

  /* clntSock is connected to a client! */
  cm_msg(MINFO,"ECalLiTCPAcceptClient", 
         "ECalLiTCPAcceptClient> Handling client %s", inet_ntoa(clnt_addr.sin_addr));

  return clnt_sock;
}

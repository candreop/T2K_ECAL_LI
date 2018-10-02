/*
--------------------------------------------------------------------------------------------
\name   ECalLiCtrlShutDown

\brief  Shut-down the ECAL LI controller

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   06/05/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <unistd.h>

#include "ECalLiBuildInfo.h"
#include "ECalLiPrototypes.h"
#include "ECalLiGlobals.h"

void ECalLiCtrlShutDown(void)
{
  /*
   *  Disconnect from the experiment
   */ 
  cm_disconnect_experiment();

  /*
   *  Close TCP/IP sockets
   */ 
   for(int icard = 0; icard < kNumOfLIControlCards; icard++) {

     if(gLISystem.Card[icard].IsConnected == 0) continue;

     int socket = gLISystem.Card[icard].Socket;
     close(socket);
   }

}


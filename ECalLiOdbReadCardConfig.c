/*
--------------------------------------------------------------------------------------------
\name   ECalLiOdbReadCardConfig.c

\brief  Load LI communication card configuration from the ODB

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

int ECalLiOdbReadCardConfig(void)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiOdbReadCardConfig", 
         "ECalLiOdbReadCardConfig> Reading LI system configuration from ODB");
#endif

  int  portnu_array [kNumOfLIControlCards];
  char ipaddr_array [kNumOfLIControlCards][kHostNameStrLen];

  int size_int_array = sizeof(portnu_array);
  int size_str_array = sizeof(ipaddr_array);

  int status;

  status = db_get_value(
     gOdbHandle, 0, kODBpCtrlPortNum, portnu_array, &size_int_array, TID_INT, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbReadCardConfig", "Can't read the LI card port numbers.");
    return kODBReadErr;
  }
  status = db_get_value(
     gOdbHandle, 0, kODBpCtrlIPAddress, ipaddr_array, &size_str_array, TID_STRING, TRUE);
  if(status != DB_SUCCESS) {
    cm_msg(MERROR,"ECalLiOdbReadCardConfig", "Can't read the LI card host names.");
    return kODBReadErr;
  }

#ifdef _ECALLI_LOW_LEVEL_DEBUG_
  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {
     cm_msg(MDEBUG,"ECalLiOdbReadCardConfig", 
            "ECalLiOdbReadCardConfig>    Read from ODB: %s:%d", 
            ipaddr_array[icard], portnu_array[icard]);
     cm_msg(MDEBUG,"ECalLiOdbReadCardConfig", 
            "ECalLiOdbReadCardConfig>    strlen = %d", 
            strlen(ipaddr_array[icard]));
  }
#endif

  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {

     int connected = ( strlen(ipaddr_array[icard]) == 0 ) ? 0 : 1;
 
     if(connected==1) {
        strcpy(gLISystem.Card[icard].IPAddress, ipaddr_array[icard]);
        gLISystem.Card[icard].PortNum              = portnu_array[icard]; 
        gLISystem.Card[icard].Socket               = -1;
        gLISystem.Card[icard].IsConnected          =  1;
        gLISystem.Card[icard].CurrFlashSeqSetting  =  0;
     } else {
        strcpy(gLISystem.Card[icard].IPAddress, "\0");
        gLISystem.Card[icard].PortNum              =  0; 
        gLISystem.Card[icard].Socket               = -1;
        gLISystem.Card[icard].IsConnected          =  0;
        gLISystem.Card[icard].CurrFlashSeqSetting  =  0;
     }
  }

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO,"ECalLiOdbReadCardConfig", "ECalLiOdbReadCardConfig> Done reading LI configuration!");
#endif

  return kODBReadOK;
}


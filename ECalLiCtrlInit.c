/*
--------------------------------------------------------------------------------------------
\name   ECalLiCtrlInit

\brief  Initialize the ECAL LI controller

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   06/05/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ECalLiBuildInfo.h"
#include "ECalLiGlobals.h"
#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

#include <midas.h>

/*
 * init globals
 */
HNDLE                gOdbHandle;
ECalLiSystem_t       gLISystem;
ECalLiFlashSeq_t     gCurrFlashSequence;
ECalLiFlashSeq_t     gNextFlashSequence;
int                  gCtrlStatus = kCtrlOK;

int ECalLiCtrlInit(char * cmdln_midas_host, char * cmdln_midas_exp)
{
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit", "ECalLiCtrlInit> ECAL LI controller initialization");
#endif

  /*
   * Get default values from environment
   * (or use the values parsed from the cmd-line, if any)
   */
  char midas_host[kCmdStrLen];
  char midas_exp [kCmdStrLen];

  cm_get_environment(midas_host, kCmdStrLen, midas_exp, kCmdStrLen);

  if ( strlen(cmdln_midas_host) > 0 ) { strcpy(midas_host, cmdln_midas_host);  }
  if ( strlen(cmdln_midas_exp ) > 0 ) { strcpy(midas_exp,  cmdln_midas_exp);   }

  /*
   * Connect to the MIDAS experiment
   */
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit", 
         "ECalLiCtrlInit> Connecting to MIDAS experiment %s:%s", midas_host, midas_exp);
#endif
  char midas_client_name [kCmdStrLen];
  sprintf(midas_client_name,"%s",kMidasClientName);
  int exp_status = cm_connect_experiment(midas_host, midas_exp, midas_client_name, NULL);
  if (exp_status != CM_SUCCESS){
    cm_msg(MERROR, "ECalLiCtrlInit", "MIDAS' cm_connect_experiment() failed!");
    gCtrlStatus = kCtrlErrMidasConx;
    return kLIFail;
  }
#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit", "ECalLiCtrlInit> Done connecting to MIDAS experiment!");
#endif

  /*
   * Connect to the ODB
   */
#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit", "ECalLiCtrlInit> Connecting to ODB");
#endif
  int odb_status = ECalLiOdbConnect();
  if(odb_status != kLISuccess) {
    cm_msg(MERROR, "ECalLiCtrlInit", "Connecting to ODB failed! \n");
    gCtrlStatus = kCtrlErrOdbConx;
    return kLIFail;
  }
#ifdef _ECALLI_MID_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit","ECalLiCtrlInit> Done connecting to ODB!");
#endif

  /*
   * Read the LI card confifuration (IP addresses, port numbers, ...)
   */
  ECalLiSystemReset();
  int config_read_status = ECalLiOdbReadCardConfig();
  if(config_read_status != kODBReadOK) {
    cm_msg(MERROR, "ECalLiCtrlInit", "Can not read the LI card configuration!");
    gCtrlStatus = kCtrlErrCardConfigRead;
    return kLIFail;
  }

  /*
   * Establish TCP/IP connection with the pulser
   */
  for(int icard = 0; icard < kNumOfLIControlCards; icard++) {

    if(gLISystem.Card[icard].IsConnected == 0) continue;

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
    cm_msg(MINFO, "ECalLiCtrlInit", 
           "ECalLiCtrlInit> Establishing connection with the LI card at %s:%d ... ", 
           gLISystem.Card[icard].IPAddress, gLISystem.Card[icard].PortNum);
#endif

    int socket = ECalLiTCPConnect2Serv(gLISystem.Card[icard].IPAddress, gLISystem.Card[icard].PortNum);
    if(socket < 0) {
       cm_msg(MERROR, "ECalLiCtrlInit", "Connection with the LI control card %d failed.", icard);
       gCtrlStatus = kCtrlErrTcpIpConx;
       return kLIFail;
    }
    gLISystem.Card[icard].Socket = socket;

  } /* loop over cards */

  ECalLiSystemConfigPrint();

  /*
   * Reset flash sequences
   */
  ECalLiFlashSeqReset(&gCurrFlashSequence);
  ECalLiFlashSeqReset(&gNextFlashSequence);

#ifdef _ECALLI_HIGH_LEVEL_DEBUG_
  cm_msg(MINFO, "ECalLiCtrlInit", "ECalLiCtrlInit> Done initializing LI client!");
#endif

  /*
   * Set status
   */

  gCtrlStatus = kCtrlOK;

  return kLISuccess;
}


/*
--------------------------------------------------------------------------------------------
\name   appECalLiPulserController

\brief  ECAL LI controller

\usage  pulser_controller [-o MIDAS_ODB_server_hostname] [-e MIDAS_experiment_name]

        [] is an optional argument

\author Costas Andreopoulos <costas.andreopoulos \@ stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <midas.h>
#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     

#include "ECalLiBuildInfo.h"
#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"
#include "ECalLiPrototypes.h"

int gRunStarted = 0;

/* 
 * Command line arguments 
 */
char gCmdMidasHost [kCmdStrLen];
char gCmdMidasExp  [kCmdStrLen];

/* 
 * Functions
 */
int  GetCmdLineArgs(int argc, char *argv[]);
void PrintSyntax   (void);

/*
--------------------------------------------------------------------------------------------
Functions expected by MIDAS's cm_register_transition()
--------------------------------------------------------------------------------------------
 */
INT start(INT run_number, char *error)
{
  gRunStarted = 1;
  if( ECalLiCtrlRunStart(run_number) == kLISuccess) return CM_SUCCESS;
  return 2;
}
/*
--------------------------------------------------------------------------------------------
LI controller main()
--------------------------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
  /*
   * Read command line arguments
   */
  if (GetCmdLineArgs(argc, argv) != 0) {
    cm_msg(MERROR, "controller", "Invalid usage!");
    PrintSyntax();
    return 1;
  }

  /* 
   * Initialize controller
   */
  if (ECalLiCtrlInit(gCmdMidasHost, gCmdMidasExp) != kLISuccess) {
    cm_msg(MERROR, "controller", "Intialization failed!");
    return 1;
  }

  /*
   * Register for transition: Respond at the start of each run
   */
  cm_register_transition(TR_START, start, kMidasSequenceNum);


  /*
   * Periodically, query the LI control card for its flashing progress.
   * Only do so after the 1st run start.
   */
  int status;
  do {    
     status = cm_yield(kMidasCMYieldTimeOut);

     if(!gRunStarted) continue;

    /*
     * Query flashing progress
     */
#ifdef _ECALLI_MID_LEVEL_DEBUG_
     cm_msg(MINFO, "controller", 
          "controller> Querying LI cards for their flashing progress.");
#endif
     for(int icard = 0; icard < kNumOfLIControlCards; icard++) {
        if(gLISystem.Card[icard].IsConnected == 0) continue;
        int query_status = ECalLiQueryFlashProgress(icard);
        if(query_status != kCardOK) {
            cm_msg(MERROR, "controller", 
                   "LI card %d doesn't respond to progress query! \n", icard);
        }
     }/* loop over cards */

    /*
     * Print
     */
     ECalLiSystemStatePrint();
       
    /*
     * Update ECAL LI status at ODB
     */
     ECalLiOdbWriteState();

  } while (status != RPC_SHUTDOWN &&
           status != SS_ABORT);

  /*
   *  Shut-down: Close socket, disconnect experiment, clean-up ...
   */
  ECalLiCtrlShutDown();

  return 0;
}
/*----------------------------------------------------------------------------------------*/
int GetCmdLineArgs(int argc, char *argv[])
{
  strcpy(gCmdMidasExp,  "");  
  strcpy(gCmdMidasHost, "");  

  // parse command line parameters
  for (int i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      if (i+1 >= argc || argv[i+1][0] == '-') {
        return 1;
      }
      if      (argv[i][1] == 'e') { strcpy(gCmdMidasExp,   argv[++i]);  }
      else if (argv[i][1] == 'o') { strcpy(gCmdMidasHost,  argv[++i]);  }
      else {
        return 1;
      }
    }
  }

  return 0;
}
/*----------------------------------------------------------------------------------------*/
void PrintSyntax(void)
{
  cm_msg(MINFO, "controller", 
         "Usage: pulser_controller[-o MIDAS_ODB_server_hostname] [-e MIDAS_expt_name]\n");
}
/*----------------------------------------------------------------------------------------*/


/*
--------------------------------------------------------------------------------------------
\name   appECalLiPulserEmulator

\brief  A pulser emulator

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>
#include <time.h>

#include "ECalLiDefs.h"
#include "ECalLiPrototypes.h"

/* command-line options */
unsigned short opt_serv_port;  /* server port */
int opt_nsettings;             /* number of settings (for reporting "progress" within valid range) */
int opt_ntriggers;             /* number of flashes per setting (for reporting "progress" within valid range) */

int  GetCmdLineArgs(int argc, char *argv[]);
void PrintSyntax   (void);

int main(int argc, char *argv[])
{
  if (GetCmdLineArgs(argc, argv) != 0) {
    printf("emulator> Invalid usage! \n");
    PrintSyntax();
    return 1;
  }

/*
  srand( time(NULL) );
*/

  int serv_sock = ECalLiTCPCreateServSock(opt_serv_port);

  while(1) {
    printf("emulator> Waiting for a new client...\n\n");
    int clnt_sock = ECalLiTCPAcceptClient(serv_sock);

    while(1) {
 	unsigned char mesg[kMesgSize];
 	memset(mesg, 0, kMesgSize);

        /* Receive next mesg  */
  	int recv_status = ECalLiTCPRecv(clnt_sock, mesg, kMesgSize);
        if(recv_status == kTCPRecvErr) {
           printf("emulator> *** TCP Recv Error. Dicsonnecting the client.  \n");
           break;        
        }
        ECalLiPrintByteField16(mesg);

        /*
         * Handle the received message
         */

        /* 
         * Err handling test: 
         * Decide whether to pretend that the mesg was never received... 
         */
        double pi = rand() / ((double) RAND_MAX + 1);
        int ignore = (pi < kEmuMesgDropRate) ? 1 : 0;
        if(ignore) {
           printf("emulator> *** Viva la revolucion!!  \n");
           printf("emulator> *** Pretending that the LI card ignored/lost the mesg...\n");
           printf("emulator> *** Testing whether the controller will handle the error...\n");
           continue;
        }

        /* Determine mesg type */
        unsigned short int header = ECalLiToUShort(mesg[0], mesg[1]);

        /* 
         * Handle ping messages / send back a ping response 
         */
        if(header == kPingMesg) {
           printf("emulator> ** Recvd mesg is a 'ping' signal...\n");
           printf("emulator> Sending a 'ping response'...\n");
           unsigned char reply[kMesgSize];
           reply[0] = ECalLiToMSByte(kPingMesgResponse);
           reply[1] = ECalLiToLSByte(kPingMesgResponse);
      	   ECalLiTCPSend(clnt_sock, reply, kMesgSize);
        }

        /* 
         * Handle progress query messages
         */
        if(header == kFlashProgressMesg) {
           printf("emulator> ** Recvd mesg is a 'progress query'...\n");
           printf("emulator> Sending a 'progress query response'...\n");
           unsigned char reply[kMesgSize];
           /*
            * add expected header
            */
           reply[0] = ECalLiToMSByte(kFlashProgressResponse);
           reply[1] = ECalLiToLSByte(kFlashProgressResponse);
           /* 
            * pretend we're already done flashing at a few settings
            */
           unsigned short int curr_setid = (unsigned short int) (opt_nsettings * (rand() / ((double) RAND_MAX + 1)));
           unsigned short int curr_ntrig = (unsigned short int) (opt_ntriggers * (rand() / ((double) RAND_MAX + 1)));
           printf("emulator> *** Fake flashing progress - Current setting = %u \n", curr_setid);
           printf("emulator> *** Fake flashing progress - Num trig at current setting = %u \n", curr_ntrig);
           reply[2] = ECalLiToMSByte(curr_setid);
           reply[3] = ECalLiToLSByte(curr_setid);
           reply[4] = ECalLiToMSByte(curr_ntrig);
           reply[5] = ECalLiToLSByte(curr_ntrig);
           /*
            * send response
            */
      	   ECalLiTCPSend(clnt_sock, reply, kMesgSize);
        }

        /* Handle flash sequence messages 
         * Echo the flash sequence back for err checking 
         * Every now and then introduce an error to see whether the controller will handle it
         */
        if(header == kNewFlashSeq || header == kResentFlashSeq) {
           if(header == kNewFlashSeq) {
               printf("emulator> ** Recvd mesg is a new 'flash sequence'...\n");
           } else {
               printf("emulator> ** Recvd mesg is a 'flash sequence' sent again after a transmission err...\n");
           }

           printf("emulator> Echoing-back the flash sequence ...\n");

           /* 
            * Error handling test: Decide whether to introduce an err at the mesg echoed back
            */
            pi = rand() / ((double) RAND_MAX + 1);
            int adderr = (pi < kEmuMesgErrRate) ? 1 : 0;
            if(adderr) {
               printf("emulator> *** Introducing an error at the mesg echoed-back...\n");
               printf("emulator> *** Testing whether the controller will handle the error...\n");
               mesg[2]++;
               mesg[9]--;
            }

      	   ECalLiTCPSend(clnt_sock, mesg, kMesgSize);
        }

    }// 2nd w(1)
  }//1st w(1)

  close(serv_sock);
  return 0;
}
/*----------------------------------------------------------------------------------------*/
int GetCmdLineArgs(int argc, char *argv[])
{
  // parse command line parameters
  for (int i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      if (i+1 >= argc || argv[i+1][0] == '-') {
        return 1;
      }
      if       (argv[i][1] == 'p') { opt_serv_port = atoi(argv[++i]);  }
      else if  (argv[i][1] == 's') { opt_nsettings = atoi(argv[++i]);  }
      else if  (argv[i][1] == 't') { opt_ntriggers = atoi(argv[++i]);  }
      else {
        return 1;
      }
    }
  }
  if(opt_serv_port==0) return 1;


  return 0;
}
/*----------------------------------------------------------------------------------------*/
void PrintSyntax(void)
{
  printf("Usage: pulser_emulator -p pulser_port -s nsettings -t ntriggs\n\n");
}
/*----------------------------------------------------------------------------------------*/


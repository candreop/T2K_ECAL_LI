/*
--------------------------------------------------------------------------------------------
\name   ECalLiPrototypes

\brief  ECAL LI controller softw: Function propotypes

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_PROTOTYPES_H_
#define _ECALLI_PROTOTYPES_H_

#include <arpa/inet.h>  

#include "ECalLiBuildInfo.h"
#include "ECalLiFlashSeq.h"
#include "ECalLiDefs.h"

#include <midas.h>

/*
 * TCP/IP functions
 */ 
int ECalLiTCPCreateServSock (int serv_port);
int ECalLiTCPAcceptClient   (int serv_sock);
int ECalLiTCPConnect2Serv   (char* serv_ip, int serv_port);
int ECalLiTCPRecv           (int sock, char * mesg, int len);
int ECalLiTCPRecv           (int sock, unsigned char * mesg, int len);
int ECalLiTCPSend           (int sock, char * mesg, int len);
int ECalLiTCPSend           (int sock, unsigned char * mesg, int len);

/*
 * Controller functions
 */ 
int  ECalLiCtrlInit           (char * cmdln_midas_host, char * cmdln_midas_exp);
void ECalLiCtrlShutDown       (void);
int  ECalLiCtrlRunStart       (int run_nu = 0);
int  ECalLiPingCard           (int icard);
int  ECalLiQueryFlashProgress (int icard);

/*
 * Flash sequence functions
 */ 
void ECalLiFlashSeqReset   (ECalLiFlashSeq_t * flseq);
void ECalLiFlashSeqPrint   (ECalLiFlashSeq_t * flseq);
void ECalLiFlashSeqSend    (ECalLiFlashSeq_t * flseq, unsigned short int icard, int first_setting_id);
void ECalLiFlashSeqMesg    (ECalLiFlashSeq_t * flseq, unsigned short int icard, int first_setting_id, int n_failed_prev_attempts, unsigned char *mesg);
void ECalLiFlashSeqCopy    (ECalLiFlashSeq_t * source, ECalLiFlashSeq_t * target);
int  ECalLiFlashSeqComp    (ECalLiFlashSeq_t * flseq0, ECalLiFlashSeq_t * flseq1);
int  ECalLiFlashSeqComp    (ECalLiFlashSeq_t * flseq0, ECalLiFlashSeq_t * flseq1, unsigned short int icard);

/*
 * ODB functions
 */ 
int  ECalLiOdbConnect                (void);
int  ECalLiOdbReadCardConfig         (void);
int  ECalLiOdbReadNextFlashSeq       (void);
int  ECalLiOdbWriteState             (void);

/*
 * LI card config 
 */ 
void ECalLiSystemReset       (void);
void ECalLiSystemConfigPrint (void);
void ECalLiSystemStatePrint  (void);

/*
 * Utilities
 */ 
unsigned char      ECalLiPatternId        (char * str);
unsigned short int ECalLiToUShort         (unsigned char msbyte, unsigned char lsbyte);
unsigned       int ECalLiToUInt           (unsigned short int ms, unsigned short int ls);
unsigned long int  ECalLiToULong          (unsigned short int ms, unsigned short int ls);
unsigned char      ECalLiToMSByte         (unsigned short int us);
unsigned char      ECalLiToLSByte         (unsigned short int us);
void               ECalLiPrintByteField16 (unsigned char * mesg, const char * title = 0);

#endif

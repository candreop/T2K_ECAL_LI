/*
--------------------------------------------------------------------------------------------
\name   ECalLiDefs

\brief  ECal LI definitions

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   19/03/2008
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_DEFS_H_
#define _ECALLI_DEFS_H_

/*
 * MIDAS
 * .........................................................................................
 */

/* sequence num used in MIDAS' cm_register_transition() */
#define kMidasSequenceNum 400   

/* client name used in MIDAS' cm_connect_experiment()   */
#define kMidasClientName "ECALLI"  

/* timeout MIDAS' cm_yield() central yield function for clients [in millisec] */
#define kMidasCMYieldTimeOut 100000

/*
 * ODB paths etc
 * .........................................................................................
 */

/* Enable/disable use of BYTE type */
#define kODBNoBYTE

/* LI system configuration */
#define kODBpCtrlIPAddress           "/Config/ECAL/LI/Controller/IPAddress"
#define kODBpCtrlPortNum             "/Config/ECAL/LI/Controller/Port"

/* flash sequence */
#define kODBpSeqNF                   "/Config/ECAL/LI/Sequence/NFlashesPerSetting"
#define kODBpSeqNA                   "/Config/ECAL/LI/Sequence/NTrigAppended"
#define kODBpSeqNS                   "/Config/ECAL/LI/Sequence/NSettings"
#define kODBpSeqAmplitude            "/Config/ECAL/LI/Sequence/Amplitude"
#define kODBpSeqPattern              "/Config/ECAL/LI/Sequence/Pattern"
#define kODBpSeqPulserMask           "/Config/ECAL/LI/Sequence/PulserMask"

/* LI state */
#define kODBpStateIsActive           "/State/ECAL/LI/IsActive"
#define kODBpStateCurrSet            "/State/ECAL/LI/CurrSetting"
#define kODBpStateCurrAmplitude      "/State/ECAL/LI/CurrAmplitude"
#define kODBpStateCurrPattern        "/State/ECAL/LI/CurrPattern"
#define kODBpStateCurrNTrigInSet     "/State/ECAL/LI/CurrNTrigInSetting"
#define kODBpStateCurrNTrigTot       "/State/ECAL/LI/CurrNTrigTotal"
#define kODBpStateGlobCurrSet        "/State/ECAL/LI/GlobalCurrSetting"
#define kODBpStateGlobCurrNTrigInSet "/State/ECAL/LI/GlobalCurrNTrigInSetting"

/*
 * LI system
 * .........................................................................................
 */

/* number of LI control cards */
#define kNumOfLIControlCards 8

/* max allowed difference in number of flashed triggers reported by each card */
#define kMaxNTrigSpread 50

/* 
 * Flash sequences
 * .........................................................................................
 */

/* max number of settings in a flash sequence */
#define kMaxNumOfFlashSeqSet 50                          

/* max number of attempts to send the sequence (in case the mesg is echoed back with errs)  */
#define kMaxNumOfFlashSeqErrMesgs 3       

/* flash sequence mesg size */
#define kFlashSeqMesgSize (kMaxNumOfFlashSeqSet+4)*2

/* headers for flash sequence TCP/IP mesgs */
#define kNewFlashSeq      0x0000
#define kResentFlashSeq   0x0001

/* flash sequence comparison results */
#define kFlashSeqCompErr            0 /* error in comparison    */
#define kFlashSeqCompIdentical      1 /* sequences same         */
#define kFlashSeqCompDifferent      2 /* sequences not the same */

/*
 * TCP/IP
 * .........................................................................................
 */

/* define string length for host names */
#define kHostNameStrLen 16

/* use TCP_NODELAY? */
#define kUseTCP_NODELAY 1                 

/* TCP socket 'receive' timeout (sec) */
#define kTCPRecvTimeout 10                 

/* number of pending client mesgs that can be queued for a socket */
#define kNMaxPendingMesg 5                 

/* size of the buffer sent/received over the TCP/IP socket */
#define kMesgSize kFlashSeqMesgSize 

/* max allowed number of recv() calls fetching 0 bytes  */
#define kTCPMaxNumOfZeroByteRecv 10                 

/* 
 * LI control card 'ping'
 * .........................................................................................
 */

/* number of (ignored) pings attempted before going on err */
#define kMaxNumOfPings 5       

#define kPingMesg            0xFFFF  /* 'ping' mesg */
#define kPingMesgResponse    0xF0F0  /* 'ping response' mesg */

/* 
 * LI control card 'flashing progress query'
 * .........................................................................................
 */

/* number of (ignored) queries before going on err */
#define kMaxNumOfFlashProgressQueries 5       

#define kFlashProgressMesg      0xEEEE  /* flash progress query mesg */
#define kFlashProgressResponse  0xE0E0  /* expected response to flash query mesg */

/* 
 * Bit-field masks
 * .........................................................................................
 */
#define k16bitMask_8LS 0xFF   
#define k16bitMask_8MS 0xFF00  

/* 
 * Debug-flags
 * .........................................................................................
 */

/* print-out byte arrays sent/received over TCP/IP ? (0:no, 1:yes. Note: prolific output) */
#define kPrintOutByteFields 0

/* print-out byte number of bytes sent/recved over TCP/IP (0:no, 1:yes) */
#define kPrintOutByteCounts 0

/* set 0 to disable pinging the LI control card at the beginning of each run */
#define kPingLIControlCard 1       

/* set 0 to querying the LI card progress */
#define kQueryFlashProgress 1       

/* enable/disable debug print-out */
//#define _ECALLI_LOW_LEVEL_DEBUG_
//#define _ECALLI_MID_LEVEL_DEBUG_
//#define _ECALLI_HIGH_LEVEL_DEBUG_


/* 
 * Pulser emulator settings for testing error handling
 * .........................................................................................
 */

/* how often messages are to be "dropped"?  */
#define kEmuMesgDropRate 0    

/* how often errors are to be introduced in the sequences transmitted over TCP/IP? */ 
#define kEmuMesgErrRate  0

/* 
 * Return and Error codes
 * .........................................................................................
 */

/* controller return codes */
#define kCtrlOK                  0  
#define kCtrlErrTcpIpConx        1  /* error in TCP/IP connection           */
#define kCtrlErrMidasConx        2  /* error in connection with MIDAS expt  */
#define kCtrlErrOdbConx          3  /* error in connection with ODB         */
#define kCtrlErrCardConfigRead   4  /* error reading the LI system config   */
#define kCtrlErrFlashSeqRead     5  /* error reading the flash sequence     */
#define kCtrlErrEtc              6  /* etc error                            */

/* ECalLiCtrl<> func return codes */
#define kLIFail                  0
#define kLISuccess               1

/* TCP/IP send/recv return codes */
#define kTCPSendOK               0
#define kTCPSendErr              1
#define kTCPRecvOK               0
#define kTCPRecvErr              1

/* ODB read/write returm codes */
#define kODBReadOK               0
#define kODBReadErr              1
#define kODBWriteOK              0
#define kODBWriteErr             1

/* LI card ping and progress query return codes */
#define kCardOK                  0
#define kCardErr                 1

/* 
 * Etc
 * .........................................................................................
 */

/* string length for command line arguments */
#define kCmdStrLen 512

#endif

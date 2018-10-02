/*
--------------------------------------------------------------------------------------------
\name   ECalLiCard

\brief  Store info for an LI card

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   15/02/2010
--------------------------------------------------------------------------------------------
*/

#ifndef _ECALLI_CARD_H_
#define _ECALLI_CARD_H_

#include "ECalLiDefs.h"

typedef struct
{
  /*
   *  IP address
   */ 
  char IPAddress [kHostNameStrLen];

  /* 
   * port number  
   */
  unsigned short PortNum; 

  /* 
   * TCP/IP socket                 
   */
  int Socket;

  /*
   *  Flag indicating whether the card is connected to the system
   */ 
  unsigned int IsConnected;

  /*
   *  Flag indicating whether a connected card is active
   */ 
  unsigned int IsActive;

  /*
   *  State (as reported by the card): 
   *  - Current flash sequence setting
   *  - Amplitude for current setting
   *  - Flashing pattern for current setting
   *  - Number of triggers flashed in current setting
   *  - Current total number of triggers since the start of run
   */ 
  unsigned short int CurrFlashSeqSetting;
  unsigned short int CurrAmplitude;
  unsigned short int CurrPattern;
  unsigned short int CurrNTrigInSetting;
  unsigned       int CurrNTrigTotal;

} 
ECalLiCard_t;

#endif



/*
--------------------------------------------------------------------------------------------
\name   ECalLiOdbConnect

\brief  Get a handle to the experiment dbase

\author Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
        STFC, Rutherford Appleton Lab.

\date   06/05/2008
--------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

#include "ECalLiPrototypes.h"
#include "ECalLiDefs.h"
#include "ECalLiGlobals.h"

#include <midas.h>

int ECalLiOdbConnect(void)
{
  HNDLE *hKeyClient;
  hKeyClient = 0;
  if( CM_SUCCESS != cm_get_experiment_database(&gOdbHandle, hKeyClient)){
    return kLIFail;
  }
  return kLISuccess;
}

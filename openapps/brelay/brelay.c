/**
\brief An example CoAP application.
*/

#include "opendefs.h"
#include "opentimers.h"
#include "msp430f1611.h"
#include "brelay.h"

//=========================== defines =========================================

/// inter-packet period (in ms)
#define BRELAYPERIOD     10000

//=========================== variables =======================================

//=========================== prototypes ======================================


//=========================== public ==========================================

void brelay_init() {
  P2SEL &= (~BIT0); // Set P2.0 as GPIO
  P2DIR &= (BIT0); // Set P2.0 as Output
  P2OUT = 0x01; // Set it
}

//=========================== private =========================================

#ifndef __BPIR_H
#define __BPIR_H

/**
\addtogroup AppUdp
\{
\addtogroup ctest
\{
*/
#include "opencoap.h"
//=========================== define ==========================================

//=========================== typedef =========================================

typedef struct {
   coap_resource_desc_t desc;
} bpir_vars_t;

//=========================== variables =======================================

//=========================== prototypes ======================================

void bpir_init(void);

/**
\}
\}
*/

#endif

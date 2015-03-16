#ifndef __CTEST_H
#define __CTEST_H

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
   opentimer_id_t       timerId;
} ctest_vars_t;

//=========================== variables =======================================

//=========================== prototypes ======================================

void ctest_init(void);

/**
\}
\}
*/

#endif

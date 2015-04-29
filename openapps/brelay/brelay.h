#ifndef __BRELAY_H
#define __BRELAY_H

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
   opentimer_id_t       tId;
} brelay_vars_t;

//=========================== variables =======================================

//=========================== prototypes ======================================

void brelay_init(void);

/**
\}
\}
*/

#endif

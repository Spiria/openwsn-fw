/*
 * dtls.h
 *
 *  Created on: Feb 25, 2015
 *      Author: Yeohwan
 */

#ifndef __DTLS_H
#define __DTLS_H

#include "opendefs.h"

void dtls_init(void);
bool is_dtls_packet(OpenQueueEntry_t *recv);
OpenQueueEntry_t* dtls_receive(OpenQueueEntry_t* recv);



#endif /* __DTLS_H */

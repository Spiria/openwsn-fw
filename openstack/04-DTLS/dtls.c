#include "../04-DTLS/dtls.h"

#include "opendefs.h"
#include "openudp.h"
#include "openqueue.h"
#include "openserial.h"
#include "packetfunctions.h"

void dtls_init(){

}

bool is_dtls_packet(OpenQueueEntry_t *recv){
	return recv->l4_dtls_enabled?TRUE:FALSE;
}

OpenQueueEntry_t* dtls_receive(OpenQueueEntry_t* recv){
	// Just print Debug message now.
	if(recv == NULL){
		openserial_printError(
				COMPONENT_DTLS,
				ERR_INVALIDPACKETFROMRADIO,
				(errorparameter_t)0,
				(errorparameter_t)0
				);
		openqueue_freePacketBuffer(recv);
		return recv;
	}

	openserial_printInfo(
			COMPONENT_DTLS,
			ERR_DTLS_PACKET_RECEIVED,
			(errorparameter_t)0,
			(errorparameter_t)0
			);
	return recv;
}

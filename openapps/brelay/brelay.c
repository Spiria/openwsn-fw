/**
\brief An example CoAP application.
*/

#include "opendefs.h"
#include "opentimers.h"

// CoAP Control
#include "opencoap.h"

// Relay Control
#include "msp430f1611.h"
#include "brelay.h"

//=========================== defines =========================================

//=========================== variables =======================================
brelay_vars_t brelay_vars;
// Path
const uint8_t brelay_path0[] = "relay";
int relay_status = 0;

//=========================== prototypes ======================================
owerror_t brelay_receive(
   OpenQueueEntry_t* msg,
   coap_header_iht*  coap_header,
   coap_option_iht*  coap_options
   );
void    brelay_sendDone(OpenQueueEntry_t* msg,
                       owerror_t error);


//=========================== public ==========================================

void brelay_init() {
  P2SEL &= (~BIT0); // Set P2.0 as GPIO
  P2DIR &= (BIT0); // Set P2.0 as Output
  P2OUT = 0x00; // Disconnect at first (Low : Disconnect, High: Connect)
  relay_status = 0; // Disconnected

   // prepare the resource descriptor for the /ex path
   brelay_vars.desc.path0len             = sizeof(brelay_path0)-1;
   brelay_vars.desc.path0val             = (uint8_t*)(&brelay_path0);
   brelay_vars.desc.path1len             = 0;
   brelay_vars.desc.path1val             = NULL;
   brelay_vars.desc.componentID          = COMPONENT_BRELAY;
   brelay_vars.desc.callbackRx           = &brelay_receive;
   brelay_vars.desc.callbackSendDone     = &brelay_sendDone;

   // Register with the CoAP Module
   opencoap_register(&brelay_vars.desc);
}
//=========================== private =========================================
/**
\brief Called when a CoAP message is received for this resource.

\param[in] msg          The received message. CoAP header and options already
   parsed.
\param[in] coap_header  The CoAP header contained in the message.
\param[in] coap_options The CoAP options contained in the message.

\return Whether the response is prepared successfully.
*/
owerror_t brelay_receive(
      OpenQueueEntry_t* msg,
      coap_header_iht*  coap_header,
      coap_option_iht*  coap_options
   ) {
   owerror_t outcome;

   switch (coap_header->Code) {
      case COAP_CODE_REQ_GET:
         // reset packet payload
         msg->payload                     = &(msg->packet[127]);
         msg->length                      = 0;

         // add CoAP payload
         packetfunctions_reserveHeaderSize(msg,2);
         msg->payload[0]                  = COAP_PAYLOAD_MARKER;

         if (relay_status==1) {
            msg->payload[1]               = '1';
         } else {
            msg->payload[1]               = '0';
         }

         // set the CoAP header
         coap_header->Code                = COAP_CODE_RESP_CONTENT;

         outcome                          = E_SUCCESS;
         break;

      case COAP_CODE_REQ_PUT:

         // change the LED's state
         if (msg->payload[0]=='E') {
           P2OUT = 0x01; // (Low : Disconnect, High: Connect)
           relay_status = 1;
         } else if (msg->payload[0]=='D') {
           P2OUT = 0x00; // (Low : Disconnect, High: Connect)
          relay_status = 0;
         }

         // reset packet payload
         msg->payload                     = &(msg->packet[127]);
         msg->length                      = 0;

         // set the CoAP header
         coap_header->Code                = COAP_CODE_RESP_CHANGED;

         outcome                          = E_SUCCESS;
         break;

      default:
         outcome                          = E_FAIL;
         break;
   }

   return outcome;
}

/**
\brief The stack indicates that the packet was sent.

\param[in] msg The CoAP message just sent.
\param[in] error The outcome of sending it.
*/
void brelay_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
   openqueue_freePacketBuffer(msg);
}

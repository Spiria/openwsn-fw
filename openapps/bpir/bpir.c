/**
\brief An example CoAP application.
*/

#include "opendefs.h"
#include "opentimers.h"

// CoAP Control
#include "opencoap.h"

// Relay Control
#include "msp430f1611.h"
#include "bpir.h"

//=========================== defines =========================================

//=========================== variables =======================================
bpir_vars_t bpir_vars;
// Path
const uint8_t bpir_path0[] = "pir";
int pir_status = 0;

//=========================== prototypes ======================================
owerror_t bpir_receive(
   OpenQueueEntry_t* msg,
   coap_header_iht*  coap_header,
   coap_option_iht*  coap_options
   );
void    bpir_sendDone(OpenQueueEntry_t* msg,
                       owerror_t error);


//=========================== public ==========================================

void bpir_init() {
   P2SEL |= 0x00; // Set P2.0 as GPIO
   P2DIR |= 0x00; // Set P2.0 as Input
   P2IES |= 0x00; // low-to-high triggering
   P2IFG &= (~BIT0);    //  Clear   interrupt   flag
   P2IE  |= (BIT0); //  Enable  interrupt   for P2.1
   pir_status = 0; // Disconnected

   // prepare the resource descriptor for the /ex path
   bpir_vars.desc.path0len             = sizeof(bpir_path0)-1;
   bpir_vars.desc.path0val             = (uint8_t*)(&bpir_path0);
   bpir_vars.desc.path1len             = 0;
   bpir_vars.desc.path1val             = NULL;
   bpir_vars.desc.componentID          = COMPONENT_BPIR;
   bpir_vars.desc.callbackRx           = &bpir_receive;
   bpir_vars.desc.callbackSendDone     = &bpir_sendDone;

   __enable_interrupt();  //  Enable  Global  Interrupts

   // Register with the CoAP Module
   opencoap_register(&bpir_vars.desc);
}
//=========================== private =========================================
// interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	pir_status = 1;
	P2IFG &= (~BIT0);    // IFG clear
}

/**
\brief Called when a CoAP message is received for this resource.

\param[in] msg          The received message. CoAP header and options already
   parsed.
\param[in] coap_header  The CoAP header contained in the message.
\param[in] coap_options The CoAP options contained in the message.

\return Whether the response is prepared successfully.
*/
owerror_t bpir_receive(
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

         if (pir_status==1) {
            msg->payload[1]               = '1';
            pir_status = 0; // Reset it.
         } else {
            msg->payload[1]               = '0';
         }

         // set the CoAP header
         coap_header->Code                = COAP_CODE_RESP_CONTENT;

         outcome                          = E_SUCCESS;
         break;

      case COAP_CODE_REQ_PUT:

         // change the LED's state
         /*if (msg->payload[0]=='E') {

         } else if (msg->payload[0]=='D') {

         }*/

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
void bpir_sendDone(OpenQueueEntry_t* msg, owerror_t error) {
   openqueue_freePacketBuffer(msg);
}

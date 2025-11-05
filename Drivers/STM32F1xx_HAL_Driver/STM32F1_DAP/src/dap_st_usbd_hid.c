#include <string.h>


#include "DAP_config.h"
#include "DAP.h"




// USB HID Callback: when data is received from the host
//void usbd_hid_set_report(uint8_t rtype, uint8_t rid, uint8_t *buf, int len, uint8_t req)
//{
//    uint8_t * rbuf;
//    switch (rtype) {
//        case HID_REPORT_OUTPUT:
//            if (len == 0) {
//                break;
//            }

//            if (buf[0] == ID_DAP_TransferAbort) {
//                DAP_TransferAbort = 1;
//                break;
//            }

//            // execute and store to DAP_queue
//            if (DAP_queue_execute_buf(&DAP_Cmd_queue, buf, len, &rbuf)) {
//                if (USB_ResponseIdle) {
//                    hid_send_packet();
//                    USB_ResponseIdle = 0;
//                }
//            } else {
//                util_assert(0);
//            }
//            break;

//        case HID_REPORT_FEATURE:
//            break;
//    }
//}

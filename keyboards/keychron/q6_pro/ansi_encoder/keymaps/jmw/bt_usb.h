#ifndef JMW_BT_USB_H
#define JMW_BT_USB_H

#include "transport.h"

void bt_usb_set_transport(transport_t t);
transport_t bt_usb_get_transport(void);

#endif
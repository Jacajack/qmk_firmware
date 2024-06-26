#include "quantum.h"
#include "bluetooth.h"
#include "indicator.h"
#if defined(PROTOCOL_CHIBIOS)
#    include <usb_main.h>
#endif
#include "transport.h"

#if defined(PROTOCOL_CHIBIOS)
extern host_driver_t chibios_driver;
#endif
extern host_driver_t   bluetooth_driver;

// This only commands where the shared driver sends reports
static transport_t bt_usb_transport = TRANSPORT_USB;

void bt_usb_set_transport(transport_t t)
{
    bt_usb_transport = t;
}

transport_t bt_usb_get_transport(void)
{
    return bt_usb_transport;
}

static uint8_t bt_usb_keyboard_leds(void)
{
    if (bt_usb_transport == TRANSPORT_USB && chibios_driver.keyboard_leds)
        return chibios_driver.keyboard_leds();

    if (bt_usb_transport == TRANSPORT_BLUETOOTH && bluetooth_driver.keyboard_leds)
        return bluetooth_driver.keyboard_leds();

    return 0;
}

static void bt_usb_send_keyboard(report_keyboard_t *rpt)
{
    if (bt_usb_transport == TRANSPORT_USB && chibios_driver.send_keyboard)
        chibios_driver.send_keyboard(rpt);

    if (bt_usb_transport == TRANSPORT_BLUETOOTH && bluetooth_driver.send_keyboard)
        bluetooth_driver.send_keyboard(rpt);
}

static void bt_usb_send_mouse(report_mouse_t *rpt)
{
    if (bt_usb_transport == TRANSPORT_USB && chibios_driver.send_mouse)
        chibios_driver.send_mouse(rpt);

    if (bt_usb_transport == TRANSPORT_BLUETOOTH && bluetooth_driver.send_mouse)
        bluetooth_driver.send_mouse(rpt);
}

static void bt_usb_send_extra(report_extra_t *rpt)
{
    if (bt_usb_transport == TRANSPORT_USB && chibios_driver.send_extra)
        chibios_driver.send_extra(rpt);

    if (bt_usb_transport == TRANSPORT_BLUETOOTH && bluetooth_driver.send_extra)
        bluetooth_driver.send_extra(rpt);
}

static host_driver_t bt_usb_host_driver = {
    .keyboard_leds = &bt_usb_keyboard_leds,
    .send_keyboard = &bt_usb_send_keyboard,
    .send_mouse = &bt_usb_send_mouse,
    .send_extra = &bt_usb_send_extra,
};

static void orig_usb_transport_enable(bool enable) {
    if (enable) {
        if (host_get_driver() != &chibios_driver) {

#if !defined(KEEP_USB_CONNECTION_IN_BLUETOOTH_MODE)
            usb_power_connect();
            usb_start(&USBD1);
#endif
            host_set_driver(&chibios_driver);
        }
    } else {
        if (USB_DRIVER.state == USB_ACTIVE) {
            report_keyboard_t empty_report = {0};
            chibios_driver.send_keyboard(&empty_report);
        }

#if !defined(KEEP_USB_CONNECTION_IN_BLUETOOTH_MODE)
        usbStop(&USBD1);
        usbDisconnectBus(&USBD1);
        usb_power_disconnect();
#endif
    }
}

static void orig_bt_transport_enable(bool enable) {
    if (enable) {
        if (host_get_driver() != &bluetooth_driver) {
            host_set_driver(&bluetooth_driver);

            /* Disconnect and reconnect to sync the bluetooth state
             * TODO: query bluetooth state to sync
             */
            bluetooth_disconnect();
            bluetooth_connect();
            // TODO: Clear USB report
        }
    } else {
        indicator_stop();

        if (bluetooth_get_state() == BLUETOOTH_CONNECTED) {
            report_keyboard_t empty_report = {0};
            bluetooth_driver.send_keyboard(&empty_report);
        }
    }
}

// Note: This overrides weak def in transport.c
void bt_transport_enable(bool enable)
{
    if (enable)
    {
        // Enter Bluetooth + USB mode
        bt_usb_transport = TRANSPORT_USB;
        orig_usb_transport_enable(true);
        orig_bt_transport_enable(true);
        host_set_driver(&bt_usb_host_driver);
    }
    else
    {
        // Enter USB-only mode
        orig_usb_transport_enable(true);
        orig_bt_transport_enable(false);
    }
}

// Note: This overrides weak def in transport.c
void usb_transport_enable(bool enable)
{
    // Hacky as fuck, but hear me out: bt_transport_enable() and 
    // usb_transport_enable() are always called in pair with
    // opposite arguments. We don't really need to handle both of them.
}
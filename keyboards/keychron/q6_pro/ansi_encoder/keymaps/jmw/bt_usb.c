#include <stdbool.h>
#include "lpm.h"
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

static bool bt_usb_running_on_battery(void)
{
    return get_transport() == TRANSPORT_BLUETOOTH && !usb_power_connected();
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

// Note: this overrides lpm_task() in lpm.c with --wrap option
void __real_lpm_task(void);
void __wrap_lpm_task(void)
{
    // In USB mode we make sure that LPM timer is always reset
    if (!bt_usb_running_on_battery())
        lpm_timer_reset();

    __real_lpm_task();
}


// Note this overrides battery_task() in battery.c with --wrap option
void __real_battery_task(void);
void __wrap_battery_task(void)
{
    static bool was_running_on_battery = false;
    bool running_on_battery = bt_usb_running_on_battery();
    bool power_just_connected = was_running_on_battery && !running_on_battery;

    if (running_on_battery || power_just_connected)
        __real_battery_task();

    was_running_on_battery = running_on_battery;
}

// Note this overrides indicator_task() in indicator.c with --wrap option
void __real_indicator_task(void);
void __wrap_indicator_task(void)
{
    if (!bt_usb_running_on_battery())
        rgb_matrix_disable_time_reset();

    __real_indicator_task();
}

// Note this overrides LED_DRIVER_SHUTDOWN_ALLOWED() in indicator.c with --wrap option
bool __real_rgb_matrix_driver_allow_shutdown(void);
bool __wrap_rgb_matrix_driver_allow_shutdown(void)
{
	return true;
}

// Overrides weak function in indicator.c
void os_state_indicate(void)
{
}

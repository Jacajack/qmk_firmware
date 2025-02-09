VIA_ENABLE = no
ENCODER_MAP_ENABLE = yes
COMBO_ENABLE = yes
NKRO_ENABLE = yes
MOUSEKEY_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes
DEBOUNCE_TYPE = sym_defer_pk

# Custom simultaneous BT/USB connection
SRC += bt_usb.c
EXTRAFLAGS += -Wl,--wrap=lpm_task -Wl,--wrap=battery_task -Wl,--wrap=indicator_task -Wl,--wrap=rgb_matrix_driver_allow_shutdown

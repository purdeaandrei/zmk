/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <hardware/structs/pads_qspi.h>
#include <hardware/gpio.h>
#include <zmk/event_manager.h>
#include <zmk/led_indicators.h>
#include <zmk/events/led_indicator_changed.h>
#include <zmk/keymap.h>

static int local_gpio_init(const struct device *port) {
    ARG_UNUSED(port);
    for (int gpio=0;gpio<NUM_BANK0_GPIOS;gpio++) {
        gpio_set_drive_strength(gpio, GPIO_DRIVE_STRENGTH_2MA);
    }

    for (int gpio=0;gpio<NUM_QSPI_GPIOS;gpio++) {
        pads_qspi_hw->io[gpio] &= ~((3 << 4) | (1 << 0));
    }
    return 0;
}

#ifndef CONFIG_DISABLE_NUMLOCK_TOGGLES_LAYER
static int numlock_listener(const zmk_event_t *eh) {
    const struct zmk_led_changed *ev = as_zmk_led_changed(eh);
    zmk_leds_flags_t leds = ev->leds;
    if (leds & ZMK_LED_NUMLOCK_BIT) {
        zmk_keymap_layer_activate(CONFIG_NUMLOCK_TOGGLES_THIS_LAYER);
    } else {
        zmk_keymap_layer_deactivate(CONFIG_NUMLOCK_TOGGLES_THIS_LAYER);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(numlock_listener, numlock_listener);
ZMK_SUBSCRIPTION(numlock_listener, zmk_led_changed);
#endif

SYS_INIT(local_gpio_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

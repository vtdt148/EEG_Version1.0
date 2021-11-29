#ifndef BOARD_EEG_H
#define BOARD_EEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

#define LEDS_NUMBER    1

//#define LED_1          NRF_GPIO_PIN_MAP(0,13) //EVK
//#define BUTTON_1       NRF_GPIO_PIN_MAP(1,6)  //DONGLE and EVK
#define LED_1          NRF_GPIO_PIN_MAP(1,1)    //EEG board
#define BUTTON_1       NRF_GPIO_PIN_MAP(0,11)   //EEG board


#define LEDS_ACTIVE_STATE 0
#define LEDS_LIST { LED_1 }
#define LEDS_INV_MASK  LEDS_MASK

#define BUTTONS_NUMBER 1

#define BUTTONS_LIST { BUTTON_1 }
#define BSP_BUTTON_0   BUTTON_1

#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5
#define HWFC           true

#ifdef __cplusplus
}
#endif

#endif // BOARD_EEG_H

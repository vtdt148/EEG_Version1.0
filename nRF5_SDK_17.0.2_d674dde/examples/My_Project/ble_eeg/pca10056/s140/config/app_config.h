
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// <e> SPI_ENABLED - nrf_drv_spi - SPI/SPIM peripheral driver - legacy layer
//==========================================================
#ifndef SPI_ENABLED
#define SPI_ENABLED 1
#endif
// <e> NRFX_SPI_ENABLED - nrfx_spi - SPI peripheral driver
//==========================================================
#ifndef NRFX_SPI_ENABLED
#define NRFX_SPI_ENABLED 1
#endif
// <e> NRFX_SPIM_ENABLED - nrfx_spim - SPIM peripheral driver
//==========================================================
#ifndef NRFX_SPIM_ENABLED
#define NRFX_SPIM_ENABLED 1
#endif
// <e> SPI0_ENABLED - Enable SPI0 instance
//==========================================================
#ifndef SPI0_ENABLED
#define SPI0_ENABLED 1
#endif

// <e> NRF_CLOCK_ENABLED - nrf_drv_clock - CLOCK peripheral driver - legacy layer
//==========================================================
#ifndef NRF_CLOCK_ENABLED
#define NRF_CLOCK_ENABLED 1
#endif
// <o> CLOCK_CONFIG_LF_SRC  - LF Clock Source
 
// <0=> RC 
// <1=> XTAL 
// <2=> Synth 
// <131073=> External Low Swing 
// <196609=> External Full Swing 

#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 0 // Tai change for Internal RC
#endif

//==========================================================
// <o> NRF_SDH_CLOCK_LF_SRC  - SoftDevice clock source.
 
// <0=> NRF_CLOCK_LF_SRC_RC 
// <1=> NRF_CLOCK_LF_SRC_XTAL 
// <2=> NRF_CLOCK_LF_SRC_SYNTH 

#ifndef NRF_SDH_CLOCK_LF_SRC
#define NRF_SDH_CLOCK_LF_SRC 0 //tai change for Internal RC
#endif

// <o> NRF_SDH_CLOCK_LF_RC_CTIV - SoftDevice calibration timer interval. 
#ifndef NRF_SDH_CLOCK_LF_RC_CTIV
#define NRF_SDH_CLOCK_LF_RC_CTIV 16// 0 //tai change for Internal RC
#endif

// <o> NRF_SDH_CLOCK_LF_RC_TEMP_CTIV - SoftDevice calibration timer interval under constant temperature. 
// <i> How often (in number of calibration intervals) the RC oscillator shall be calibrated
// <i>  if the temperature has not changed.

#ifndef NRF_SDH_CLOCK_LF_RC_TEMP_CTIV
#define NRF_SDH_CLOCK_LF_RC_TEMP_CTIV 2//0 //tai change for Internal RC
#endif

// <o> NRF_SDH_CLOCK_LF_ACCURACY  - External clock accuracy used in the LL to compute timing.
 
// <0=> NRF_CLOCK_LF_ACCURACY_250_PPM 
// <1=> NRF_CLOCK_LF_ACCURACY_500_PPM 
// <2=> NRF_CLOCK_LF_ACCURACY_150_PPM 
// <3=> NRF_CLOCK_LF_ACCURACY_100_PPM 
// <4=> NRF_CLOCK_LF_ACCURACY_75_PPM 
// <5=> NRF_CLOCK_LF_ACCURACY_50_PPM 
// <6=> NRF_CLOCK_LF_ACCURACY_30_PPM 
// <7=> NRF_CLOCK_LF_ACCURACY_20_PPM 
// <8=> NRF_CLOCK_LF_ACCURACY_10_PPM 
// <9=> NRF_CLOCK_LF_ACCURACY_5_PPM 
// <10=> NRF_CLOCK_LF_ACCURACY_2_PPM 
// <11=> NRF_CLOCK_LF_ACCURACY_1_PPM 

#ifndef NRF_SDH_CLOCK_LF_ACCURACY
#define NRF_SDH_CLOCK_LF_ACCURACY 1//7 //tai change for Internal RC
#endif

// <e> BLE_EEG_ENABLED - ble_eeg - EEG Service
//==========================================================
#ifndef BLE_EEG_ENABLED
#define BLE_EEG_ENABLED 1
#endif
// <o> BLE_NUS_BLE_OBSERVER_PRIO  
// <i> Priority with which BLE events are dispatched to the UART Service.

#ifndef BLE_EEG_BLE_OBSERVER_PRIO
#define BLE_EEG_BLE_OBSERVER_PRIO 2
#endif
#endif // APP_CONFIG_H

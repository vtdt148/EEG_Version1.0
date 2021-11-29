
#ifndef ADS1296_H__
#define ADS1296_H__
 
#include <stdint.h>
#include "nrf_drv_spi.h"

#define ADS1296_RESET_PIN	NRF_GPIO_PIN_MAP(0,26)
#define ADS1296_PWDN_PIN	NRF_GPIO_PIN_MAP(1,8)
#define ADS1296_DRDY_PIN	NRF_GPIO_PIN_MAP(0,27) //EEG


#define ADS1296_SPI_MISO_PIN	NRF_GPIO_PIN_MAP(1,13) 
#define ADS1296_SPI_SCLK_PIN	NRF_GPIO_PIN_MAP(1,15)
#define ADS1296_SPI_CS_PIN	NRF_GPIO_PIN_MAP(1,12)
#define ADS1296_SPI_MOSI_PIN	NRF_GPIO_PIN_MAP(1,14) 

#define ADS1296_NUM_REGS	26

/**
 *	\brief ADS1296 register addresses.
 */
#define	ADS1296_REGADDR_ID		0x00
#define	ADS1296_REGADDR_CONFIG1		0x01
#define	ADS1296_REGADDR_CONFIG2		0x02
#define ADS1296_REGADDR_CONFIG3		0x03
#define	ADS1296_REGADDR_LOFF		0x04
#define	ADS1296_REGADDR_CH1SET		0x05
#define	ADS1296_REGADDR_CH2SET		0x06
#define	ADS1296_REGADDR_CH3SET		0x07
#define	ADS1296_REGADDR_CH4SET		0x08
#define	ADS1296_REGADDR_CH5SET          0x09
#define	ADS1296_REGADDR_CH6SET          0x0A
#define	ADS1296_REGADDR_CH7SET          0x0B
#define	ADS1296_REGADDR_CH8SET          0x0C

#define	ADS1296_REGADDR_RLD_SENSP	0x0D
#define	ADS1296_REGADDR_RLD_SENSN	0x0E
#define	ADS1296_REGADDR_LOFF_SENSP	0x0F
#define	ADS1296_REGADDR_LOFF_SENSN	0x10
#define	ADS1296_REGADDR_LOFF_FLIP	0x11
#define	ADS1296_REGADDR_LOFF_STATP 	0x12
#define	ADS1296_REGADDR_LOFF_STATN	0x13
#define	ADS1296_REGADDR_GPIO		0x14
#define	ADS1296_REGADDR_PACE		0x15
#define	ADS1296_REGADDR_RESP		0x16
#define ADS1296_REGADDR_CONFIG4		0x17
#define ADS1296_REGADDR_WCT1		0x18
#define ADS1296_REGADDR_WCT2		0x19

/**
 *	\brief ADS1296 SPI communication opcodes.
 *	
 */
/*SYSTEM COMMANDS*/
#define	ADS1296_OPC_WAKEUP		0x02			///< Wake up from standby.
#define	ADS1296_OPC_STANDBY		0x04			///< Enter standby.
#define	ADS1296_OPC_RESET		0x06			///< Reset all registers.	
#define	ADS1296_OPC_START		0x08			///< Start data conversions.
#define	ADS1296_OPC_STOP		0x0A			///< Stop data conversions.
/*DATA READ COMMANDS*/
#define	ADS1296_OPC_RDATAC		0x10			///< Read data continuously (registers cannot be read or written in this mode).
#define	ADS1296_OPC_SDATAC		0x11			///< Stop continuous data read.
#define	ADS1296_OPC_RDATA		0x12			///< Read single data value.
/*REGISTER READ COMMANDS*/
#define	ADS1296_OPC_RREG		0x20			///< Read register value. System must not be in RDATAC mode.
#define	ADS1296_OPC_WREG		0x40			///< Write register value.




#define ADS1296R_DEVICE_ID		0xD1	
/* DEFAULT REGISTER VALUES ********************************************************/
#define	ADS1296_REGDEFAULT_CONFIG1	0x84//0x36 // LP mode 250 SPS,  Daisy-chain mode, 
#define	ADS1296_REGDEFAULT_CONFIG2	0x10//0x12 // 
#define ADS1296_REGDEFAULT_CONFIG3	0xCC // 
#define	ADS1296_REGDEFAULT_LOFF		0x01 // 
#define	ADS1296_REGDEFAULT_CH1SET	0x60 // PGA gain 12
#define	ADS1296_REGDEFAULT_CH2SET	0x60 
#define	ADS1296_REGDEFAULT_CH3SET	0x60
#define	ADS1296_REGDEFAULT_CH4SET	0x60
#define	ADS1296_REGDEFAULT_CH5SET	0x60
#define	ADS1296_REGDEFAULT_CH6SET	0x60
#define	ADS1296_REGDEFAULT_CH7SET	0x00
#define	ADS1296_REGDEFAULT_CH8SET	0x00
#define	ADS1296_REGDEFAULT_RLD_SENSP	0x3F
#define	ADS1296_REGDEFAULT_RLD_SENSN	0x3F
#define	ADS1296_REGDEFAULT_LOFF_SENSP	0x00
#define	ADS1296_REGDEFAULT_LOFF_SENSN	0x00
#define	ADS1296_REGDEFAULT_LOFF_FLIP	0x00
#define	ADS1296_REGDEFAULT_LOFF_STATP 	0x00
#define	ADS1296_REGDEFAULT_LOFF_STATN	0x00
#define	ADS1296_REGDEFAULT_GPIO		0x0F
#define	ADS1296_REGDEFAULT_PACE         0x00
#define	ADS1296_REGDEFAULT_RESP         0x00
#define ADS1296_REGDEFAULT_CONFIG4	0x00
#define ADS1296_REGDEFAULT_WCT1         0x00
#define ADS1296_REGDEFAULT_WCT2         0x00

/**@TYPEDEFS: */
typedef int16_t body_voltage_t;
typedef struct
{
	uint8_t eeg1[3]; 
	uint8_t eeg2[3];
	uint8_t eeg3[3];
	uint8_t eeg4[3];
	uint8_t eeg5[3];
	uint8_t eeg6[3];
        uint8_t eeg_raw[18];
	uint8_t status[3];  
} data_eeg;
/**************************************************************************************************************************************************
*              Function Prototypes ADS1296																																																		*
**************************************************************************************************************************************************/
void ads_spi_init(void);


void init_buf(uint8_t * const p_tx_buffer,
                     uint8_t * const p_rx_buffer,
                     const uint16_t  len);

/**
 *	\brief Initialize the ADS1296
 *
 */
void ads1296_powerup_reset(void);

void ads1296_init_regs(void);

void ads1296_powerdn(void);

void ads1296_powerup(void);

void ads1296_standby(void);

void ads1296_wake(void);

void ads1296_soft_start_conversion(void);

void ads1296_stop_rdatac(void);

void ads1296_start_rdatac(void);

void ads1296_check_id(void);


void get_eeg_adc_samples (data_eeg *eeg_data );

#endif // ADS1296_H__

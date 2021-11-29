#include "ads1296.h"
#include "app_error.h"
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "nrf_log.h"
#include "nrf_delay.h"

#include <stdio.h> 
#include "compiler_abstraction.h"
#include "nrf.h"       			

uint8_t ADS1296_default_registers[] = {
	ADS1296_REGDEFAULT_CONFIG1, 
	ADS1296_REGDEFAULT_CONFIG2,
	ADS1296_REGDEFAULT_CONFIG3, 
	ADS1296_REGDEFAULT_LOFF,
	ADS1296_REGDEFAULT_CH1SET,
	ADS1296_REGDEFAULT_CH2SET,
	ADS1296_REGDEFAULT_CH3SET,
	ADS1296_REGDEFAULT_CH4SET,
	ADS1296_REGDEFAULT_CH5SET,
	ADS1296_REGDEFAULT_CH6SET,
	ADS1296_REGDEFAULT_CH7SET,
	ADS1296_REGDEFAULT_CH8SET,
	ADS1296_REGDEFAULT_RLD_SENSP,
	ADS1296_REGDEFAULT_RLD_SENSN,
	ADS1296_REGDEFAULT_LOFF_SENSP,
	ADS1296_REGDEFAULT_LOFF_SENSN,
	ADS1296_REGDEFAULT_LOFF_FLIP,
	ADS1296_REGDEFAULT_LOFF_STATP,
	ADS1296_REGDEFAULT_LOFF_STATN,
	ADS1296_REGDEFAULT_GPIO,
	ADS1296_REGDEFAULT_PACE,
	ADS1296_REGDEFAULT_RESP,
	ADS1296_REGDEFAULT_CONFIG4,
	ADS1296_REGDEFAULT_WCT1,
	ADS1296_REGDEFAULT_WCT2
};



/**@SPI HANDLERS:
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{

}


#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE); 

void ads_spi_init(void) {
  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
  spi_config.bit_order            = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
  //SCLK = 1MHz is right speed because fCLK = (1/2)*SCLK, and fMOD = fCLK/4, and fMOD MUST BE 128kHz. Do the math.
  spi_config.frequency		= NRF_DRV_SPI_FREQ_1M;
  spi_config.irq_priority	= APP_IRQ_PRIORITY_LOW;
  spi_config.mode		= NRF_DRV_SPI_MODE_1; //CPOL = 0 (Active High); CPHA = TRAILING (1)
  spi_config.miso_pin 		= ADS1296_SPI_MISO_PIN;
  spi_config.sck_pin 		= ADS1296_SPI_SCLK_PIN;
  spi_config.mosi_pin 		= ADS1296_SPI_MOSI_PIN;
  spi_config.ss_pin		= ADS1296_SPI_CS_PIN;
  spi_config.orc			= 0x55;
  APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
  printf("SPI Initialized..\r\n");
}


/**************************************************************************************************************************************************
 *               Function Definitions                                                                                                              *
 **************************************************************************************************************************************************/

/*
 * ADS1296 CONTROL FUNCTIONS:
 */
void ads1296_init_regs(void) {
	uint8_t err_code;
	uint8_t num_registers = 25;
	uint8_t txrx_size = num_registers+2;
	uint8_t tx_data_spi[txrx_size];
	uint8_t rx_data_spi[txrx_size]; 
	ads1296_check_id();
	for (int i = 0; i < txrx_size; ++i) {
		tx_data_spi[i] = 0;
		rx_data_spi[i] = 0;
	}
	tx_data_spi[0] = 0x41;// WREG
	tx_data_spi[1] = num_registers - 1;
	for (int j = 0; j < num_registers; ++j) {
		tx_data_spi[j+2] = ADS1296_default_registers[j];
	}
	err_code = nrf_drv_spi_transfer(&spi, tx_data_spi, num_registers+2, rx_data_spi, num_registers+2);
	nrf_delay_ms(1500);
}

void ads1296_powerup_reset(void)
{
	nrf_gpio_pin_clear(ADS1296_RESET_PIN);
	nrf_gpio_pin_set(ADS1296_PWDN_PIN);
	nrf_delay_ms(100);
        nrf_gpio_pin_set(ADS1296_RESET_PIN);
        nrf_delay_ms(100);
	printf("ADS1296 powered up and reset..\r\n");
}

void ads1296_powerdn(void)
{
	nrf_delay_us(20);
	printf("ADS1296 POWERED DOWN..\r\n");
}

void ads1296_powerup(void)
{

	nrf_delay_ms(1000);		// Allow time for power-on reset
	printf("ADS1296 POWERED UP...\r\n");
}

void ads1296_standby(void) {
	uint8_t tx_data_spi;
	uint8_t rx_data_spi;

	tx_data_spi = ADS1296_OPC_STANDBY;

	nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1);
        nrf_delay_ms(20); //Wait for response
	printf("ADS1296 placed in standby mode...\r\n");
}

void ads1296_wake(void) {
	uint8_t tx_data_spi;
	uint8_t rx_data_spi;

	tx_data_spi = ADS1296_OPC_WAKEUP;

	nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1);
	nrf_delay_ms(10);	// Allow time to wake up - 10ms
	printf("ADS1296 Wakeup..\r\n");
}

void ads1296_soft_start_conversion(void) {
	uint8_t tx_data_spi;
	uint8_t rx_data_spi;

	tx_data_spi = ADS1296_OPC_START;

	nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1);
        nrf_delay_ms(20); //Wait for response
	printf("Start ADC conversion..\r\n");
}

void ads1296_stop_rdatac(void) {
	uint8_t tx_data_spi;
	uint8_t rx_data_spi;

	tx_data_spi = ADS1296_OPC_SDATAC;

	nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1);
        nrf_delay_ms(20); //Wait for response
        printf("Stop ADC conversion..\r\n");
}

void ads1296_start_rdatac(void) {
	uint8_t tx_data_spi;
	uint8_t rx_data_spi;

	tx_data_spi = ADS1296_OPC_RDATAC;

	nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1);
        nrf_delay_ms(20); //Wait for response
}

void ads1296_check_id(void) {
	uint8_t device_id_reg_value;
	uint8_t tx_data_spi[3];
	uint8_t rx_data_spi[3];

	tx_data_spi[0] = 0x20;	//Request Device ID
	tx_data_spi[1] = 0x01;	//Intend to read 1 byte
	tx_data_spi[2] = 0x00;	//This will be replaced by Reg Data
	nrf_drv_spi_transfer(&spi, tx_data_spi, 2+tx_data_spi[1], rx_data_spi, 2+tx_data_spi[1]);
	nrf_delay_ms(20); //Wait for response
       
	device_id_reg_value = rx_data_spi[2];

	if (device_id_reg_value==ADS1296R_DEVICE_ID) {
		printf("ADS1296 Device found! CHIP ID = 0X%X\r\n", device_id_reg_value);
                
	} else {
		printf("ADS1296 Device NOT found! Return value = 0X%X\r\n", device_id_reg_value);
        }
}

/**@brief Function for acquiring a EEG samples.
 *
 * @details Uses SPI
 *          
 */
void get_eeg_adc_samples (data_eeg *eeg_data ) {
		uint8_t tx_rx_data[24] = {
		0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00,
                0x00, 0x00, 0x00};
		uint8_t cnt = 0;
		do {    nrf_drv_spi_transfer(&spi, tx_rx_data, 22, tx_rx_data, 22);
			if(tx_rx_data[0]==0xC0){
				break;
			}
			cnt++;
			nrf_delay_us(10);
		} while(cnt<255);
                nrf_delay_us(1000);
/*printf("Status: [0x%x 0x%x 0x%x 0x%x | cnt=%d]\r\n",tx_rx_data[0],tx_rx_data[1],tx_rx_data[2],tx_rx_data[3],cnt);
printf("DATA1:[0x%.2X%.2X%.2X]\r\n",tx_rx_data[4],tx_rx_data[5],tx_rx_data[6]);
printf("DATA2:[0x%.2X%.2X%.2X]\r\n",tx_rx_data[7],tx_rx_data[8],tx_rx_data[9]);
printf("DATA3:[0x%.2X%.2X%.2X]\r\n",tx_rx_data[10],tx_rx_data[11],tx_rx_data[12]);
printf("DATA4:[0x%.2X%.2X%.2X]\r\n",tx_rx_data[13],tx_rx_data[14],tx_rx_data[15]);
printf("DATA5:[0x%.2X%.2X%.2X]\r\n",tx_rx_data[16],tx_rx_data[17],tx_rx_data[18]);
printf("DATA6:[0x%.2X%.2X%.2X]\r\n\r\n",tx_rx_data[19],tx_rx_data[20],tx_rx_data[21]);*/
 for (uint32_t i = 0; i < 18; i++)
	{
	  eeg_data->eeg_raw[i] = tx_rx_data[i+4];
	}
}

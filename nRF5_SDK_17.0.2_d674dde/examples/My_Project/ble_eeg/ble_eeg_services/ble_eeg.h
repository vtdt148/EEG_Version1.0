/*      
 */
#ifndef BLE_EEG_H__
#define BLE_EEG_H__

#include <stdint.h>
#include <stdbool.h>
#include "sdk_config.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "ble_link_ctx_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_EEG_DEF(_name, _eeg_max_clients)                      \
    BLE_LINK_CTX_MANAGER_DEF(CONCAT_2(_name, _link_ctx_storage),  \
                             (_eeg_max_clients),                  \
                             sizeof(ble_eeg_client_context_t));   \
    static ble_eeg_t _name =                                      \
    {                                                             \
        .p_link_ctx_storage = &CONCAT_2(_name, _link_ctx_storage) \
    };                                                            \
    NRF_SDH_BLE_OBSERVER(_name ## _obs,                           \
                         BLE_EEG_BLE_OBSERVER_PRIO,               \
                         ble_eeg_on_ble_evt,                      \
                         &_name)

#define BLE_UUID_EEG_SERVICE 0x0001 /**< The UUID of the Custom EEG Service. */

#define OPCODE_LENGTH        1
#define HANDLE_LENGTH        2

/**@brief   Maximum length of data (in bytes) that can be transmitted to the peer by the Custom EEG service module. */
#if defined(NRF_SDH_BLE_GATT_MAX_MTU_SIZE) && (NRF_SDH_BLE_GATT_MAX_MTU_SIZE != 0)
    #define BLE_EEG_MAX_DATA_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH)
#else
    #define BLE_EEG_MAX_DATA_LEN (BLE_GATT_MTU_SIZE_DEFAULT - OPCODE_LENGTH - HANDLE_LENGTH)
    #warning NRF_SDH_BLE_GATT_MAX_MTU_SIZE is not defined.
#endif


/**@brief   Custom EEG Service event types. */
typedef enum
{
    BLE_EEG_EVT_RX_DATA,      /**< Data received. */
    BLE_EEG_EVT_TX_RDY,       /**< Service is ready to accept new data to be transmitted. */
    BLE_EEG_EVT_COMM_STARTED, /**< Notification has been enabled. */
    BLE_EEG_EVT_COMM_STOPPED, /**< Notification has been disabled. */
} ble_eeg_evt_type_t;


/* Forward declaration of the ble_eeg_t type. */
typedef struct ble_eeg_s ble_eeg_t;


/**@brief   Custom EEG Service @ref BLE_EEG_EVT_RX_DATA event data.
 *
 * @details This structure is passed to an event when @ref BLE_EEG_EVT_RX_DATA occurs.
 */
typedef struct
{
    uint8_t const * p_data; /**< A pointer to the buffer with received data. */
    uint16_t        length; /**< Length of received data. */
} ble_eeg_evt_rx_data_t;


/**@brief Custom EEG Service client context structure.
 *
 * @details This structure contains state context related to hosts.
 */
typedef struct
{
    bool is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
} ble_eeg_client_context_t;


/**@brief   Custom EEG Service event structure.
 *
 * @details This structure is passed to an event coming from service.
 */
typedef struct
{
    ble_eeg_evt_type_t         type;        /**< Event type. */
    ble_eeg_t                * p_eeg;       /**< A pointer to the instance. */
    uint16_t                   conn_handle; /**< Connection handle. */
    ble_eeg_client_context_t * p_link_ctx;  /**< A pointer to the link context. */
    union
    {
        ble_eeg_evt_rx_data_t rx_data; /**< @ref BLE_EEG_EVT_RX_DATA event data. */
    } params;
} ble_eeg_evt_t;


/**@brief Custom EEG Service event handler type. */
typedef void (* ble_eeg_data_handler_t) (ble_eeg_evt_t * p_evt);


/**@brief   Custom EEG Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_eeg_init
 *          function.
 */
typedef struct
{
    ble_eeg_data_handler_t data_handler; /**< Event handler to be called for handling received data. */
} ble_eeg_init_t;


/**@brief   Custom EEG Service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_eeg_s
{
    uint8_t                         uuid_type;          /**< UUID type for Custom EEG Service Base UUID. */
    uint16_t                        service_handle;     /**< Handle of Custom EEG Service (as provided by the SoftDevice). */
    ble_gatts_char_handles_t        tx_handles;         /**< Handles related to the TX characteristic (as provided by the SoftDevice). */
    ble_gatts_char_handles_t        rx_handles;         /**< Handles related to the RX characteristic (as provided by the SoftDevice). */
    blcm_link_ctx_storage_t * const p_link_ctx_storage; /**< Pointer to link context storage with handles of all current connections and its context. */
    ble_eeg_data_handler_t          data_handler;       /**< Event handler to be called for handling received data. */
};


/**@brief   Function for initializing the Custom EEG Service.
 *
 * @param[out] p_eeg      Custom EEG Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_eeg_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers p_eeg or p_eeg_init is NULL.
 */
uint32_t ble_eeg_init(ble_eeg_t * p_eeg, ble_eeg_init_t const * p_eeg_init);


/**@brief   Function for handling the Custom EEG Service's BLE events.
 *
 * @details The Custom EEG Service expects the application to call this function each time an
 * event is received from the SoftDevice. This function processes the event if it
 * is relevant and calls the Custom EEG Service event handler of the
 * application if necessary.
 *
 * @param[in] p_ble_evt     Event received from the SoftDevice.
 * @param[in] p_context     Custom EEG Service structure.
 */
void ble_eeg_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**@brief   Function for sending a data to the peer.
 *
 * @details This function sends the input string as an RX characteristic notification to the
 *          peer.
 *
 * @param[in]     p_eeg       Pointer to the Custom EEG Service structure.
 * @param[in]     p_data      String to be sent.
 * @param[in,out] p_length    Pointer Length of the string. Amount of sent bytes.
 * @param[in]     conn_handle Connection Handle of the destination client.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_eeg_data_send(ble_eeg_t * p_eeg,
                           uint8_t   * p_data,
                           uint16_t  * p_length,
                           uint16_t    conn_handle);


#ifdef __cplusplus
}
#endif

#endif // BLE_EEG_H__

/** @} */

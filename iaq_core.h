#ifndef _IAQCORE_H_
#define _IAQCORE_H_

#include "FreeRTOS.h"
#include "task.h"

#include "espressif/esp_common.h"
#include "espressif/sdk_private.h"

#include "esp/uart.h"
#include "i2c/i2c.h"

#include "stdint.h"
#include "stdbool.h"

// The status (`stat` output parameter of `read`) uses the following flags0x00: OK (data valid)
#define IAQ_CORE_OK     			0x00 // Data ok
#define IAQ_CORE_BUSY   			0x01 // Data might be inconsistent; re-read()
#define IAQ_CORE_RUNIN  			0x10 // iAQcore chip in warm up phase
#define IAQ_CORE_ERROR  			0x80 // Sensor might be broken
#define IAQ_CORE_I2C_ERR 			0x10 // Software added error: I2C transaction error
#define IAQ_CORE_STATUS_UNDEFINED   0xFF
#define IAQ_CORE_NOT_PRESENT        0xA8    // arbitrary library status code

// Chip constants
#define IAQCORE_I2C_ADDR          	0x5A  // 7-bit I2C slave address of the iAQcore
#define IAQCORE_I2C_CLOCK_STRETCH  	3000
#define IAQCORE_I2C_FREQ      		I2C_FREQ_100K

#define IAQCORE_DATA_SIZE           9     // Size (in bytes) of the measurement data block 
#define IAQCORE_REG                 0


// Definition of error codes
#define IAQ_CORE_I2C_OK             0
#define IAQ_CORE_I2C_NOK           -1

// Error codes for the I2C interface
#define IAQ_CORE_I2C_READ_FAILED    1
#define IAQ_CORE_SENSOR_ERROR    	2
#define IAQ_CORE_I2C_BUSY           3
#define IAQ_CORE_HEATING_UP	    	4

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct {
    int           error_code;  // contains the error code of last operation
    uint8_t       bus;         // I2C bus
    uint8_t       addr;        // I2C slave address
} iaq_core_sensor_t;

/**
 * @brief	Initialize a iAQ Core sensor
 *
 * The function initializes the iAQ Core sensor and checks its availability.
 *
 * @param  bus       I2C bus at which iAQ Core sensor is connected
 * @param  addr      I2C slave address of the iAQ Core sensor
 *
 * @return           pointer to sensor data structure, or NULL on error
 */
iaq_core_sensor_t* iaq_core_init_sensor (uint8_t bus, uint8_t addr);

/**
 * Check if iAQ Core is busy while measuring.
 * Return true if iAQ Core is busy.
 */
bool iaq_core_is_measuring(iaq_core_sensor_t *dev);


/**
 * Check if iAQ Core is busy while measuring.
 * Return true if iAQ Core is busy.
 */
bool iaq_core_is_heating(iaq_core_sensor_t *dev);


/**
 * @brief	Get latest IAQ sensor values and/or RAW sensor data
 *
 * The function reads the IAQ sensor values (TVOC and eCO2) and/or the raw
 * sensor data. If some of the results are not needed, the corresponding
 * pointer parameters can be set to NULL.
 *
 *
 * @param  sensor    pointer to the sensor device data structure
 * @param  iaq_tvoc  TVOC total volatile organic compound (0 - 1187 ppb)
 * @param  iaq_eco2  eCO2 equivalent CO2 (400 - 8192 ppm)
 * @param  stat      current status of the sensor
 * @param  resist    sensor resistance (raw data)
 *
 * @return           true on success, false on error
 */
bool iaq_core_get_results (iaq_core_sensor_t* sensor,
                         uint16_t* etvoc,
                         uint16_t* eco2); 
                         //uint16_t* stat,
                         //uint32_t* resist);                       


#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* _IAQCORE_H_ */


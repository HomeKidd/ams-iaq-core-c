#include <stdio.h>

#include "espressif/esp_common.h"
#include "esp/uart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "i2c/i2c.h"
#include "iaq_core.h"

#define SENSOR_POLL_PERIOD 10000  // reading time

#define SCL_PIN 5
#define SDA_PIN 4
#define I2C_BUS 0

static iaq_core_sensor_t* iaq_core; // iAQ Core device data structure

void iaq_core_task(void *pvParameters){

uint16_t tvoc_value;
uint16_t co2_value;

while(1){


        while (iaq_core_is_heating(iaq_core)){  // busy waiting until sensor is heating up
                printf("Sensor is heating up...\n It takes about 5 minutes.\n");
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                }

        if (!iaq_core_get_results(iaq_core, &tvoc_value, &co2_value)){
                printf("iAQ Core C reading failed\n");
                vTaskDelay(5000 / portTICK_PERIOD_MS);
            }
        while (iaq_core_is_measuring(iaq_core)){}; // busy waiting until measurement results are available

        if (iaq_core_get_results(iaq_core, &tvoc_value, &co2_value)){
            printf("iAQ Core C Sensor: %d TVOC, %d CO2\n", tvoc_value, co2_value);
            }

        else {

                // error happened
            switch (iaq_core->error_code)
                {
                    case IAQ_CORE_I2C_BUSY:
                                            printf("Sensor is BUSY...\n");
                                            break;
                    case IAQ_CORE_I2C_READ_FAILED:
                                            printf("I2C BUS read failed...\n");
                                            break;                                            
                    case IAQ_CORE_SENSOR_ERROR:
                                            printf("Sensor error, its might be broken...\n");
                                            break;                    
                    default:
                            printf(" ");
                }
            }

        vTaskDelay(SENSOR_POLL_PERIOD / portTICK_PERIOD_MS);
   }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version : %s\n", sdk_system_get_sdk_version());

    i2c_init (I2C_BUS, I2C_SCL_PIN, I2C_SDA_PIN, IAQCORE_I2C_FREQ);
    i2c_set_clock_stretch (I2C_BUS, IAQCORE_I2C_CLOCK_STRETCH);
    iaq_core = iaq_core_init_sensor (I2C_BUS, IAQCORE_I2C_ADDR);

    if (iaq_core) xTaskCreate(iaq_core_task, "iaq_core_task", 256, NULL, 2, NULL);
}

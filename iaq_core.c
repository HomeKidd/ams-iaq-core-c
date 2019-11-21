#include <string.h>
#include <stdlib.h>

#include "iaq_core.h"


#define IAQ_CORE_CO2_MSB_OFFSET             0
#define IAQ_CORE_CO2_LSB_OFFSET             1
#define IAQ_CORE_STATUS_OFFSET              2
#define IAQ_CORE_RESISTANCE_MSB_OFFSET      4
#define IAQ_CORE_RESISTANCE_MID_OFFSET      5
#define IAQ_CORE_RESISTANCE_LSB_OFFSET      6
#define IAQ_CORE_TVOC_MSB_OFFSET            7
#define IAQ_CORE_TVOC_LSB_OFFSET            8


iaq_core_sensor_t* iaq_core_init_sensor (uint8_t bus, uint8_t addr)
{
    iaq_core_sensor_t* dev;
    
    if ((dev = malloc (sizeof(iaq_core_sensor_t))) == NULL)
        return NULL;

    // init sensor data structure
    dev->bus  = bus;
    dev->addr = addr; //IAQCORE_I2CADDR
    dev->error_code = IAQ_CORE_I2C_OK;

    return dev;
}

bool iaq_core_get_results (iaq_core_sensor_t* dev,
                         uint16_t* etvoc,
                         uint16_t* eco2) 
                         //uint16_t* stat,
                         //uint32_t* resist);
{
    if (!dev) return false;
  
dev->error_code = IAQ_CORE_I2C_OK;

  uint8_t data[10];

  for(int i=0; i<10; i++ ) data[i]=i2c_slave_read(dev->bus, dev->addr, NULL, data, IAQCORE_DATA_SIZE);

  if (data[2] == IAQ_CORE_I2C_ERR){
      dev->error_code = IAQ_CORE_I2C_READ_FAILED;
      return false;} 
  
  if (data[2] == IAQ_CORE_ERROR){
      dev->error_code = IAQ_CORE_SENSOR_ERROR;
      return false;} 



  //if (buffer[2] == IAQ_CORE_OK ) {

    // if *iaq* is not NULL return IAQ sensor values
    if (etvoc != 0) *etvoc =  (data[IAQ_CORE_TVOC_MSB_OFFSET]<<8) + (data[IAQ_CORE_TVOC_LSB_OFFSET]<<0);
    if (eco2  != 0) *eco2  =  (data[IAQ_CORE_CO2_MSB_OFFSET]<<8)  + (data[IAQ_CORE_CO2_LSB_OFFSET]<<0);
    //if (stat != 0) *stat  =  (data[IAQ_CORE_STATUS_OFFSET]<<0); 

    //} 
 
    return true;

}

bool iaq_core_is_measuring(iaq_core_sensor_t *dev)
{
    uint8_t data[10];
 
    for(int i=0; i<10; i++ ) data[i]=i2c_slave_read(dev->bus, dev->addr, NULL, data, IAQCORE_DATA_SIZE);
  
      if (data[2] == IAQ_CORE_BUSY){
          //printf("Sensor error...\n");
          dev->error_code = IAQ_CORE_I2C_BUSY;
          return true;
        }

    dev->error_code = IAQ_CORE_I2C_OK;

    return false;
}

bool iaq_core_is_heating(iaq_core_sensor_t *dev)
{
    uint8_t data[10];
 
    for(int i=0; i<10; i++ ) data[i]=i2c_slave_read(dev->bus, dev->addr, NULL, data, IAQCORE_DATA_SIZE);
  
      if (data[2] == IAQ_CORE_RUNIN){
          //dev->error_code = IAQ_CORE_HEATING_UP;
          return true;
        }
    dev->error_code = IAQ_CORE_I2C_OK;

    return false;
}



# AMS iAQ Core C library
Library for AMS iAQ Core C Air quality sensor using [esp-open-rtos](https://github.com/SuperHouse)

### Hardware configurations

Following figure shows the hardware configuration for ESP8266.

```
  +------------------+       +--------+
  | ESP8266          |       |iAQ Core|
  |                  |       |        |
  |    GPIO 5  (SCL) >-------> SCL    |
  |    GPIO 4  (SDA) <-------> SDA    |
  |                  |       |        |
  +------------------+       +--------+
```

 **Please note both SCL and SDA pins needs an external 10k pullup according to [AMS iAQ Core datasheet](https://ams.com/documents/20143/36005/iAQ-core_DS000334_1-00.pdf/123c67cc-92d4-9a5c-d7ca-24f1d439c6a4)!**
 
 
 ### Main program

Before using the iAQ Core driver, function ```i2c_init``` needs to be called for each I2C interface to setup them.

**Please note:** iAQ Core C uses clock streching that can be longer than the default I2C clock stretching. Therefore the clock stretching parameter of I2C has to be set to at least ```IAQCORE_I2C_CLOCK_STRETCH```.

```
...
i2c_init (I2C_BUS, I2C_SCL_PIN, I2C_SDA_PIN, IAQCORE_I2C_FREQ);
i2c_set_clock_stretch (I2C_BUS, IAQCORE_I2C_CLOCK_STRETCH);
...
```
Once I2C interfaces to be used are initialized, function ```iaq_core_init_sensor``` has to be called for iAQ Core sensor to initialize the sensor and to check its availability as well as its error state. The parameters specify the I2C bus to which it is connected and its I2C slave address.

```
...
iaq_core_init_sensor (I2C_BUS, IAQCORE_I2C_ADDR);
...
```

Function ```iaq_core_init_sensor``` returns a pointer to the sensor device data structure or NULL in case of error.

If initialization of the sensor was successful, the sensor **starts Heating Up phase which takes about 5 minutes**.
Two functions are used for busy waiting until Heating up and/or sensor reading is finishing. When heating up and/or sensor reading is done, return false and exits the loop.

>     while (iaq_core_is_heating(iaq_core)){
>             ...
>             }
> 
>     while (iaq_core_is_measuring(iaq_core)){
>             ...
>             };





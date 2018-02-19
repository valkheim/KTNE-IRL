# KTNE module core library

This library implements all the mandatory features for a module :
* pin initialization (leds, sense)
* communication (I2C and serial)
* globals such as difficulty, penality, ...

## Core functions
* `void setupCore();` : setup the core (init pins, init I2C and serial
* `uint8_t getI2CAddr();` : read module I2C address from `as` pins
* `uint16_t readUnit16();` : read 2-byte data from I2C bus
* `void i2c_receive_data(int count);` : read command and its parameter from I2C bus
* `void writeUint16(uint16_t i);` : write 2-byte data on I2C bus
* `void answer(uint16_t command, uint16_t parameter);` : answer `parameter` to master given a command
* `bool needToSpeak();` : check wether the module need to contact the master or not
* `void i2c_receive_request();` : I2C callback to parse command and respond
* `void defuseModule();` : function to defuse the module. This is the only one to be used 'publically'
* `void applyPenality(uint16_t penality);` : funtion to notify that user made a mistake

## Installation

Create a zip file of the library and [rtfm](https://www.arduino.cc/en/Guide/Libraries).

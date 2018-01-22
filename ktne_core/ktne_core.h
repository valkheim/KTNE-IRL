#ifndef CORE_H_
# define CORE_H_

# if ARDUINO >= 100
#  include "Arduino.h"
# else
#  include "WProgram.h"
# endif

// Output
# define RED_LED (2)
# define GREEN_LED (3)
# define SENSE_PIN (7)

// Command
# define CMD_TIME (0)
# define CMD_DIFFICULTY (1)
# define CMD_NEED_TO_SPEAK (2)
# define CMD_INFO (3)

# define CMD_DEFUSED (4)
# define CMD_PENALITY (5)
# define CMD_END (6)

// Answer
# define ANSWER_YES (1)
# define ANSWER_NO (0)
# define ANSWER_OK (1)

// Difficulty
# define MAX_DIFFICULTY (3)
# define HARD (3)
# define MEDIUM (2)
# define EASY (1)

extern int as[]; // Pins used to read the module's address

extern const size_t sizeof_as;
extern uint16_t timeleft;
extern uint16_t difficulty;
extern bool masterNeedsDefusingInformation;
extern uint16_t defused;
extern uint16_t penality;
extern uint16_t command;   // Last command sent by master
extern uint16_t parameter; // Last command's parameter sent by master

extern void setupCore();
extern uint8_t getI2CAddr();
extern uint16_t readUnit16();
extern void i2c_receive_data(int count);
extern void writeUint16(uint16_t i);
extern void answer(uint16_t command, uint16_t parameter);
extern bool needToSpeak();
extern void i2c_receive_request();
extern void updateModuleStatus();
extern void defuseModule();

#endif

#ifndef KTNE_CORE_H_
# define KTNE_CORE_H_

# if ARDUINO >= 100
#  include "Arduino.h"
# else
#  include "WProgram.h"
# endif

/* Output */
# define LED_RED (4)
# define LED_GREEN (5)
# define SENSE_PIN (7)

/* Command */
# define CMD_TIME (0)
# define CMD_DIFFICULTY (1)
# define CMD_NEED_TO_SPEAK (2)
# define CMD_INFO (3)

# define CMD_DEFUSED (4)
# define CMD_PENALITY (5)
# define CMD_END (6)

/* Answer */
# define ANSWER_YES (1)
# define ANSWER_NO (0)
# define ANSWER_OK (1)

/* Difficulty */
# define MAX_DIFFICULTY (3)
# define HARD (3)
# define MEDIUM (2)
# define EASY (1)

extern uint16_t difficulty;
extern uint16_t timeleft;

void setupCore();
void defuseModule();
void applyPenality(uint16_t penality);

#endif

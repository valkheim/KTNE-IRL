/****************************************
 * Simon slave
 *
 * Théophile Champion
 *
 * This is the simon slave.
 * User have to repeat the correct sequence to defuse
 * the module (by calling defuseModule()
 * from ktne_core).
****************************************/

#include <ktne_core.h>

# define PIN_LATCH (6)
# define PIN_CLOCK (12)
# define PIN_DATA  (13)

// Value for needPrintSeq
# define FALSE         (0)
# define TRUE          (1)
# define WAIT_USER_SEQ (2)

int needPrintSeq = FALSE;

// Led index
/*
** Warning : If you change the following define take a look to the loop function.
** ---> for (int color = LED_GREEN; color <= LED_RED; color++)
*/
# define LED_OFF    (0)
# define LED_GREEN  (1)
# define LED_YELLOW (2)
# define LED_BLUE   (3)
# define LED_RED    (4)

// Sequences' size
# define SEQ_EASY_SIZE   (4)
# define SEQ_MEDIUM_SIZE (8)
# define SEQ_HARD_SIZE   (12)
# define SEQ_USER_SIZE   (12)

// Pin use to start printing the leds sequence
# define PIN_BUTTON_SEQ (2)

/*
** Warning : If you change the following define remember that seq_user MUST 
** have at least the same size as the biggest difficulty sequence.
*/
int seq_easy[SEQ_EASY_SIZE]     = {LED_GREEN, LED_GREEN, LED_BLUE, LED_RED};
int seq_medium[SEQ_MEDIUM_SIZE] = {LED_BLUE, LED_GREEN, LED_BLUE, LED_RED, LED_YELLOW, LED_BLUE, LED_GREEN, LED_YELLOW};
int seq_hard[SEQ_HARD_SIZE]     = {LED_BLUE, LED_GREEN, LED_YELLOW, LED_YELLOW, LED_BLUE, LED_BLUE, LED_RED, LED_GREEN, LED_GREEN, LED_YELLOW, LED_BLUE, LED_RED};
      
int answer_seq_easy[SEQ_EASY_SIZE]     = {LED_GREEN, LED_GREEN, LED_YELLOW, LED_GREEN};
int answer_seq_medium[SEQ_MEDIUM_SIZE] = {LED_YELLOW, LED_RED, LED_YELLOW, LED_GREEN, LED_YELLOW, LED_YELLOW, LED_RED, LED_YELLOW};
int answer_seq_hard[SEQ_HARD_SIZE]     = {LED_RED, LED_RED, LED_GREEN, LED_GREEN, LED_RED, LED_RED, LED_BLUE, LED_RED, LED_RED, LED_GREEN, LED_RED, LED_BLUE};

int seq_user[SEQ_USER_SIZE]     = {LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF, LED_OFF};

// Pin of simon buttons
# define PIN_BUTTONS (2)

// Value buttons
# define STATE_LOW        (0)
# define STATE_HIGH       (1)
# define STATE_STILL_HIGH (2)

int buttons[4] = {STATE_LOW, STATE_LOW, STATE_LOW, STATE_LOW};

void loop()
{
  if (needPrintSeq == TRUE)
  {
    printSequence();
    needPrintSeq = WAIT_USER_SEQ;
  }
  else if (needPrintSeq == WAIT_USER_SEQ)
  {
    updateButtons();
    for (int color = LED_GREEN; color <= LED_RED; color++)
    {
      if (buttons[color - 1] == STATE_HIGH)
        pushInUserSequence(color);
    }
    needPrintSeq = chechUserSeq();
    delay(100);
  }
}

void setup()
{
  setupCore();
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_BUTTON_SEQ, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_SEQ), updateNeedPrintSequence, RISING);
}

int *getSequenceRequired()
{
  if (difficulty == EASY)
    return seq_easy;
  else if (difficulty == MEDIUM)
    return seq_medium;
  else
    return seq_hard;
}

int *getAnswerSequenceRequired()
{
  if (difficulty == EASY)
    return answer_seq_easy;
  else if (difficulty == MEDIUM)
    return answer_seq_medium;
  else
    return answer_seq_hard;
}

int getSizeOfSequenceRequired()
{
  if (difficulty == EASY)
    return SEQ_EASY_SIZE;
  else if (difficulty == MEDIUM)
    return SEQ_MEDIUM_SIZE;
  else
    return SEQ_HARD_SIZE;
}

void printSequence()
{
  for (int j = 0; j < getSizeOfSequenceRequired(); j++)
  {
    printColor(PIN_DATA, PIN_CLOCK, getSequenceRequired()[j]);
    delay(1000);
    printColor(PIN_DATA, PIN_CLOCK, LED_OFF);
    delay(1000);
  }
}

void printColor(int myDataPin, int myClockPin, byte color)
{
  /*
  ** This table is used to light on/off the leds using the card 74HC595.
  ** The i-th element of this table corresponds to the i-th led (LED_OFF, LED_GREEN, LED_YELLOW, LED_BLUE, LED_RED)
  ** The elements are encoded in binary format, for example 0x00000010 allows to turn on the second led and turn off the others. In our case it is linked with the green led.
  */
  uint8_t tab[5] = {0, 2, 4, 8, 16};

  digitalWrite(PIN_LATCH, 0);
  shiftOut(myDataPin, myClockPin, MSBFIRST, tab[color]);
  shiftOut(myDataPin, myClockPin, MSBFIRST, tab[color]);
  digitalWrite(PIN_LATCH, 1);
}

int computeUpdateButton(int old_value, int curr_value)
{
  if (old_value == STATE_LOW && curr_value == HIGH)
    return STATE_HIGH;
  if (old_value == STATE_HIGH && curr_value == HIGH)
    return STATE_STILL_HIGH;
  if (old_value == STATE_STILL_HIGH && curr_value == HIGH)
    return STATE_STILL_HIGH;
  return STATE_LOW;
}

void computeButtonsActivation(int analogValue, int readValue[5])
{
  if (analogValue >= 300 && analogValue < 400)
    readValue[0] += 1;
  else if (analogValue >= 400 && analogValue < 500)
    readValue[1] += 1;
  else if (analogValue >= 500 && analogValue < 700)
    readValue[2] += 1;
  else if (analogValue >= 700)
    readValue[3] += 1;
  else
    readValue[4] += 1;
}

void updateButtons()
{
  int readValue[5] = {0};

  for (int i = 0; i < 4; i++)
  {
    computeButtonsActivation(analogRead(PIN_BUTTONS), readValue);
    delay(8);
  }
  int max_index = 0;
  int max_value = readValue[0];
  for (int i = 1; i < 5; i++)
  {
    if (max_value < readValue[i])
    {
      max_index = i;
      max_value = readValue[i];
    }
  }
  for (int i = 0; i < 5; i++)
    readValue[i] = (max_index == i);
  for (int i = 0; i < 4; i++)
    buttons[i] = computeUpdateButton(buttons[i], readValue[i]);
}

void pushInUserSequence(int color)
{
  for (int i = SEQ_USER_SIZE - 1; i > 0; i--)
    seq_user[i] = seq_user[i - 1];
  seq_user[0] = color;
}

int indexSequencesDiff(int *seq_user, int *seq_test, int seq_test_size)
{
  for (int i = 0; i < seq_test_size; i++)
    if (seq_user[getSizeOfSequenceRequired() - 1 - i] != seq_test[i])
      return i;
  return seq_test_size;
}

void clearUserSequence()
{
  for (int i = 0; i < SEQ_USER_SIZE; i++)
    seq_user[i] = LED_OFF;
}

void clearButtons()
{
  for (int i = 0; i < 4; i++)
    buttons[i] = STATE_LOW;
}

int chechUserSeq()
{
  int index;

  index = indexSequencesDiff(seq_user, getAnswerSequenceRequired(), getSizeOfSequenceRequired());
  if (index == getSizeOfSequenceRequired()) // Module defused
  {
    defuseModule();
    return FALSE;
  }
  if (seq_user[getSizeOfSequenceRequired() - 1] == LED_OFF)
    return WAIT_USER_SEQ;
  clearUserSequence();
  clearButtons();
  applyPenality(10);
  return TRUE;
}

void updateNeedPrintSequence()
{
  if (needPrintSeq == FALSE)
    needPrintSeq = TRUE;
}


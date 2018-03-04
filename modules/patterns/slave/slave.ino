/****************************************
 * patterns/slave
 *
 * Charles Paulet
 *
 * Slave part of the module that controls the
 * SD/TFT and implements game mechanics.
 *
 * Require SD and TFT library.
 * Wiring guide:
 * https://www.arduino.cc/en/Guide/TFTtoBoards
 * TFT guide:
 * https://www.arduino.cc/en/Tutorial/TFTBitmapLogo
 *
 * Use windows 3.x bmp formats. Convert with
 * $> convert file.bmp -format bmp3 file.bmp
 *
 * SD card must be FAT16 or FAT32 formatted
 ***************************************/

#include <SPI.h>
#include <SD.h>
#include <TFT.h>
#include <ktne_core.h>

#define PIN_DIFFICULTY_0 (A0) /* sync the module difficulty */
#define PIN_DIFFICULTY_1 (A1) /* sync the module difficulty */
#define PIN_PENALITY   (6)
#define PIN_DEFUSAL    (7)

#define PIN_BTN_LEFT   (2)
#define PIN_BTN_MIDDLE (3)
#define PIN_BTN_RIGHT  (5)

#define IMG_DIMENSIONS (48)
#define IMG_MARGIN     (4)

static uint16_t slave_difficulty = difficulty;

static uint8_t const buttons[] = { PIN_BTN_LEFT, PIN_BTN_MIDDLE, PIN_BTN_RIGHT };
/* Other images can be found in the module's img/ subdirectory */
static char const * const filenames[] = { "cat.bmp", "logo.bmp", "vim.bmp" };
static PImage images[] = { {}, {}, {} };

#define NB_IMGS (sizeof(images) / sizeof(images[0]))
#define NB_BTNS (sizeof(buttons) / sizeof(buttons[0]))

typedef struct
{
  uint8_t const id[NB_IMGS];
  uint8_t const solution;
} t_pattern;

static t_pattern const patterns[MAX_DIFFICULTY] = {
  {{ 0, 1, 2 }, PIN_BTN_LEFT},
  {{ 2, 0, 1 }, PIN_BTN_MIDDLE},
  {{ 2, 2, 0 }, PIN_BTN_RIGHT}
};

#define sd_cs   (4)
#define lcd_cs  (10)
#define dc      (9)
#define rst     (8)

TFT TFTscreen = TFT(lcd_cs, dc, rst);

void loadImages(void)
{
  for (size_t i = 0 ; i < NB_IMGS ; ++i)
    images[i] = TFTscreen.loadImage(filenames[(slave_difficulty - 1) * NB_IMGS + i]);
}

void updateDifficulty(void)
{
  int const value_0 = analogRead(PIN_DIFFICULTY_0);
  int const value_1 = analogRead(PIN_DIFFICULTY_1);

  if (value_0 < 100 && value_1 < 100 && slave_difficulty != EASY)
  {
    slave_difficulty = EASY;
    printImages();
  }
  else if (value_0 > 100 && value_1 < 100 && slave_difficulty != MEDIUM)
  {
    slave_difficulty = MEDIUM;
    printImages();
  }
  else if (value_0 > 100 && value_1 > 100 && slave_difficulty != HARD)
  {
    slave_difficulty = HARD;
    printImages();
  }
}

void printImages(void)
{
  static uint8_t const y = TFTscreen.height() / 2 - IMG_DIMENSIONS / 2;

  for (size_t i = 0 ; i < NB_IMGS ; ++i)
    TFTscreen.image(images[patterns[slave_difficulty - 1].id[i]],
    IMG_MARGIN + (IMG_DIMENSIONS + IMG_MARGIN) * i,
    y);
}

void setup(void)
{
  if (!SD.begin(sd_cs))
    return;
  pinMode(PIN_DIFFICULTY_0, INPUT);
  pinMode(PIN_DIFFICULTY_1, INPUT);
  pinMode(PIN_PENALITY, OUTPUT);
  pinMode(PIN_DEFUSAL, OUTPUT);
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  loadImages();
  printImages();
}

void sendSignal(uint8_t const pin)
{
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}

void loop(void)
{
  updateDifficulty();
  for (size_t i = 0 ; i < NB_BTNS ; ++i)
  {
    if (digitalRead(buttons[i]) == HIGH)
    {
      if (patterns[slave_difficulty - 1].solution == buttons[i])
        sendSignal(PIN_DEFUSAL);
      else
        sendSignal(PIN_PENALITY);
      delay(500);
    }
  }
  delay(100);
}

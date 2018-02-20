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

#define PIN_DIFFICULTY (2) /* interrupt to sync this module difficulty */
#define PIN_PENALITY (6)
#define PIN_DEFUSAL (7)

#define PIN_BTN_LEFT (1)
#define PIN_BTN_MIDDLE (3)
#define PIN_BTN_RIGHT (5)

#define PATTERNS (3)
#define IMG_DIMENSIONS (48)
#define IMG_MARGIN (4)

static uint16_t slave_difficulty = difficulty;
static uint8_t btns[PATTERNS] = { PIN_BTN_LEFT, PIN_BTN_MIDDLE, PIN_BTN_RIGHT };

typedef struct
{
  uint8_t const id[PATTERNS];
  uint8_t const solution;
} t_pattern;

static t_pattern const patterns[MAX_DIFFICULTY] = {
  {{ 1, 0, 0 }, 2},
  {{ 1, 1, 0 }, 1},
  {{ 1, 1, 1 }, 2}
};

static char const * const filenames[] = { "cat.bmp", "logo.bmp", "sample.bmp", "jeremy.bmp", "hexagon.bmp", "vim.bmp", "w2d.bmp", "resitor.bmp", "epitech.bmp" };
static PImage images[] = { {}, {}, {}, {}, {}, {}, {}, {}, {} };

#define sd_cs 4
#define lcd_cs 10
#define dc 9
#define rst 8

// screen
TFT TFTscreen = TFT(lcd_cs, dc, rst);

void loadImages(void)
{
  for (size_t i = 0 ; i < sizeof(images) / sizeof(images[0]) ; ++i)
    images[i] = TFTscreen.loadImage(filenames[i]);
}

void handleDifficulty(void)
{
  if (slave_difficulty++ == MAX_DIFFICULTY)
    slave_difficulty = EASY;
  Serial.print("handle diff, new diff=");
  Serial.println(slave_difficulty);
  printImages();
}

void printImages(void)
{
  Serial.println("Print!");
  uint8_t y = TFTscreen.height() / 2 - IMG_DIMENSIONS / 2;
  Serial.print("slave diff=");
  Serial.println(slave_difficulty);
  for (size_t i = 0 ; i < PATTERNS ; ++i)
  {
    TFTscreen.image(images[patterns[slave_difficulty - 1].id[i]], IMG_MARGIN + (IMG_DIMENSIONS + IMG_MARGIN) * i, y);
    Serial.println(IMG_MARGIN + (IMG_DIMENSIONS + IMG_MARGIN) * i);
    Serial.println(y);
  }
}

void setup()
{
  delay(2000);
  Serial.begin(9600);
  if (!SD.begin(sd_cs))
  {
    Serial.println("sd");
    return;
  }
  pinMode(PIN_DIFFICULTY, INPUT);
  pinMode(PIN_PENALITY, OUTPUT);
  pinMode(PIN_DEFUSAL, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_DIFFICULTY), &handleDifficulty, RISING);
  TFTscreen.begin();
  loadImages();
  TFTscreen.background(0, 0, 0);
  printImages();
  Serial.println("Setup ok");
}

void sendSignal(int pin)
{
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}

void loop()
{
  for (size_t i = 0 ; i < sizeof(btns) / sizeof(btns[0]) ; ++i)
  {
    if (digitalRead(btns[i]) == HIGH)
    {
      if (patterns[slave_difficulty - 1].solution == btns[i])
        sendSignal(PIN_DEFUSAL);
      else
        sendSignal(PIN_PENALITY);
    }
  }
  delay(100);
}

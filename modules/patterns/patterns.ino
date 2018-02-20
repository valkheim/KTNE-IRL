/****************************************
 * patterns
 *
 * Charles Paulet
 *
 * Patterns are displayed on screen, user
 * must describe them to its teammate and
 * push the button he advised.
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

#define sd_cs 4
#define lcd_cs 10
#define dc 9
#define rst 8

// screen
TFT TFTscreen = TFT(lcd_cs, dc, rst);

// image to be drawn on screen
PImage img;

void setup()
{
  setupCore();
  Serial.begin(9600);
  if (!SD.begin(sd_cs))
  {
    Serial.println("Cannot load SD");
    return;
  }
  // initialize and clear the GLCD screen
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);
  // load image
  img = TFTscreen.loadImage("sample.bmp");
  if (!img.isValid())
  {
    Serial.println("Cannot load image file");
    return;
  }
}

void loop()
{
  int const x = random(TFTscreen.width() - img.width());
  int const y = random(TFTscreen.height() - img.height());
  // clear screen
  TFTscreen.background(0, 0, 0);
  // draw the image to the screen
  TFTscreen.image(img, 0, 0);
  TFTscreen.image(img, TFTscreen.width() - img.width(), TFTscreen.height() - img.height());
  // wait a little bit before drawing again
  delay(1500);
}

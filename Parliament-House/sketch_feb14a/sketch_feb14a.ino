#include <SPI.h>

#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h>
#define TFT_CS         14
#define TFT_RST        15
#define TFT_DC         32

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
char* textstring = "work this time";

void setup() {
   tft.initR(INITR_BLACKTAB);
  // put your setup code here, to run once:
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(ST77XX_BLACK);
  drawtext(textstring, ST77XX_WHITE, 0, 30);
  sleep(100000);
}

void drawtext(char* text, uint16_t colour, int xmod, int ymod) {
  tft.setCursor(xmod, ymod);
  tft.setTextColor(colour);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print(text);
}

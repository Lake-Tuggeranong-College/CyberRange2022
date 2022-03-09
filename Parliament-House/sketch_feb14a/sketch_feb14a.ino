#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>

#define TFT_CS         14
#define TFT_RST        15
#define TFT_DC         32

#define Neutral 0
#define Press 1
#define Up 2
#define Down 3
#define Right 4
#define Left 5

int numAlpha = 0;
int numBravo = 0;
int numCharlie = 0;
int numDelta = 0;


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
char* textstring = "work this time";

void setup() {
  tft.initR(INITR_BLACKTAB);
  // put your setup code here, to run once:
  tft.setRotation(1);
  Serial.begin(9600);
}

void loop() {
  checkVote();
  updateVote();
};

void drawtext(String text, uint16_t colour, int xmod, int ymod) {
  tft.setCursor(xmod, ymod);
  tft.setTextColor(colour);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print(text);
}

int CheckJoystick()
{
  //I duno what pin should be used here
  int joystickState = analogRead(1);

  if (joystickState < 50) return Left;
  if (joystickState < 150) return Down;
  if (joystickState < 250) return Press;
  if (joystickState < 500) return Right;
  if (joystickState < 650) return Up;
  return Neutral;
}

void checkVote() {
  int joyStick = CheckJoystick();
  switch (joyStick) {
    case Neutral:
      return;
      break;
    case Up:
      numAlpha++;
      break;
    case Right:
      numBravo++;
      break;
    case Down:
      numCharlie++;
      break;
    case Left:
      numDelta++;
      break;
  }
}

void updateVote() {
  String alpha = "0";
  String bravo = "0";
  String charlie = "0";
  String delta = "0";
  alpha = String(numAlpha);
  bravo = String(numBravo);
  charlie = String(numCharlie);
  delta = String(numDelta);
  tft.fillScreen(ST77XX_BLACK);
  drawtext(delta, ST77XX_WHITE, 35, 60);
  drawtext(charlie, ST77XX_WHITE, 35, 30);
  drawtext(bravo, ST77XX_WHITE, 5, 60);
  drawtext(alpha, ST77XX_WHITE, 5, 30);
  sleep(50);
}

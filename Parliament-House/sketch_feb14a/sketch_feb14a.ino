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
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(1);
  Serial.begin(9600);
}

void loop() {
  checkVote();
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

  int numAlpha = 0;
  int numBravo = 0;
  int numCharlie = 0;
  int numDelta = 0;
  String alpha = "A";
  String bravo = "B";
  String charlie = "C";
  String delta = "D";
  switch (joyStick) {
    case Up:
      numAlpha++;
      alpha = String(numAlpha);
      drawtext(alpha, ST77XX_WHITE, 0, 30);
      break;
    case Right:
      numBravo++;
      bravo = String(numBravo);
      drawtext(bravo, ST77XX_WHITE, 0, 60);
      break;
    case Down:
      numCharlie++;
      charlie = String(numCharlie);
      drawtext(charlie, ST77XX_WHITE, 30, 30);
      break;
    case Left:
      numDelta++;
      delta = String(numDelta);
      drawtext(delta, ST77XX_WHITE, 30, 60);
      break;
  }
}

/*void voteToString () {
   alpha = String(numAlpha);
   bravo = String(numBravo);
   charlie = String(numCharlie);
   delta = String(numDelta);
  }*/

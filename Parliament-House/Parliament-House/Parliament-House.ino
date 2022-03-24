/*
   Code explanation goes here....

   This is the master copy of the code to start building your Arduino based
   Cyber Range component.
*/


/***************************************************
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "sensitiveInformation.h"

#define FORMAT_SPIFFS_IF_FAILED true

// Wifi & Webserver
#include "WiFi.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

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

bool stoppedVote = false;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
char* textstring = "work this time";
// 2.13" Monochrome displays with 250x122 pixels and SSD1675 chipset
//ThinkInk_213_Mono_B72 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

// EINK End


// RTC Start - Remove if unnecessary
#include "RTClib.h"

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// RTC End

const int LOOPDELAY = 25;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    // Follow instructions in README and install
    // https://github.com/me-no-dev/arduino-esp32fs-plugin
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Wifi Configuration
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println();
  Serial.print("Connected to the Internet");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



  routesConfiguration(); // Reads routes from routesManagement

  server.begin();



  // RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //    abort();
  }

  // The following line can be uncommented if the time needs to be reset.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.start();
  logEvent("System Initialisation...");
}

void loop() {
  updateVote();
  delay(LOOPDELAY);
}

void logEvent(String dataToLog) {
  /*
     Log entries to a file stored in SPIFFS partition on the ESP32.
  */
  // Get the updated/current time
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d,%02d,%02d,%02d,%02d,%02d,",  rightNow.year(), rightNow.month(), rightNow.day(), rightNow.hour(), rightNow.minute(), rightNow.second());

  String logTemp = csvReadableDate + dataToLog + "\n"; // Add the data to log onto the end of the date/time

  const char * logEntry = logTemp.c_str(); //convert the logtemp to a char * variable

  //Add the log entry to the end of logevents.csv
  //  appendFile(SPIFFS, "/logEvents.csv", logEntry);

  // Output the logEvents - FOR DEBUG ONLY. Comment out to avoid spamming the serial monitor.
  //  readFile(SPIFFS, "/logEvents.csv");

  Serial.print("\nEvent Logged: ");
  Serial.println(logEntry);
}

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
  float joystickState = analogRead(A12);

  if (joystickState < 50) return Left;
  if (joystickState < 150) return Down;
  if (joystickState < 250) return Press;
  if (joystickState < 500) return Right;
  if (joystickState < 650) return Up;
  Serial.println(joystickState);
  return Neutral;
}

void checkVote() {
  int joyStick = CheckJoystick();
  switch (joyStick) {
    case Neutral:
      return;
      break;
    case Up:
      increaseAlpha(1);
      break;
    case Right:
      increaseBravo(1);
      break;
    case Down:
      increaseCharlie(1);
      break;
    case Left:
      increaseDelta(1);
      break;
  }
}

//functions to increase voting totals, other files don't seem to work with "numVote++" for whatever reason.
void increaseAlpha(int amount) {
  numAlpha + amount;
}

void increaseBravo(int amount) {
  numBravo + amount;
}

void increaseCharlie(int amount) {
  numCharlie + amount;
}

void increaseDelta(int amount) {
  numDelta + amount;
}

/*
        <a href="/VoteAlpha">Vote Alpha</a>
        <a href="/VoteBravo">Vote Bravo</a>
        <a href="/VoteCharlie">Vote Charlie</a>
        <a href="/VoteDelta">Vote Delta</a>
        <a href="/StopTheCount">Cease Voting</a>
        <a href="/ContinueVoting">Continue Voting</a>
        <a href="/Reset">Reset Votes</a>
*/
void updateVote() {
  if (stoppedVote == true) 
    return;
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
  sleep(20);
}

void stopTheCount() {
  stoppedVote = true;
  logEvent("Voting was stopped");
  Serial.println("Voting stopped!");
}

void continueVoting() {
  stoppedVote = false;
  logEvent("Voting was resumed");
  Serial.println("Voting resumed!");
}

void resetVotes() {
  if (stoppedVote == true)
    return;
  logEvent("Votes were reset");
  numAlpha = 0;
  numBravo = 0;
  numCharlie = 0;
  numDelta = 0;
}

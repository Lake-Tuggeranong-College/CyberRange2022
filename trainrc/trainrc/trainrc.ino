/*
   Code explanation goes here....

   This is the master copy of the code to start building your Arduino based
   Cyber Range component.
*/
#include <PowerFunctions.h>   //Power Functions Library

//IR Channels
#define CH1 0x0
#define CH2 0x1
#define CH3 0x2
#define CH4 0x3

//IR Transmission
#define IR_TRANS_IN   21  //IR Trans PIN
#define IR_DEBUG_OFF  0  //IR Debug Mode Off
#define IR_DEBUG_ON   1  //IR Debug Mode On

//Call PowerFunctions parameters
PowerFunctions pf(IR_TRANS_IN, CH1, IR_DEBUG_ON);

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;



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

// EINK Start - Remove if unnecessary
#include "Adafruit_ThinkInk.h"

#define EPD_CS      15
#define EPD_DC      33
#define SRAM_CS     32
#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)

// 2.13" Monochrome displays with 250x122 pixels and SSD1675 chipset
ThinkInk_213_Mono_B72 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

// EINK End

// IR TRansmitter
#define IRTransmitterPin 21

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
  delay(1000);

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

  //EINK
  display.begin(THINKINK_MONO);
  display.clearBuffer();
  updateEPD ();
  logEvent("System Initialisation...");

  pinMode(IRTransmitterPin, OUTPUT);
}

void loop() {
  delay(LOOPDELAY);
}

void logEvent(String dataToLog) {
  /*
     Log entries to a file stored in SPIFFS partition on the ESP32.
  */
  // Get the updated/current time
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d,%02d,%02d,%02d,%02d,%02d,", rightNow.hour(), rightNow.minute(), rightNow.second(), rightNow.day(), rightNow.month(), rightNow.year());

  String logTemp = csvReadableDate + dataToLog + "\n"; // Add the data to log onto the end of the date/time

  const char * logEntry = logTemp.c_str(); //convert the logtemp to a char * variable

  //Add the log entry to the end of logevents.csv
  appendFile(SPIFFS, "/logEvents.csv", logEntry);

  // Output the logEvents - FOR DEBUG ONLY. Comment out to avoid spamming the serial monitor.
  //  readFile(SPIFFS, "/logEvents.csv");

  Serial.print("\nEvent Logged: ");
  Serial.println(logEntry);
}

void updateEPD() {
  /*
     "Draws" updates to the Eink (E Paper Display -EPD) display.
     Configures all in memory and only writes to the display using 'display.display();'

     Update this as necessary.
  */

  // Display IP Address
  drawText(WiFi.localIP().toString(), EPD_BLACK, 1, 130, 80);


  logEvent("Updating the EPD");
  display.display();

}

//String getDateTime() {
//  DateTime rightNow = rtc.now();
//  char csvReadableDate[25];
//
//}
void drawText(String text, uint16_t color, int textSize, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(color);
  display.setTextSize(textSize);
  display.setTextWrap(true);
  display.print(text);
}

// Single and dual motor control is defined
void step(uint8_t output, uint8_t pwm, uint16_t time) {
  pf.combo_pwm(output, pwm);
  pf.single_pwm(output, pwm);
}
// Single increment for speed is defined
void increment(uint8_t output) {
  pf.single_increment(output);
}
// Single decrement for speed is defined
void decrement(uint8_t output) {
  pf.single_decrement(output);
}

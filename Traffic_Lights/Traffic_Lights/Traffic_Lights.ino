#include <AsyncTCP.h>

#include "sensitiveInformation.h"

#define FORMAT_SPIFFS_IF_FAILED true

// Traffic lights colour definitions
#define LIGHT_1_RED 13
#define LIGHT_1_YELLOW 12
#define LIGHT_1_GREEN 27

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


// RTC Start - Remove if unnecessary
#include "RTClib.h"

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// RTC End

const int LOOPDELAY = 25;

boolean trafficLightsOn = true;

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

  // initializes the traffic lights as the outputs
  pinMode(LIGHT_1_RED, OUTPUT);
  pinMode(LIGHT_1_YELLOW, OUTPUT);
  pinMode(LIGHT_1_GREEN, OUTPUT);

  digitalWrite(LIGHT_1_GREEN, HIGH);
  digitalWrite(LIGHT_1_YELLOW, HIGH);
  digitalWrite(LIGHT_1_RED, HIGH);

  delay(5000);

  routesConfiguration(); // Reads routes from routesManagement

  server.begin();


  // RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //    abort();
  }

  // The following line can be uncommented if the time needs to be reset.
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  rtc.start();

  //EINK
  display.begin(THINKINK_MONO);
  display.clearBuffer();

  logEvent("System Initialisation...");
  updateEPD();
}

void loop() {
  lightSwitching();
  delay(LOOPDELAY);
}

void lightSwitching() {
  if (trafficLightsOn) {
    // Lights switch until hacked
    digitalWrite(LIGHT_1_RED, HIGH);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
    delay(1000);
    digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, HIGH);   // turn the red LED on (HIGH is the voltage level)
    delay(1000);
    digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, HIGH);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
    delay(300);
  } else {
    // how lights function when hacked
    digitalWrite(LIGHT_1_RED, HIGH);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, HIGH);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
    digitalWrite(LIGHT_1_GREEN, HIGH);   // turn the red LED on (HIGH is the voltage level)
    delay(100);
  }
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
  drawText(WiFi.localIP().toString(), EPD_BLACK, 1, 0, 0);


  logEvent("Updating the EPD");
  display.display();

}

void drawText(String text, uint16_t color, int textSize, int x, int y) {
  display.setCursor(x, y);
  display.setTextColor(color);
  display.setTextSize(textSize);
  display.setTextWrap(true);
  display.print(text);
}

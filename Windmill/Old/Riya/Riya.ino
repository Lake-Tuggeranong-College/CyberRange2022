//Wifi
#define FORMAT_SPIFFS_IF_FAILED true

// Wifi & Webserver
#include "WiFi.h"
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include "wifiConfig.h"
AsyncWebServer server(80);
#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int servoPin = 12; //pin which the servo is attached to 


// RTC
#include "RTClib.h"

RTC_PCF8523 rtc;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin, 1000, 2000); 

  while (!Serial) {
    delay(10);
  }
  delay(1000);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Webserver

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Configure web routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor);
  });
  server.on("/logOutput", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/logEvents.csv", "text/html", true);
  });

  server.begin(); // Start the webserver

  // RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  rtc.start();
  logEvent("System Initialisation...");
}

void windmill () {
  //the code within this function at this stage just turns on the servo to move the windill
  myservo.write(pos);    // tell servo to go to position in variable 'pos'
  delay(15);
}

void loop() {
  windmill(); 
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

String processor(const String& var) {
  Serial.println(var);

  if (var == "DATETIME") {
    String datetime = getTimeAsString() + " " + getDateAsString();
    return datetime;
  }
  if (var == "EXAMPLEOUTPUT") {
    // Process data...
    return "This is example data";
  }

  return String();
}

String getDateAsString() {
  DateTime now = rtc.now();

  // Converts the date into a human-readable format.
  char humanReadableDate[20];
  sprintf(humanReadableDate, "%02d/%02d/%02d", now.day(), now.month(), now.year());

  return humanReadableDate;
}

String getTimeAsString() {
  DateTime now = rtc.now();

  // Converts the time into a human-readable format.
  char humanReadableTime[20];
  sprintf(humanReadableTime, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

  return humanReadableTime;
}


// SPIFFS File Management functions
void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

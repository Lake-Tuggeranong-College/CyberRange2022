# Arduino Master

This folder contains the initial copy of the Arduino Sketch for the Adafruit Huzzah32 devices.

This code is configured with the SPIFFS file management code (add/remove/edit files) as well as basic website routes configured.

After duplicating this sketch, create a folder in the repository for your component. Modify the code and routes to add the functionality required.

## Note

After opening this project in the Arduino IDE, you will need to add a new file into the project called sensitiveInformation.h (case sensitive).

The contents of this file are the user names and passwords to the Wifi access point, and for the user to access the site running on the Arduino.

The template is:

```
const char* host = "";
const char* ssid = "";             // Wifi Network Name
const char* password = "";         // Wifi Password


const char* http_username = "";     // Username and password for user access
const char* http_password = "";     // to site
```

void routesConfiguration() {

  // Example of a 'standard' route
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", "text/html" , false, processor);
  });

  // Duplicated serving of index.html route, so the IP can be entered directly to browser.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /");
    request->send(SPIFFS, "/index.html", "text/html" , false, processor);
  });

  server.on("/TrainSpeed1", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainspeed1");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false , processor);
  });

  server.on("/TrainSpeed2", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainspeed2");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false, processor);
  });

  server.on("/TrainSpeed3", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainspeed3");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false, processor);
  });

  server.on("/TrainReverse1", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainreverse1");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false, processor);
  });

  server.on("/TrainReverse2", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainreverse2");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false, processor);
  });

  server.on("/TrainReverse3", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /Trainreverse3");
    request->send(SPIFFS, "/dashboard.html", "text/html" , false, processor);
  });

  // Example of linking to an external file
  server.on("/arduino.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/arduino.css", "text/css" , false, processor);
  });

  //traindude.jpeg
 server.on("/traindude.jpeg", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /img.jpg");
    request->send(SPIFFS, "/img.jpg", "image/jpeg");
  });

  // Example of a route with additional authentication (popup in browser)
  // And uses the processor function.
  server.on("/dashboard.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor);
  });

  // Example of route with authentication, and use of processor
  // Also demonstrates how to have arduino functionality included (turn LED on)
  server.on("/LEDOn", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    digitalWrite(LED_BUILTIN, HIGH);
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor );
  });

  // Example of route which sets file to download - 'true' in send() command.
  server.on("/logOutput", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("output");
    request->send(SPIFFS, "/logEvents.csv", "text/html", true);



  });
}
String getDateTime() {
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d:%02d:%02d %02d/%02d/%02d%",  rightNow.hour(), rightNow.minute(), rightNow.second(), rightNow.day(), rightNow.month(), rightNow.year());
  return csvReadableDate;
}

String processor(const String& var) {
  
  if (var == "DATETIME") {
    String datetime = getDateTime();
    return datetime;

  }

  // Default "catch" which will return nothing in case the HTML has no variable to replace.
  return String();
}

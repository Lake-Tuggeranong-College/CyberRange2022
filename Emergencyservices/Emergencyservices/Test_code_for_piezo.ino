int speakerPin = 8; // control horn pin
int potPin = 4; // control pin adjustable resistor
int value = 0;

void setup() {
pinMode(speakerPin, OUTPUT);
}

void loop() {
value = analogRead(potPin); // reading resistor values pin
digitalWrite(speakerPin, HIGH);
delay(value); // adjust the speaker sound of the time;
digitalWrite(speakerPin, LOW);
delay(value); // adjust the speaker does not ring a time;
}

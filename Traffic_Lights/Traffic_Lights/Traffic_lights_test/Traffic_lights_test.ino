// Traffic lights colour definitions
#define LIGHT_1_RED 2
#define LIGHT_1_YELLOW 3
#define LIGHT_1_GREEN 4

// Constants used here to set pin numbers:
const int Light_1_RED = A1; // the number of the red LED pin
const int Light_1_YELLOW = A2; // the number of the Yellow LED pin
const int Light_1_GREEN = A3; // the number of the green LED pin

void setup() {
  // Traffic light serial setup
Serial.begin(9600);
// initializes the traffic lights as the outputs
  pinMode(LIGHT_1_RED, OUTPUT);
  pinMode(LIGHT_1_YELLOW, OUTPUT);
  pinMode(LIGHT_1_GREEN, OUTPUT);

  digitalWrite(LIGHT_1_GREEN, HIGH);
  digitalWrite(LIGHT_1_YELLOW, HIGH);
  digitalWrite(LIGHT_1_RED, HIGH);  

  delay(5000);
}

void loop() {
  // Traffic Light cycle
  digitalWrite(LIGHT_1_RED, HIGH);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
  delay(10000);
  digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_GREEN, HIGH);   // turn the red LED on (HIGH is the voltage level)
  delay(10000);
  digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_YELLOW, HIGH);   // turn the red LED on (HIGH is the voltage level)
  digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
  delay(3000);
}

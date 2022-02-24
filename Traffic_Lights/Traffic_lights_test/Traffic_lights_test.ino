// Traffic lights colour definitions
#define LIGHT_1_RED 13
#define LIGHT_1_YELLOW 12
#define LIGHT_1_GREEN 27

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

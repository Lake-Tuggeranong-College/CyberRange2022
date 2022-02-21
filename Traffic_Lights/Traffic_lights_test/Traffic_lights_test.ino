// Traffic lights timer definitions
// #define RED_SET_TIME 5000
// #define YELLOW_SET_TIME 2000
// #define GREEN_SET_TIME 5000

/*
   Ryan:
   Thse definitions are duplicated below as well. I would suggest deleting these three #define commands.
*/

// Traffic lights colour definitions
#define LIGHT_1_RED 2
#define LIGHT_1_YELLOW 3
#define LIGHT_1_GREEN 4

/*
   Ryan: I can't see these being used anywhere else. Will these be used later?
*/

// Light definitions
#define GREEN_LIGHT 0
#define YELLOW_LIGHT 1
#define RED_LIGHT 2

/*
   Ryan:
   The pins below don't match the wiring diagram. Diagrm shows A1, A2, A3 are used.
   According to this page: https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/pinouts
   Those pins are 25, 34, & 39 respectively.
   So you will need to change those pin numbers below to either A1, A2, A3 or 25, 34, & 39.
*/

// Constants used here to set pin numbers:
const int Light_1_RED = 11; // the number of the red LED pin
const int Light_1_YELLOW = 12; // the number of the Yellow LED pin
const int Light_1_GREEN = 13; // the number of the green LED pin




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

  /*
   * Ryan:
   * Put a delay(5000) in here so you can see if the LEDS are working correctly 
   * when the board first starts up.
   */
}

void loop() {

  /*
   * Ryan:
   * This code will go through and turn each LED on for 1 second at a time. 
   * With a second in between each LED. Traffic lights don't/shouldn't have a gap.
   * 
   * Traffic lights run (for e.g.) red for 10 seconds, green for 10 seconds, yellow for 3 seconds.
   * Rinse, Repeat.
   * 
   * So the logic in the loop function should be something like:
   * digitalWrite(LIGHT_1_RED, HIGH);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
   * delay(10000);
   * digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_YELLOW, LOW);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_GREEN, HIGH);   // turn the red LED on (HIGH is the voltage level)
   * delay(10000);
   * digitalWrite(LIGHT_1_RED, LOW);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_YELLOW, HIGH);   // turn the red LED on (HIGH is the voltage level)
   * digitalWrite(LIGHT_1_GREEN, LOW);   // turn the red LED on (HIGH is the voltage level)
   * delay(3000);
   * 
   * Try that and see how that goes..
   */

  
  // Traffic Light cycle
  digitalWrite(LIGHT_1_RED, HIGH);   // turn the red LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LIGHT_1_RED, LOW);    // turn the red LED off by making the voltage LOW
  delay(1000);

  digitalWrite(LIGHT_1_YELLOW, HIGH);   // turn the yellow LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LIGHT_1_YELLOW, LOW);    // turn the yellow LED off by making the voltage LOW
  delay(1000);

  digitalWrite(LIGHT_1_GREEN, HIGH);   // turn the green LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LIGHT_1_GREEN, LOW);    // turn the green LED off by making the voltage LOW
  delay(1000);
}

// Ryan's Note: Arduino Board - ESP32 Wrover Module.

/*
 * Program:
 *  5_Wire_Read_05
 * Created by:
 *  Jonathon Weeks
 * On date:
 *  2018-10-22
 * 
 * Description:
 *  This program is a state machine that waits for a button-press
 *  before reading a coordinate pair from a 5-wire resisitve touch
 *  panel. The coordinate pair is then sent to the computer via
 *  Serial.
 * 
 * Function:
 *  Upon button press:
 *    LED turns on, and a message is sent through Serial, indicating
 *      that the process has started.
 *    X and Y coordinates are read from the panel, with pauses while
 *      the panel's voltages settle.
 *    X and Y coordinates are sent to the computer through Serial.
 *    The panel is turned off.
 *    Time is checked to allow enough time for button debouncing.
 *    The LED turns off, and a message is sent through Serial,
 *      indicating that the process has finished.
 *  Additionally, a function is used to determine elapsed time,
 *    rather than simple subtraction. This accounts for looping of
 *    the internal clock value.
 * Plan:
 */

//~~~~~~~~
//~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Libraries
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Loop_Diff.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int temp;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// State variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int state;
#define STATE_START 0

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Button variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int btn_prev;
unsigned long btn_time;
#define DEBOUNCE_TIME 100

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Touch panel variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PIN_TR 3
#define PIN_TL 2
#define PIN_S  A0
#define PIN_BL 1
#define PIN_BR 0
int x_pos;
int y_pos;
unsigned long panel_time;
#define SETTLE_TIME 10


void setup()
{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Establish pin modes
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //~~~~~~~~~~~~~~~~
  // Output
  //~~~~~~~~~~~~~~~~
  // LED
  pinMode(PIN_LED1, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  // Touch panel
  pinMode(PIN_TR, OUTPUT);
  pinMode(PIN_TL, OUTPUT);
  pinMode(PIN_BL, OUTPUT);
  pinMode(PIN_BR, OUTPUT);
  digitalWrite(PIN_TR, LOW);
  digitalWrite(PIN_TL, LOW);
  digitalWrite(PIN_BL, LOW);
  digitalWrite(PIN_BR, LOW);
  
  //~~~~~~~~~~~~~~~~
  // Input
  //~~~~~~~~~~~~~~~~
  // Button
  pinMode(PIN_BTN1, INPUT);
  // Touch panel
  pinMode(PIN_S, INPUT);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Initialize variables
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // State Machine
  state = STATE_START;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Begin Serial Communication
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Serial.begin(9600);
}

void loop()
{
  switch(state)
  {
    case 0: // Wait for button press
      // What is the current state of our button?
      temp = digitalRead(PIN_BTN1);
      
      // Has the button been pressed?
      if ((temp == HIGH) & (btn_prev == LOW))
      { // If it has...
        // Turn on the LED
        digitalWrite(PIN_LED1, HIGH);
        
        // Record button time
        btn_time = millis();
        
        // Switch to the Set up X state
        state = 1;
      } // Otherwise, do nothing
      break;
      
    case 1: // Set up X read
      //~~~~~~~~~~~~~~~~
      // Set up X read
      //~~~~~~~~~~~~~~~~
      // Set TR and BR high
      digitalWrite(PIN_TR, HIGH);
      digitalWrite(PIN_BR, HIGH);
      // Set TL and BL low
      digitalWrite(PIN_TL, LOW);
      digitalWrite(PIN_BL, LOW);

      //~~~~~~~~~~~~~~~~
      // Move to next state
      //~~~~~~~~~~~~~~~~
      // Record the time
      panel_time = millis();
      // Switch to waiting for the panel to settle
      state = 2;
      break;
    
    case 2: // Wait for panel to settle
      // How long has it been since the panel changed configuration?
      temp = loop_diff(panel_time, millis(), true);
      
      // Has it been long enough?
      if (temp >= SETTLE_TIME)
      { // If it has...
        
        // Switch to the Read X state
        state = 3;
      } // Otherwise, do nothing.
      break;

    case 3: // Read the X position
      // Read the sensor voltage (X position)
      x_pos = analogRead(PIN_S);

      // Switch to the set up Y state
      state = 4;
      break;
      
    case 4: // Set up Y read
      //~~~~~~~~~~~~~~~~
      // Set up Y read
      //~~~~~~~~~~~~~~~~
      // Set TR and TL high
      digitalWrite(PIN_TR, HIGH);
      digitalWrite(PIN_TL, HIGH);
      // Set BL and BR low
      digitalWrite(PIN_BL, LOW);
      digitalWrite(PIN_BR, LOW);

      //~~~~~~~~~~~~~~~~
      // Move to next state
      //~~~~~~~~~~~~~~~~
      // Record the time
      panel_time = millis();
      // Switch to waiting for the panel to settle
      state = 5;
      break;
    
    case 5: // Wait for panel to settle
      // How long has it been since the panel changed configuration?
      temp = loop_diff(panel_time, millis(), true);
      
      // Has it been long enough?
      if (temp >= SETTLE_TIME)
      { // If it has...
        
        // Switch to the Read Y state
        state = 6;
      } // Otherwise, do nothing.
      break;

    case 6: // Read the Y position
      // Read the sensor voltage (Y position)
      y_pos = analogRead(PIN_S);

      // Switch to the Output coords state
      state = 7;
      break;

    case 7: // Output coords to Serial
      // Write the coordinates to Serial
      Serial.print(x_pos);
      Serial.print("\t");
      Serial.println(y_pos);

      // Switch to the Turn off the panel state
      state = 8;
      break;

    case 8: // Turn off the panel
      // Turn off the panel
      digitalWrite(PIN_TR, LOW);
      digitalWrite(PIN_TL, LOW);
      digitalWrite(PIN_BL, LOW);
      digitalWrite(PIN_BR, LOW);

      // Switch to button debounce state
      state = 9;
      break;

    case 9: // Button debounce
      // How long has it been since we pressed the button?
      temp = loop_diff(btn_time, millis(), true);

      // Has it been long enough?
      if (temp >= DEBOUNCE_TIME)
      { // If it has...
        // Switch to the end state
        state = 10;
      } // Otherwise do nothing
      break;

    case 10: // End state
      // Turn off the LED
      digitalWrite(PIN_LED1, LOW);
      
      // Switch to the waiting state
      state = 0;
      break;
      
    default: // Error
      Serial.println("Something has gone terribly wrong.");
      break;
  }
}

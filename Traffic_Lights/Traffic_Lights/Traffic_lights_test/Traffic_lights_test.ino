  // Traffic lights timer definitions
#define RED_SET_TIME 5000
#define YELLOW_SET_TIME 2000
#define GREEN_SET_TIME 5000

  // Traffic lights colour definitions
#define LIGHT_1_RED 2
#define LIGHT_1_YELLOW 3
#define LIGHT_1_GREEN 4

  // Light definitions
#define GREEN_LIGHT 0
#define YELLOW_LIGHT 1
#define RED_LIGHT 2

  // ?
unsigned long gulStart_Timer = 0;

unsigned short gusSet_Time[3] = {GREEN_SET_TIME, YELLOW_SET_TIME, RED_SET_TIME};

short gsWhich_Light_Index = GREEN_LIGHT;

void setup() {
  // Traffic light serial setup
Serial.begin(9600);
  pinMode(LIGHT_1_RED, OUTPUT);
  pinMode(LIGHT_1_YELLOW, OUTPUT);
  pinMode(LIGHT_1_GREEN, OUTPUT);

  digitalWrite(LIGHT_1_GREEN, HIGH);
  digitalWrite(LIGHT_1_YELLOW, LOW);
  digitalWrite(LIGHT_1_RED, LOW);  
}

void loop() {
  // Traffic Light cycle
  if((millis()-gulStart_Timer) >= gusSet_Time[gsWhich_Light_Index])
  {
    gsWhich_Light_Index++;

    if(gsWhich_Light_Index > RED_LIGHT)
    {
      gsWhich_Light_Index = GREEN_LIGHT;
    }
    
    gulStart_Timer = millis();

    if(gsWhich_Light_Index == GREEN_LIGHT)
    {
      digitalWrite(LIGHT_1_GREEN, HIGH);
      digitalWrite(LIGHT_1_YELLOW, LOW);
      digitalWrite(LIGHT_1_RED, LOW);      
    }
    else if(gsWhich_Light_Index == YELLOW_LIGHT)
    {
      digitalWrite(LIGHT_1_GREEN, LOW);
      digitalWrite(LIGHT_1_YELLOW, HIGH);
      digitalWrite(LIGHT_1_RED, LOW);      
    }
    else if(gsWhich_Light_Index == RED_LIGHT)
    {
      digitalWrite(LIGHT_1_GREEN, LOW);
      digitalWrite(LIGHT_1_YELLOW, LOW);
      digitalWrite(LIGHT_1_RED, HIGH);      
    }
  }
}

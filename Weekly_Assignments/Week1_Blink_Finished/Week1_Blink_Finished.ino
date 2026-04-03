
const int BTN_SHORT = 2;
const int BTN_LONG = 3;
const int BTN_STANDARD = 4; //while the assignment doesn't call for it, we wanted to add a function that will set it back to the standard
const int LED_YELLOW = 12;

//using millis because delay can stop other functions from happening
unsigned long previousMillis = 0;
unsigned long interval = 1000;


bool LED_STATE = LOW;

//input happens (you press the button) output happens (the led blinks at different speeds)
void setup() {
  pinMode(LED_YELLOW, OUTPUT); //where the robot will output
  pinMode(BTN_SHORT, INPUT); //where the robot will input 
  pinMode(BTN_LONG, INPUT);
  pinMode(BTN_STANDARD, INPUT);
}

void loop() {

  //defines millis
  unsigned long currentMillis = millis();

  bool buttonStateShort = digitalRead(BTN_SHORT);
  bool buttonStateLong = digitalRead(BTN_LONG);
  bool buttonStateStandard = digitalRead(BTN_STANDARD);


  if (buttonStateShort == LOW) {
    interval = 500;
  }

  if (buttonStateLong == LOW) {
    interval = 2000;
  }
  if (buttonStateStandard == LOW) {
    interval = 1000;
  }
  
  //must be here otherwise won't work, checks if enough millis have passed
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

  //changes ledstate
    LED_STATE = !LED_STATE;
    digitalWrite(LED_YELLOW, LED_STATE);
  }
}

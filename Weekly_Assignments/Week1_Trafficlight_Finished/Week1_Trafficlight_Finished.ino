const int BTN = 2;
const int LED_RED = 13;
const int LED_YELLOW = 12;
const int LED_GREEN = 11;

int buttonState;

void setup() {
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(BTN, INPUT);

  //in the robot's hardware low and high are switched
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
}

void loop() {

  buttonState = digitalRead(BTN);


  //if button pressed
  if (buttonState == LOW) {

    //red turns off
    digitalWrite(LED_RED, HIGH);

    //green turns on
    delay(1000);
    digitalWrite(LED_GREEN, LOW);
    delay(3000);

    //green turns off and yellow turns on
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    delay(1000);

    //yellow turns off
    digitalWrite(LED_YELLOW, HIGH);

    //red turns on
    digitalWrite(LED_RED, LOW);  
  }
}

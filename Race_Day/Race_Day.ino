#include <Adafruit_NeoPixel.h>
#include <Servo.h>

//turn lights interval
unsigned long turnPreviousMillis = 0;
const unsigned long turnInterval = 250;

//end sequence black detection
unsigned long allBlackStartTime = 0;
bool allBlackDetected = false;

//checks for neopixel blinking and default setters, so that everything is at a known state
bool turnActive = false;
bool turnLeftActive = false;
bool turnRightActive = false;
bool turnLedState = false;
int turnBlinkCount = 0;
const int maxBlinks = 4;

//gripper settings
#define SERVO 12
#define gripperOpen 1600
#define gripperClose 1000

Servo gripperServo;

//Robot states for easier working
enum RobotState {
  WAIT_FOR_START,
  FLAG_RAISED,
  GRABBING_CONE,
  ON_BLACK_SQUARE,
  LINE_FOLLOWING,
  OBJECT_AVOIDANCE,
  END_SEQUENCE
};

RobotState currentRobotState = WAIT_FOR_START;

unsigned long stateStartTime = 0;

#define WAIT_TO_START 1000
#define DRIVE_TO_CONE 1000
#define TURN_TO_LINE 800
#define END_SQUARE_TIME 150

//object avoidance

bool avoidActive = false;
unsigned long avoidStart = 0;
int avoidStage = 0;

const unsigned long stopTime = 100;
bool motorsStopped = false;
bool avoiding = false;

//neopixel

#define PIN_NEO_PIXEL 13
#define NUM_PIXELS 4
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

//motor pins

const int mA1 = 10;
const int mA2 = 9;
const int mB1 = 6;
const int mB2 = 5;

//sensors

const int SENSOR_COUNT = 8;
const int sensors[SENSOR_COUNT] = {A0,A1,A2,A3,A4,A5,A6,A7};

#define trigPin 8
#define echoPin 7

bool sensorOnLine[SENSOR_COUNT];

const int threshold = 600;

int BASE_SPEED = 255;
int TURN_SPEED = 170;
int SHARP_TURN_SPEED = 90;

long duration;
int distance;

//line states for line recovery

enum LineState{
  LINE_FORWARD,
  LINE_LEFT,
  LINE_RIGHT,
  LINE_SHARP_LEFT,
  LINE_SHARP_RIGHT
};

LineState lastLineState = LINE_FORWARD;

void setup(){

  Serial.begin(9600);

  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  NeoPixel.begin();

  gripperServo.attach(SERVO);
  closeGripper();
}

void gripper(int pulse){
  gripperServo.writeMicroseconds(pulse);
}

void avoidObject(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}

void avoidSequence(){

  if (!avoidActive) return;

  unsigned long now = millis();

  switch (avoidStage){

    case 0:
      stopMotors();
      if (now - avoidStart >= stopTime){
        avoidStage = 1;
        avoidStart = now;
      }
      break;

    case 1:
      setMotor(200,0);
      startLeftSignal();
      if (now - avoidStart >= 700){
        avoidStage = 2;
        avoidStart = now;
      }
      break;

    case 2:
      setMotor(200,200);
      if (now - avoidStart >= 400){
        avoidStage = 3;
        avoidStart = now;
      }
      break;

    case 3:
      setMotor(0,200);
      startRightSignal();
      if (now - avoidStart >= 1000){
        avoidStage = 4;
        avoidStart = now;
      }
      break;

    case 4:
      setMotor(200,200);
      if (now - avoidStart >= 800){
        avoidStage = 5;
        avoidStart = now;
      }
      break;

    case 5:
      setMotor(0,200);
      startLeftSignal();
      if (now - avoidStart >= 1200){
        avoidStage = 6;
        avoidStart = now;
      }
      break;

    case 6:
      setMotor(200,200);
      if (now - avoidStart >= 300){
        avoidStage = 7;
        avoidStart = now;
      }
      break;

    case 7:
      setMotor(200,0);
      if (now - avoidStart >= 500){
        avoiding = false;
        avoidActive = false;
        stopMotors();
      }
      break;
  }
}

//turn signals

void startLeftSignal(){
  turnActive = true;
  turnLeftActive = true;
  turnRightActive = false;
  turnBlinkCount = 0;
}

void startRightSignal(){
  turnActive = true;
  turnLeftActive = false;
  turnRightActive = true;
  turnBlinkCount = 0;
}

void updateTurnSignals(){

  if(!turnActive) return;

  unsigned long currentMillis = millis();

  if(currentMillis - turnPreviousMillis >= turnInterval){

    turnPreviousMillis = currentMillis;
    turnLedState = !turnLedState;

    if(turnLedState){

      if(turnLeftActive){
        NeoPixel.setPixelColor(1, NeoPixel.Color(255,130,0));
        NeoPixel.setPixelColor(2, NeoPixel.Color(255,130,0));
      }

      if(turnRightActive){
        NeoPixel.setPixelColor(0, NeoPixel.Color(255,130,0));
        NeoPixel.setPixelColor(3, NeoPixel.Color(255,130,0));
      }

    }else{
      NeoPixel.clear();
    }

    NeoPixel.show();
    turnBlinkCount++;

    if(turnBlinkCount >= maxBlinks*2){
      turnActive = false;
      NeoPixel.clear();
      NeoPixel.show();
    }
  }
}

//motor logic

void setMotor(int leftMotor, int rightMotor){

  leftMotor  = constrain(leftMotor,-255,255);
  rightMotor = constrain(rightMotor,-255,255);

  if(leftMotor >= 0){
    analogWrite(mA1,0);
    analogWrite(mA2,leftMotor);
  }else{
    analogWrite(mA1,-leftMotor);
    analogWrite(mA2,0);
  }

  if(rightMotor >= 0){
    analogWrite(mB1,rightMotor);
    analogWrite(mB2,0);
  }else{
    analogWrite(mB1,0);
    analogWrite(mB2,-rightMotor);
  }
}

//gripper

void openGripper(){
  for(int i=0;i<1000;i++){
    gripper(gripperOpen);
    delay(1);
  }
}

void closeGripper(){
  for(int i=0;i<1000;i++){
    gripper(gripperClose);
    delay(1);
  }
}

//movement

void forward(){
  motorsStopped = false;
  setMotor(BASE_SPEED, BASE_SPEED);
}

void turnLeft(){
  motorsStopped = false;
  setMotor(0, TURN_SPEED);
  startLeftSignal();
}

void turnRight(){
  motorsStopped = false;
  setMotor(TURN_SPEED,0);
  startRightSignal();
}

void sharpLeft(){
  motorsStopped = false;
  setMotor(0, SHARP_TURN_SPEED);
  startLeftSignal();
}

void sharpRight(){
  motorsStopped = false;
  setMotor(SHARP_TURN_SPEED,0);
  startRightSignal();
}

void stopMotors(){
  setMotor(0,0);
  motorsStopped = true;
}

void backward(){
  motorsStopped = false;
  setMotor(-BASE_SPEED,-BASE_SPEED);
}

void readSensors(){
  for(int i=0;i<SENSOR_COUNT;i++){
    int value = analogRead(sensors[i]);
    sensorOnLine[i] = (value >= threshold);
  }
}

void loop(){

  unsigned long currentTime = millis();

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH,25000);

  if(duration>0){
    distance = duration*0.034/2;
  }
  else{
    distance = -1;
  }

 avoidObject();
 delay(50);
      
 int safeDistance = distance;
            
 static int hitCount = 0;

 if(distance < 7 || distance > 400){
    safeDistance = 999;
 }

 if (safeDistance <= 15){
    hitCount++;
 }
          
 else{
    hitCount = 0;
 }

  switch(currentRobotState){

    case WAIT_FOR_START:
      stopMotors();
      openGripper();
    
      if (distance == -1 || distance >= 20) {
        return;
      }
    
      // Flag is close → start!
      Serial.println("Flag detected");
      delay(WAIT_TO_START);
      stateStartTime = millis();
      currentRobotState = FLAG_RAISED;
      return;

    case FLAG_RAISED:
      forward();

      if(currentTime - stateStartTime >= DRIVE_TO_CONE){
        stopMotors();
        stateStartTime = currentTime;
        currentRobotState = GRABBING_CONE;
      }
      return;

    case GRABBING_CONE:
      stopMotors();
      closeGripper();

      if(currentTime - stateStartTime >= 800){
        stateStartTime = currentTime;
        currentRobotState = ON_BLACK_SQUARE;
      }
      return;

    case ON_BLACK_SQUARE:
      turnRight();

      if(currentTime - stateStartTime >= TURN_TO_LINE){
        currentRobotState = LINE_FOLLOWING;
      }
      return;

    case LINE_FOLLOWING:

        readSensors();
        updateTurnSignals();

        
        bool leftSide   = sensorOnLine[0] || sensorOnLine[1];
        bool leftMiddle = sensorOnLine[2] || sensorOnLine[3];
        bool rightMiddle= sensorOnLine[4] || sensorOnLine[5];
        bool rightSide  = sensorOnLine[6] || sensorOnLine[7];
      
        bool anyLine = leftSide || leftMiddle || rightMiddle || rightSide;

        bool allBlack = true;

        for(int i = 0; i < SENSOR_COUNT; i++){
          if(!sensorOnLine[i]){
          allBlack = false;
          break;
          }
         }
      
        if(!anyLine){
      
          switch(lastLineState){
            case LINE_LEFT: turnLeft(); 
            break;
            case LINE_RIGHT: turnRight(); 
            break;
            case LINE_SHARP_LEFT: sharpLeft(); 
            break;
            case LINE_SHARP_RIGHT: sharpRight(); 
            break;
            default: forward(); 
            break;
          }
      
          return;
      
        }
      
          if(leftSide && !leftMiddle && !rightMiddle){
            sharpLeft();
            lastLineState = LINE_SHARP_LEFT;
          }
          else if(leftMiddle && !rightMiddle){
            turnLeft();
            lastLineState = LINE_LEFT;
          }
          else if(rightSide && !rightMiddle && !leftMiddle){
            sharpRight();
            lastLineState = LINE_SHARP_RIGHT;
          }
          else if(rightMiddle && !leftMiddle){
            turnRight();
            lastLineState = LINE_RIGHT;
          }
          else{
            forward();
            lastLineState = LINE_FORWARD;
          }

           if (!avoidActive && hitCount >= 2){
              avoidActive = true;
              avoiding = true;
              avoidStart = millis();
              avoidStage = 0;
              currentRobotState = OBJECT_AVOIDANCE;
          }
          
          if(allBlack){
              if(!allBlackDetected){
                allBlackDetected = true;
                allBlackStartTime = millis();
              }
            
              if(millis() - allBlackStartTime >= END_SQUARE_TIME){
                stopMotors();
                currentRobotState = END_SEQUENCE;
                return;
              }
            }
            else{
              allBlackDetected = false;
            }

      return;
      
      case OBJECT_AVOIDANCE:
        
          if (avoiding){
              if (avoidActive){
                  avoidSequence();
                  return;
              }
            }

        currentRobotState = LINE_FOLLOWING;
       return;

       case END_SEQUENCE:

          stopMotors();
          openGripper();
          backward();
          delay(300);
          stopMotors();
        
          while(true);
          
  }
}

#define trigPin 8
#define echoPin 7

const int mA1 = 10;
const int mA2 = 9;
const int mB1 = 6;
const int mB2 = 5;

const int SENSOR_COUNT = 8;
const int sensors[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A7};

bool sensorOnLine[SENSOR_COUNT];

const int threshold = 500;

int BASE_SPEED = 200;
int TURN_SPEED = 170;
int SHARP_TURN_SPEED = 90;

long duration;
int distance;

int lineCount = 0;

void setup() {

  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}


void setMotor(int leftMotor, int rightMotor) {

  leftMotor  = constrain(leftMotor,  -255, 255);
  rightMotor = constrain(rightMotor, -255, 255);

  if (leftMotor >= 0) {
    analogWrite(mA1, 0);
    analogWrite(mA2, leftMotor);
  } 
  else {
    analogWrite(mA1, -leftMotor);
    analogWrite(mA2, 0);
  }

  if (rightMotor >= 0) {
    analogWrite(mB1, rightMotor);
    analogWrite(mB2, 0);
  } 
  else {
    analogWrite(mB1, 0);
    analogWrite(mB2, -rightMotor);
  }
}

void forward() {
  setMotor(BASE_SPEED, BASE_SPEED);
}

void turnLeft(){
  setMotor(0, TURN_SPEED);
}

void turnRight(){
  setMotor(TURN_SPEED, 0);
}

void sharpLeft() {
  setMotor(0, SHARP_TURN_SPEED);
}

void sharpRight() {
  setMotor(SHARP_TURN_SPEED, 0);
}

void motorStop() {
  setMotor(0, 0);
}


void readSensors() {

  for (int i = 0; i < SENSOR_COUNT; i++) {

    int value = analogRead(sensors[i]);

    sensorOnLine[i] = (value >= threshold);
  }
}

void loop() {

    Serial.println(lineCount);
    
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH, 25000);

    if (duration > 0) {
      distance = duration * 0.034 / 2;
    } 
    else {
      distance = -1;
    }

  readSensors();

  bool leftSide = sensorOnLine[0] || sensorOnLine[1];
  bool leftMiddle = sensorOnLine[2] || sensorOnLine[3];
  bool rightMiddle = sensorOnLine[4] || sensorOnLine[5];
  bool rightSide = sensorOnLine[6] || sensorOnLine[7];
  
  bool straightLine = leftMiddle || rightMiddle;
  bool fullLine = leftSide && leftMiddle && rightMiddle && rightSide;
  
      if(lineCount == 0){
      //if (distance > 0 && distance <= 20){
       // motorStop;
       // return;
      //}
      //else{
        forward();
        Serial.println("First Line");
      //}
    }
  
  if(fullLine){
    lineCount++;

    if(lineCount > 0 && lineCount < 4){
        Serial.println("No actions yet");
        forward();
        
      }
    if(lineCount == 4){
        Serial.println("We made it chat");
        motorStop();
        //close gripper
        //forward;
            //if (leftSide && !leftMiddle && !rightMiddle) {
              //sharpLeft();
            //}
            //else if (leftMiddle && !rightMiddle) {
              //turnLeft();
            //}
            //else if (rightSide && !rightMiddle && !leftMiddle) {
              //sharpRight();
            //}
            //else if (rightMiddle && !leftMiddle) {
              //turnRight();
            //}
            //else {
              //forward();
            //}
      }
     if(lineCount == 5){
        motorStop();
        //release gripper
        //move backwards
        //victory dance :3
      }
  }
  
  
}

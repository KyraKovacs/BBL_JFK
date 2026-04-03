// Motor Pins
const int mA1 = 10;
const int mA2 = 9;
const int mB1 = 6;
const int mB2 = 5;

//Sensors
const int SENSOR_COUNT = 8;
const int sensors[SENSOR_COUNT] = {A0, A1, A2, A3, A4, A5, A6, A7};

bool sensorOnLine[SENSOR_COUNT];

//Threshold (from when it senses black, the value is 500 because even tho the white values are between 200-500, the black is all ~900, so no need for an array, I just put the max amount I saw)
const int threshold = 500;

//Different speeds for different types of turning, you can modify BASE_SPEED, but the TURN_SPEED is the highest it can go, I checked
int BASE_SPEED = 200;
int TURN_SPEED = 170;
int SHARP_TURN_SPEED = 90;

void setup() {

  //pin setups for the motors
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);

}


void setMotor(int leftMotor, int rightMotor) {

  //this limits motorspeed to between -255 and 255, even tho the robot never reverses, I wanted to add the minus in case we need it later on, if this isn't here and the robot goes over 255, the robot stops going
  //But also, going over 255 isn't something that can happen with this code, this is just in case we need it later on
  leftMotor  = constrain(leftMotor,  -255, 255);
  rightMotor = constrain(rightMotor, -255, 255);

//defines what motor should do what, cannot be changed to the same name because it gets a compile error in that case, also it cannot tell anymore which motor is which
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

//motor functions
void forward() {
  setMotor(BASE_SPEED, BASE_SPEED);
}

//one wheel turns off, if you put it on one wheel forward, one wheel backwards, it doesn't turn sharp enough
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

void stopMotors() {
  setMotor(0, 0);
}


void readSensors() {

  //Reads all the sensors and puts them in value as an array, then checks if it's under or over the treshold
  for (int i = 0; i < SENSOR_COUNT; i++) {

    int value = analogRead(sensors[i]);

    sensorOnLine[i] = (value >= threshold);
  }
}

void loop() {

  readSensors();

  //defines what side is what, and on which sensor is which
  bool leftSide = sensorOnLine[0] || sensorOnLine[1]; //this could potentially be "and" instead of "or" because of line wideness, but it makes it kinda janky, so keep it on or
  bool leftMiddle = sensorOnLine[2] || sensorOnLine[3];
  bool rightMiddle = sensorOnLine[4] || sensorOnLine[5];
  bool rightSide = sensorOnLine[6] || sensorOnLine[7];
  
  bool straightLine = leftMiddle || rightMiddle;

//! means opposite, so if no line is active
  if (!straightLine) {
    stopMotors(); //this can be changed for it finding the line again, it was just easier for testing
    return; //you need return here so it isn't infinitely stuck on the motor being stopped when it looses the line
  }

//checks if the black line is only on the left side, left out rightSide because in case of turning it isn't really possible for the line to also be on the right when turning left
  if (leftSide && !leftMiddle && !rightMiddle) {
    sharpLeft();
  }
  else if (leftMiddle && !rightMiddle) {
    turnLeft();
  }
  else if (rightSide && !rightMiddle && !leftMiddle) {
    sharpRight();
  }
  else if (rightMiddle && !leftMiddle) {
    turnRight();
  }
  else {
    forward();
  }
}

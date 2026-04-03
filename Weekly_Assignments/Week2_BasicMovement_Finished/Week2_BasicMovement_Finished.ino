// Motor A pins
#define mA1 7
#define mA2 6

// Motor B pins
#define mB1 5
#define mB2 4

#define speedAForward 200
#define speedBForward 210
#define speedABackward 130
#define speedBBackward 255

#define TURN_TIME 750

//#define trigPin 2;
//#define echoPin 3;
//long duration;
//int distance;

void setup() {
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);
  //pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  //pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void moveForward() {
  // Move forward
  analogWrite(mA1, 0);
  analogWrite(mA2, speedAForward);

  analogWrite(mB1, speedBForward);
  analogWrite(mB2, 0); 
  }
  
void moveBackward() {

  // Start Motor B first
  analogWrite(mB1, 0);
  analogWrite(mB2, speedBBackward*4);


  // Start Motor A
  analogWrite(mA1, speedABackward );
  analogWrite(mA2, 0);

}

void turnRight(int turnTime){
  analogWrite(mA1, 0);
  analogWrite(mA2, speedAForward);

  analogWrite(mB1, speedBForward / 2);
  analogWrite(mB2, 0);

  delay(turnTime*2);
  }

void turnLeft(int turnTime){
  analogWrite(mA1, speedAForward);
  analogWrite(mA2, 0);

  analogWrite(mB1, 0);
  analogWrite(mB2, speedBForward / 2);

  delay(turnTime/2);
  }

void stopMotors(){ 
  //stop
  analogWrite(mA1, 0);
  analogWrite(mA2, 0);

  analogWrite(mB1, 0);
  analogWrite(mB2, 0); 
  }
/*int getDistance(){
  // Clears the trigPin
  analogWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  analogWrite(trigPin, HIGH);
  delayMicroseconds(10);
  analogWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
 
  return distance;
}*/

void loop() {
  moveForward();
  delay(3000);

  stopMotors();
  delay(1000);

  moveBackward();
  delay(3000);

  stopMotors();
  delay(1000);
  
  turnRight(TURN_TIME);
  delay(450);

  stopMotors();
  delay(1000);

  turnLeft(TURN_TIME);
  delay(450);

  stopMotors();
  delay(1000);*/
}

//Locator pins
#define trigPin 8
#define echoPin 7

//Speed definition for even speeds, one motor is weaker than the other 
#define speedAForward 200
#define speedBForward 240

//turn time
#define TURN_TIME 500

// Motor A pins
int mA1 = 10;
int mA2 = 9;

// Motor B pins
int mB1 = 6;
int mB2 = 5;

//defines duration and distance used later on for the object detection
long duration;
int distance;

void setup() {

  //Locator setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Motor setup
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);
  Serial.begin(9600);
}

void moveForward() {
  
  analogWrite(mA1, 0);
  analogWrite(mA2, speedAForward);

  analogWrite(mB1, speedBForward);
  analogWrite(mB2, 0); 
  }

void motorStop(){
  
  analogWrite(mA1, 0);
  analogWrite(mA2, 0);

  analogWrite(mB1, 0);
  analogWrite(mB2, 0); 
}

void turnLeft() {

  analogWrite(mA1, 0);
  analogWrite(mA2, speedAForward);

  analogWrite(mB1, 0);
  analogWrite(mB2, speedBForward); //adjustment for one motor being weaker than the other

  delay(TURN_TIME); //also adjustment for that
}

void turnRight() {
  analogWrite(mA1, speedAForward);
  analogWrite(mA2, 0);

  analogWrite(mB1, speedBForward);
  analogWrite(mB2, 0);

  delay(TURN_TIME);
}

void measureDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}

void loop() {
  measureDistance();
    delay(50);
  
    if (distance > 0 && distance <= 10) {
      Serial.println("Object Detected"); //this is just for debugging so you can see it detected the object
      motorStop();
      delay (100);
      
      turnLeft();
      
      moveForward();
      delay(900); 

      turnRight();

      moveForward();
      delay(900); 
      
      turnRight();

      moveForward();
      delay(200);

      turnLeft();

      moveForward();
      delay(500);
    } 
    else {
      moveForward();
    }
}

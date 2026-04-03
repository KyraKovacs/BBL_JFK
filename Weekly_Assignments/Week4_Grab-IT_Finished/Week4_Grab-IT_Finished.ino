/*Pick up the cone and keep the servo active by using millis() to update, open the gripper close it after a 
second en open it again after a second without having mechanical limits, drive 25 cm forward to the cone, 
grab it en drive another 25cm and keep the gripper active !!!*/

#define SERVO 12
#define gripperOpen 1600
#define gripperClose 1000

// Motor A pins
#define mA1 10
#define mA2 9

// Motor B pins
#define mB1 6
#define mB2 5

#define speedAForward 200
#define speedBForward 210
#define speedABackward 130
#define speedBBackward 255

//make methods for gripper open/close 

void setup(){
  pinMode(mA1, OUTPUT);
  pinMode(mA2, OUTPUT);
  pinMode(mB1, OUTPUT);
  pinMode(mB2, OUTPUT);
  pinMode(SERVO, OUTPUT);
  digitalWrite(SERVO, 0);
  }

void moveForward(){
  // Move forward
  analogWrite(mA1, 0);
  analogWrite(mA2, speedAForward);

  analogWrite(mB1, speedBForward);
  analogWrite(mB2, 0); 
  }
  
void stopMotors(){ 
  //stop
  analogWrite(mA1, 0);
  analogWrite(mA2, 0);

  analogWrite(mB1, 0);
  analogWrite(mB2, 0); 
  }

void openGripper(){
  for (int i = 0; i < 1000; i++) {
    gripper(gripperOpen);
    delay(1);
    }
  }

void closeGripper(){
  for (int i = 0; i < 1000; i++) {
    gripper(gripperClose);
    delay(1);
    }
  }

void loop() {
  openGripper();

  closeGripper();

  openGripper();
  
  moveForward();
  delay(1200);
  
  stopMotors();
  
  closeGripper();
  
  moveForward();
  delay(1200);
  
  stopMotors();
  
   while (true) {
    gripper(gripperClose);
  }
}

void gripper(int newPulse){
static unsigned long timer; //static will ensure that the timer is kept alive at all times in this function
static int pulse;
  if (millis() > timer){
    if (newPulse > 0){
      pulse = newPulse;
    }
    digitalWrite(SERVO, 1);
    delayMicroseconds(pulse);
    digitalWrite(SERVO, 0);
    timer = millis()+20;
  }
}

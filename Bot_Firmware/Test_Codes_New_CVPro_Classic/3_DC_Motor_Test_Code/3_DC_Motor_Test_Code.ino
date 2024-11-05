////////////--DC Motor Test Code--//////////////
const int motorPin1 = 32; 
const int motorPin2 = 33; 

const int nslp = 13; 
const int frequency = 5000;

// DC Motor
void motor_forward(int speed) // Speed Value (0 - 255)
{ 
  ledcWrite(5, speed);
  ledcWrite(6, 0);
  Serial.println("motor_forward");
}

void motor_backward(int speed) // Speed Value (0 - 255)
{ 
  ledcWrite(5, 0);
  ledcWrite(6, speed);
  Serial.println("motor_backward");
}

void motor_stop() {
  ledcWrite(5, 0);
  ledcWrite(6, 0);
  Serial.println("motor_stop");
}

void setup() {

  //######### DC Motor Setup ###########//
  ledcSetup(5, frequency, 8);
  ledcSetup(6, frequency, 8);
  ledcAttachPin(motorPin1, 5);
  ledcAttachPin(motorPin2, 6);
  pinMode(nslp, OUTPUT);
  digitalWrite(nslp, HIGH);
}

void loop() {

  motor_forward(240);   // Forward function
  delay(2000);          // Delay for 2 seconds.
  motor_backward(240);  // Backward function
  delay(2000);          // Delay for 2 seconds.
  motor_stop();         // Stop function
  delay(2000);          // Delay for 2 seconds.

}
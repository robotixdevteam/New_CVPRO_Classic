// Kindly Check whether the Library Files are installed in Library Manager.
#include <ESP32Servo.h>
#define SERVO_PIN 27 

Servo servo;

// Set the bot servo angles
int left = 75;
int center = 100;
int right = 125;


void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
}

void moveServoTo(int angle) {
  angle = constrain(angle, 75, 125); // Constrain the angle between 70 and 130 degrees
  servo.write(angle);               // Move the servo to the specified angle
  // delay(10);                       // Delay for servo to reach the position
  //Serial.println("Servo Angle : "+String(angle));               
}


void Sample_Servo_Test()
{
  // Simple Servo Test : 
  moveServoTo(left);    // Servo Left turn.
  delay(1000);          // Delay for 1 second.
  moveServoTo(center);  // Servo Center turn.
  delay(1000);          // Delay for 1 second.
  moveServoTo(right);   // Servo Right turn.
  delay(1000);          // Delay for 1 second.
  moveServoTo(center);  // Servo Center turn.
  delay(1000);          // Delay for 1 second.
}


void loop() {
  // To Set Servo Center
  //moveServoTo(100);

  Sample_Servo_Test();

}
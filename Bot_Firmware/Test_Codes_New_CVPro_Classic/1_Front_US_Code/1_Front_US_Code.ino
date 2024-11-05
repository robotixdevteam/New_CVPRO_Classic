#include <NewPing.h>

#define FRONT_TRIGGER 12 
#define FRONT_ECHO  4  
#define MAX_DISTANCE 400

int front_us;

NewPing sonar1(FRONT_TRIGGER, FRONT_ECHO, MAX_DISTANCE); 

void setup() {
  Serial.begin(115200);
}

void loop() {
  US_Values();
}

void US_Values()
{

  front_us = sonar1.ping_cm();

  if(front_us != 0)
  {
    Serial.println("F_US : " + String(front_us)); 
  }
 

}

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <NewPing.h>
#include <FastLED.h>


// ########### Declerations ############################################################################################################ //
//---------MQTT Declerations--------//
// wifi name and password
const char *ssid = "new_cvpro"; 
const char *password = "123456789";
// IP address
const char *mqtt_broker = "192.168.4.2";
//const char *mqtt_broker = "broker.emqx.io"; 
const int mqtt_port = 1883;
const char *mqtt_username = ssid; //"cvpro";
const char *mqtt_password = ssid; //"cvpro";
const char *topic = "cvpro";
int zspeed, zangle;
char *pch;

WiFiClient espClient;
PubSubClient client(espClient);

float batv;

//------RGB Led------//
#define LED_PIN 15
#define NUM_LEDS 1

CRGB leds[NUM_LEDS];

// DC Motor
const int motorPin1 = 32; 
const int motorPin2 = 33; 

const int nslp = 13; 
const int frequency = 5000;

// Servo Motor
#define SERVO_PIN 27
Servo servo;

// Ultrasonic Sensors
#define FRONT_TRIGGER 12 
#define FRONT_ECHO  4  

#define MAX_DISTANCE 200

NewPing sonar1(FRONT_TRIGGER, FRONT_ECHO, MAX_DISTANCE); 

int front_us;

//#---Servo Angles---####################################################################
int servo_center = 95;//deg

// ########### Functions ############################################################################################################ //
// RGB Led Function
void rgb_led(int r, int g, int b)
{
  leds[0] = CRGB(r, g, b);
  FastLED.show();
}

// DC Motor Functions
void motor_forward(int speed) {
  ledcWrite(5, speed);
  ledcWrite(6, 0);
  //Serial.println("motor_forward");
}

void motor_backward(int speed) {
  ledcWrite(5, 0);
  ledcWrite(6, speed*(-1));
  //Serial.println("motor_backward");
}

void motor_stop() {
  ledcWrite(5, 0);
  ledcWrite(6, 0);
  //Serial.println("motor_stop");
}


// Servo Functions
void moveServoTo(int angle) 
{
  angle = constrain(angle, 75, 125);
  servo.write(angle);
}


void bot_shutdown()
{
  motor_stop();
  moveServoTo(servo_center);
  //rgb_led(0, 0, 0);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  payload[length] = '\0';
  char *msg = (char *)payload;
  Serial.println("message");
  Serial.println(msg);
  int n = sscanf(msg,"%d,%d",&zangle,&zspeed); // Receiving End.

  movement(zangle, zspeed);
}


void movement(int xangle, int xspeed)
{
  if(xspeed >= 0)
  {
    if((xspeed != 0) && (xangle != 0))
    {
      motor_forward(xspeed);
      delay(1);
      moveServoTo(xangle);
      delay(5);
      Serial.println("xspeed : "+String(xspeed)+" xangle : "+String(xangle));
    } 
    else if((xspeed == 0) || (xangle == 0))
    {
      motor_stop();
      delay(1);
      moveServoTo(servo_center);
      delay(5);
    } 
  }
  else if(xspeed < 0)
  {
    motor_backward(xspeed);
    delay(1);
    moveServoTo(xangle);
    delay(5);
    Serial.println("xspeed : "+String(xspeed)+" xangle : "+String(xangle));
  }

}

void wifi_setup()
{
  //######### WiFi Setup #########//
  // Hotspot
  WiFi.softAP(ssid, password);       // Configure the ESP32 as an access point
  IPAddress myIP = WiFi.softAPIP();  // Get the IP address of the ESP32 access point
  Serial.print("Access point started with SSID ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(myIP);
  
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) 
  {
    String client_id = "New_CVPRO_Bot_Client-";
    client_id += String(WiFi.macAddress());
    // Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) 
    {
      Serial.println("Public emqx mqtt broker connected");
      // rgb_led(0, 0, 0);
      // delay(10);
      rgb_led(0, 255, 0);
    }
    else 
    {
      // Serial.println("failed with state ");
      // Serial.print(client.state());
      rgb_led(255, 0, 0);
      bot_shutdown();
    }
  }

  client.subscribe(topic);

}

// ########### Setup ############################################################################################################ //
void setup() {
  Serial.begin(115200);

  //######### RGB Led Setup #########//
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  //######### DC Motor Setup ###########//
  ledcSetup(5, frequency, 8);
  ledcSetup(6, frequency, 8);
  ledcAttachPin(motorPin1, 5);
  ledcAttachPin(motorPin2, 6);
  pinMode(nslp, OUTPUT);
  digitalWrite(nslp, HIGH);

  //######### Servo Motor Setup ###########//

 //servo.attach(SERVO_PIN, 500, 2400);
  //initial servo angle
  moveServoTo(servo_center);
  delay(10); 
  
  //######### WiFi Setup #########//
  // Hotspot
  wifi_setup();
}

void loop() {

  front_us = sonar1.ping_cm();
  
  if((front_us < 20) && (front_us != 0))
  {
    //Serial.println("F_US : " + String(front_us));
    bot_shutdown(); 
  }
  
  client.loop();  // Maintain the MQTT connection 

}


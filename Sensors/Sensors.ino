#include <Wire.h>

//pins
int airPin;
int motionPin;
int infraredPin;

//variables
int air_quality;
int motion_sensor;
int infrared;

//wire variables
#define SENSOR_ADDR 1
int answerSize = 2;
byte flag; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Wire
  Wire.begin(SENSOR_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(updateFlag);
}

void loop() {
  // put your main code here, to run repeatedly:
  air_quality = analogRead(airPin);
  motion_sensor = analogRead(motionPin);
  infrared = analogRead(infraredPin);
}

void updateFlag() {
  while (0 < Wire.available()) {
    flag = Wire.read();
  }
}

void requestEvent() {
  if (flag == "air") { //if the controller requests air, mot, or inf
    Wire.write(air_quality);
  }

  if (flag == "mot") {
    Wire.write(motion_sensor);
  }

  if (flag == "inf") { 
    Wire.write(infrared);
  }

}

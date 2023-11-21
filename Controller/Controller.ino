#include <Wire.h>

//pins-maybe change to constants
int jsXPin;
int jsYPin;
int jsButton;

//variables
int xReading; //needs mapping 0-1023
int yReading;
int jsButton; //high or low

int l_w; //left and right wheel speeds
int r_w;

int air_quality; //analogue 0-1023
int motion_sensor; //high or low
int infrared; //analogue

// wire variables
#define CAR_ADDR 9
#define SENSOR_ADDR 9

int sensorAnswerSize = 2;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  //add joystick readings

  //Wire
  
  writeToLeftWheel();
  writeToRightWheel();

  //maybe needs checks to make sure function has been completed
  requestAirQuality();
  requestMotion();
  requestInfrared();

}


void requestAirQuality() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write("air");
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR 9, sensorAnswerSize);
  while (Wire.available()) {
    air_quality = Wire.read();
  }
}

void requestMotion() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write("mot");
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR 9, sensorAnswerSize);
  while (Wire.available()) {
    motion_sensor = Wire.read();
  }
}

void requestInfrared() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write("inf");
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR 9, sensorAnswerSize);
  while (Wire.available()) {
    infrared = Wire.read();
  }
}

//first data sent is the left wheel
void writeToLeftWheel() {
  Wire.beginTransmission(CAR_ADDR);
  Wire.write(l_w);
  Wire.endTransmission();
}

void writeToRightWheel() {
  Wire.beginTransmission(CAR_ADDR);
  Wire.write(r_w);
  Wire.endTransmission();
}

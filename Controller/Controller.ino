#include <Wire.h>

//pins-maybe change to constants
int jsXPin = A1;
int jsYPin = A0;
int jsButtonPin = 13;

//variables
int xR; //needs mapping 0-1023
int yR;
int jsButton; //high or low

int l_w; //left and right wheel speeds
int r_w;

int dust_reading;
int humidity_reading;
int temp_reading;
bool motion_reading;
int distance_reading;
int bearing_reading;

// wire variables
#define CAR_ADDR 0
#define SENSOR_ADDR 9

//request codes
const int dust = 0;
const int humidity = 1;
const int temperature = 2;
const int motion = 3;
const int distance = 4;
const int bearing = 5;

int sensorAnswerSize = 1;


void setup() {
  // put your setup code here, to run once:
  pinMode(jsXPin, INPUT);
  pinMode(jsYPin, INPUT);
  pinMode(jsButton, INPUT);

  Serial.begin(9600);
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  //add joystick readings
  xR = map(1023 - analogRead(jsXPin), 0, 1023, 0, 255); 
  yR = map(analogRead(jsYPin), 0, 1023, 0, 255);

  Serial.print(xR);
  Serial.print(", ");
  Serial.print(yR);
  Serial.println();
  //Wire

  writeToLeftWheel();
  writeToRightWheel();

  requestDust();
  requestHumidity();
  requestTemp();
  requestMotion();
  requestDistance();
  requestBearing();

}


void requestDust() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(dust);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
  dust_reading = Wire.read();
  }
}

void requestHumidity() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(humidity);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
  humidity_reading = Wire.read();
  }
}

void requestTemp() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(temp);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
  temp_reading = Wire.read();
  }
}

void requestMotion() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(motion);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
    motion_reading = Wire.read();
  }
}

void requestDistance() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(distance);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
    distance_reading = Wire.read();
  }
}

void requestBearing() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(bearing);
  Wire.endTransmission();

  Wire.requestFrom(SENSOR_ADDR, sensorAnswerSize);
  while (Wire.available()) {
  bearing_reading = Wire.read();
  }
}

//first data sent is the left wheel
void writeToLeftWheel() {
  Wire.beginTransmission(CAR_ADDR);
  Wire.write(yR);
  Wire.endTransmission();
}

void writeToRightWheel() {
  Wire.beginTransmission(CAR_ADDR);
  Wire.write(xR);
  Wire.endTransmission();
}

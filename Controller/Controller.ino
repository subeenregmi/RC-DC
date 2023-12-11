#include <Wire.h>
#include <LiquidCrystal.h>

//pins-maybe change to constants
int jsXPin = A1;
int jsYPin = A0;
int jsButtonPin = 8;

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

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

int lastButtonPressed = millis();
int lastLCDupdate = millis();
int currentDisplayStat = 0;

int sensorAnswerSize = 1;


void setup() {
  // put your setup code here, to run once:
  pinMode(jsXPin, INPUT);
  pinMode(jsYPin, INPUT);
  pinMode(jsButtonPin, INPUT_PULLUP);

  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  //add joystick readings
  xR = map(1023 - analogRead(jsXPin), 0, 1023, 0, 255); 
  yR = map(analogRead(jsYPin), 0, 1023, 0, 255);

  writeToLeftWheel();
  writeToRightWheel();

  Serial.println(digitalRead(jsButtonPin));
  
  if (digitalRead(jsButtonPin) == LOW){
    if (millis() - lastButtonPressed > 200){
      currentDisplayStat++;
      currentDisplayStat %= 5;
      lastButtonPressed = millis();
    }
  }
  if (millis() - lastLCDupdate > 50){
    handleDisplayStat();
    lastLCDupdate = millis();
  }
}

void handleDisplayStat(){
  switch (currentDisplayStat){
    case 0:
      requestDust();
      printStat("Dust conc.:", dust_reading);
    case 1:
      requestHumidity();
      printStat("Humidity:", humidity_reading);
    case 2:
      requestTemp();
      printStat("Temperature:", temp_reading);
    case 3:
      requestMotion();
      printStat("Motion Detected:", motion_reading);
    case 4:
      requestBearing();
      printStat("Bearing:", angleToBearing(bearing_reading));
  }
}

String angleToBearing(int angle){
  String b;
  switch (angle) {
    case 0:
      b = "W";   
      break;
    case 1 ... 89:
      b = "NW";   
      break;
    case 90:
      b = "N";
      break;
    case 91 ... 179:
      b = "NE";
      break;
    default:
      b = "E";
      break;
  }
  return b;
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
  Wire.write(temperature);
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
    motion_reading = (bool) Wire.read();
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

void printStat(String title, float value){
  lcd.setCursor(0, 0);
  lcd.print(title + ":");
  lcd.setCursor(0, 1);
  lcd.print(value);  
}

void printStat(String title, int value){
  lcd.setCursor(0, 0);
  lcd.print(title + ":");
  lcd.setCursor(0, 1);
  lcd.print(value);
}

void printStat(String title, String value){
  lcd.setCursor(0, 0);
  lcd.print(title + ":");
  lcd.setCursor(0, 1);
  lcd.print(value);
}

void printStat(String title, bool value){
  lcd.setCursor(0, 0);
  lcd.print(title + ":");
  lcd.setCursor(0, 1);
  if (value)
    lcd.print("Yes");
  else
    lcd.print("No");
}
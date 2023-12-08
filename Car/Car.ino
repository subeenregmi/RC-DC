#include <Wire.h>

//variables
int l_w;
int r_w;
int xR;
int yR;
int speed;

//pins
const int lwSpeedPin = 3;
const int lwInputPin1 = 4;
const int lwInputPin2 = 7;
const int rwSpeedPin = 9;
const int rwInputPin1 = 12;
const int rwInputPin2 = 13;

//wire variables
#define CAR_ADDR 0
int answerSize = 2; //change if speed is <255
bool side = false; //false is left, true is right


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(lwSpeedPin, OUTPUT);
  pinMode(rwSpeedPin, OUTPUT);
  pinMode(lwInputPin1, OUTPUT);
  pinMode(lwInputPin2, OUTPUT);
  pinMode(rwInputPin1, OUTPUT);
  pinMode(rwInputPin2, OUTPUT);

  //Wire
  Wire.begin(CAR_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(updateSpeed);

}

void loop() {
  // put your main code here, to run repeatedly:

  speed = map(yR, 0, 1023, -255, 255);
  if (xR > 912) { //to turn right
    l_w = speed;
    r_w = 0;
  }
  else if (xR < 112) {
    l_w = 0;
    r_w = speed;
  }
  else {
    l_w = speed;
    r_w = speed;
  }

  analogWrite(lwSpeedPin, abs(l_w));
  analogWrite(rwSpeedPin, abs(r_w));

  if (l_w > 20) { //add deadzones here
    digitalWrite(lwInputPin1, HIGH);
    digitalWrite(lwInputPin2, LOW);
  }
  else if (l_w < -20) {
    digitalWrite(lwInputPin1, LOW);
    digitalWrite(lwInputPin2, HIGH);
  }
  else {
    digitalWrite(lwInputPin1, LOW);
    digitalWrite(lwInputPin2, LOW);
  }

  if (r_w > 20) { //add deadzones here
    digitalWrite(rwInputPin1, HIGH);
    digitalWrite(rwInputPin2, LOW);
  }
  else if (r_w < -20) {
    digitalWrite(rwInputPin1, LOW);
    digitalWrite(rwInputPin2, HIGH);
  }
  else {
    digitalWrite(rwInputPin1, LOW);
    digitalWrite(rwInputPin2, LOW);
  }

}

void updateSpeed() {
  if (side) { //if True, write to right, then flip to left
    while (0 < Wire.available()) {
      yR = Wire.read();
    }
    side = !side;
  }
  else { //--''-- opposite
    while (0 < Wire.available()) {
      xR = Wire.read();
    }
    side = !side;
  }
}

void requestEvent() {

}

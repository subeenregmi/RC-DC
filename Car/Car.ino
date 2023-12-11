#include <Wire.h>

//variables
int l_w; //speeds to write to the wheels
int r_w;
int xR; //data received from the controller
int yR;
int wspeed;

int deadzone = 20; //readings within this from the centre will not turn the car

//pins
const int lwSpeedPin = 3;
const int lwInputPin1 = 4;
const int lwInputPin2 = 5;
const int rwSpeedPin = 10;
const int rwInputPin1 = 9;
const int rwInputPin2 = 8;

//wire variables
#define CAR_ADDR 0
int answerSize = 1; //change if speed is <255
bool side = true; //false is left, true is right


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

  wspeed = map(yR, 0, 255, -255, 255);
  //Serial.println(wspeed);
  if (xR > (128 + deadzone)) { //to turn right
    l_w = wspeed;
    r_w = 0;
  }
  else if (xR < (128 - deadzone)) {
    l_w = 0;
    r_w = wspeed;
  }
  else {
    l_w = wspeed;
    r_w = wspeed;
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

void updateSpeed(int x) {
  if (side) { //if True, write to right, then flip to left
    while (0 < Wire.available()) {
      yR = 255 -  Wire.read();
    }
    Serial.print(yR);
    Serial.print(", ");
    side = !side;
  }
  else { //--''-- opposite
    while (0 < Wire.available()) {
      xR = Wire.read();
    }
    Serial.println(xR);
    side = !side;
  }
}

void requestEvent() {

}

#include <Wire.h>

//variables
int l_w; //speeds to write to the wheels
int r_w;
int xR; //data received from the controller
int yR;
int baseSpeed; //speed calculated from yR

int deadzone = 20; //readings within this from the centre will not turn the car

//pins-Hbridge
const int lwSpeedPin = 3;
const int lwInputPin1 = 4;
const int lwInputPin2 = 5;
const int rwSpeedPin = 10;
const int rwInputPin1 = 9;
const int rwInputPin2 = 8;

//wire variables
#define CAR_ADDR 0
int answerSize = 1; 
bool side = true; //true is yR, false is xR


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

  baseSpeed = map(yR, 0, 255, -255, 255); //map the received reading to a speed and direction
  if (xR > (128 + deadzone)) { //to turn right
    l_w = baseSpeed;
    r_w = 0;
  }
  else if (xR < (128 - deadzone)) { //turn left
    l_w = 0;
    r_w = baseSpeed;
  }
  else { 
    l_w = baseSpeed;
    r_w = baseSpeed;
  }

  analogWrite(lwSpeedPin, abs(l_w)); //absolute value of baseSpeed to spin motors
  analogWrite(rwSpeedPin, abs(r_w));

  if (l_w > deadzone) { //H-bridge spin forward-left wheel
    digitalWrite(lwInputPin1, HIGH);
    digitalWrite(lwInputPin2, LOW);
  }
  else if (l_w < -deadzone) {//spin back
    digitalWrite(lwInputPin1, LOW);
    digitalWrite(lwInputPin2, HIGH);
  }
  else { 
    digitalWrite(lwInputPin1, LOW);
    digitalWrite(lwInputPin2, LOW);
  }

  if (r_w > deadzone) { //same for right wheel
    digitalWrite(rwInputPin1, HIGH);
    digitalWrite(rwInputPin2, LOW);
  }
  else if (r_w < -deadzone) {
    digitalWrite(rwInputPin1, LOW);
    digitalWrite(rwInputPin2, HIGH);
  }
  else {
    digitalWrite(rwInputPin1, LOW);
    digitalWrite(rwInputPin2, LOW);
  }

}

void updateSpeed(int x) { //when reading is received
  if (side) { //if True, write to yR, then flip to xR
    while (0 < Wire.available()) {
<<<<<<< HEAD
      yR = 255 -  Wire.read(); //flips the y reading-fixed the car moving backwards
=======
      yR = 255 -  Wire.read();
>>>>>>> f947e73094be6cbb70b521e987987c12271b34d1
    }
    side = !side;//next time write to xR
  }
  else { //--''-- opposite
    while (0 < Wire.available()) {
      xR = Wire.read();
    }
    Serial.println(xR);
    side = !side;
  }
}

void requestEvent() { //never requested from

}

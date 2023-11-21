#include <Wire.h>

//variables
int l_w;
int r_w;

//wire variables
#define CAR_ADDR 0
int answerSize = 2; //change if speed is <255
bool side = False; //false is left, true is right


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Wire
  Wire.begin(CAR_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(updateSpeed);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void updateSpeed() {
  if (side) { //if True, write to right, then flip to left
    while (0 < Wire.available()) {
      r_w = Wire.read();
    }
    side = !side;
  }
  else { //--''-- opposite
    while (0 < Wire.available()) {
      l_w = Wire.read();
    }
    side = !side;
  }
}

void requestEvent() {

}

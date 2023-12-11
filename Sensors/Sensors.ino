#include <Wire.h>
#include "DHT.h" // 
#include <HCSR04.h>
#include <Servo.h>

// car variables
bool carStopped = false;

// motion sensor variables
int motionSensor = 2;              
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin

// dust sensor variables
int dustSensor = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000; 
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

// combi-sensor/ temperature humidity sensor variables
#define DHTPIN 2
// The sensor is initialized
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// initialize ultrasonic sensor (trig pin , echo pin)
HCSR04 hc(9, 10);

// servo motor variable
Servo myservo;
int pos = 0;
String bearing = "NA";
String previousBearing = "NA" ;

// wire variables
#define SLAVE_ADDR 9
int answerSize = 1; // change to variable
byte x; // collect information from master
int mainData; // store the measured value of sensor here and write it to wire



void setup() {
  // initialize serial
  Serial.begin(9600);

  // initialize motion sensor
  pinMode(motionSensor, INPUT);

  // initialize dust sensor       
  pinMode(dustSensor, INPUT);
  starttime = millis();

  // initialize servo motor
  myservo.attach(7);
  myservo.write(0);

  // start measurement from combi-sensor/ temperature humidity sensor
  dht.begin();

  //Wire-run in slave mode 
  Wire.begin(SLAVE_ADDR);

  //on request, run requestEvent()
  Wire.onRequest(requestEvent);

  //on receive, run receiveEvent() 
  Wire.onReceive(receiveEvent);
}

void loop(){
 // program runs the sensor function and sends back output according to what value of x the master program has called
  if (x == 0) { 
    dustDetect();
  }
  else if (x == 1 or x == 2) {
    combiDetect(x);
  }
  else if (x == 3) {
    motionDetect();
  }
  else if (x == 4) {
    servoMove();
  }
}

// function for the dust sensor
void dustDetect() {

  // code referenced from the website
  duration = pulseIn(dustSensor, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = round(1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62); 
    mainData = map(concentration,0,28000,0,255); // mapping the value down to one byte for wire transmission

    //    Serial.print("Concentration: ");
    //    Serial.print(concentration);
    //    Serial.println(" pcs/0.01cf");
    //    Serial.println("\n");
      
    lowpulseoccupancy = 0;
    starttime = millis();
  }
}


// function for the combi-sensor (temperature & humidity)
void combiDetect(int val) {
  float reading;

  switch (val) {
      case 1:
         // Humidity is measured
         reading = dht.readHumidity(); // get humidity
         break;
      case 2:
         // temperature is measured
         reading = dht.readTemperature(); // get temerpature
         break;
      }
      
  mainData = round(reading); // round the reading since float is not one byte
//  Serial.print(mainData);
}
  

// fuction for the motion sensor
void motionDetect() {
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(motionSensor); // read sensor state 
  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {  // print motion detected if the sensor is now HIGH 
    mainData = 1;
   // Serial.println("Motion detected!");
  }
  else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {  // print motion stopped if the sensor is now LOW 
//    Serial.println("Motion stopped!");
    mainData = 0;
  }
}
  

void servoMove() {
  for (int p = 0; p <= 180; p++) { // servo moves to 180 degrees
    myservo.write(p);
    delay(5);
    distanceMeasure(p); // runs the ultrasonic sensor at every degree the servo tilts to
    }
      
  for (int p = 180; p >= 0; p--) { // returns to 0
    myservo.write(p);
    delay(5);
    distanceMeasure(p);
    }
}

String angleToBearing(int angle){ // each degree falls into the range of a bearing
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

// function for ultrasonic snesor
void distanceMeasure(int angle) {
  int nowDistance = hc.dist(); // get distance
  //Serial.println(nowDistance);
  
  if (nowDistance <= 30) { // if and only if detected object is within 30cm 
    String b = angleToBearing(angle);
    
    if (previousBearing != b) { // only write the angle to wire if the current bearing is different from the last `                                                                                                                                                                                                                                                                                                                                                                                     
      previousBearing = b;

      mainData = angle;      
    }
  }
}


//this constantly runs throughout the game 
void requestEvent() {
  Wire.write(mainData); // sends mainData to the master program
}


// this constantly runs throughout the game 
void receiveEvent() {
  //read while data is available 
  while (0 < Wire.available()) {
      x = Wire.read(); 
    }
}

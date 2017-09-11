#include "Arduino.h"

#include "SoftwareSerial.h" //Serial library
#include "Servo.h"

Servo servo;
SoftwareSerial bt(10,11);  //RX, TX (Switched on the Bluetooth - RX -> TX | TX -> RX)

const int servoPin = 9;
const int LEDPin = 13;

int lastAngle;
const int minAngle = 10;
const int maxAngle = 170;

void setup() {
  Serial.begin(38400);
  Serial.println("RS ready");

  bt.begin(38400); //Open the serial port
  bt.println("BT ready");

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  servo.attach(servoPin);
  turnServo("0");
}

void loop() {

  if (bt.available()) { //if serial is available
    turnServo(bt.readStringUntil('\n'));
  }

  if (Serial.available()) {
	  turnServo(Serial.readStringUntil('\n'));
  }

  delay (300); //prepare for data
  digitalWrite(LEDPin, LOW);
}

void turnServo(String req) {
	if (req == "") {return;}

	int percent = constrain(req.toInt(), 0, 100);
    int angle = map(percent, 0, 100, minAngle, maxAngle);
    if (angle == lastAngle) {return;}
    lastAngle = angle;

    servo.write(angle);

    bt.println("Req "+ req +" ("+ percent +"%) turn "+ angle);
    Serial.println("Req "+ req +" ("+ percent +"%) turn "+ angle);

    digitalWrite(LEDPin, HIGH); //if we get 1, turn on the LED
}

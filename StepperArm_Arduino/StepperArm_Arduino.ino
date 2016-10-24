/*
Name:		StepperArm_Arduino.ino
Created:	10/15/2016 11:48:24 AM
Author:	Paul


3 stepper
2 servo (or 3... variable end affector)
*/
#include <MultiStepper.h>
#include <AccelStepper.h>
#include <Servo.h>
#include <Stepper.h>
#include <math.h>

#define MOTOR_STEPS 400
#define STEP_DEGREE 360/MOTOR_STEPS //.9
#define STEP_RADIAN STEP_DEGREE*(PI/180) //0.0157079633
#define NUM_OF_MOVES 10
#define PENNY_HEIGHT 1 //EDIT IN CM

#define FLIP_HEIGHT 100
#define FLIP_EXTENSION 100
#define FLIP_ORIENT 100

bool pickedUp = false;

bool isRotated = false;

const byte baseStepperDir = 1;
const byte baseStepperStep = 2;
AccelStepper baseStepper(1, baseStepperStep, baseStepperDir);

const byte acmeStepperDir = 1;
const byte acmeStepperStep = 2;
AccelStepper acmeStepper(1, acmeStepperStep, acmeStepperDir);


const byte linearStepperDir = 1;
const byte linearStepperStep = 2;
AccelStepper linearStepper(1, linearStepperStep, linearStepperDir);


/*
TODO WORK ON END
*/

Servo rotateServo;
Servo grabServo;

const byte solenoidRelayPin = 1;

const byte verticalSwitch = 1;
const byte baseSwitch = 1;
const byte extensionSwitch = 1;
//RED == NOT CONNECTED; BLUE == CONNECTED; GREEN == RUNNING;
const byte stateRLed = 1;
const byte stateBLed = 1;
const byte stateGLed = 1;

int byteXYZIn;
int byteXYZOut;
//123 456 789
int thetaCoord;	//(in-in%1000000)/1000000
int rCoord;		//(in-in%1000)/1000
int zCoord;		//in%1000
bool isConnected = false;
bool isRunning = false;

void setup() {
	//Start Serial
	Serial.begin(9600);

	//Initialize Pins
	pinMode(solenoidRelayPin, OUTPUT);

	pinMode(verticalSwitch, INPUT);
	pinMode(baseSwitch, INPUT);
	pinMode(extensionSwitch, INPUT);

	//init state LEED
	pinMode(stateRLed, OUTPUT);
	pinMode(stateBLed, OUTPUT);
	pinMode(stateGLed, OUTPUT);

	//Set stateLED to not connected
	analogWrite(stateRLed, 255);
	analogWrite(stateBLed, 0);
	analogWrite(stateGLed, 0);

	//check to see if we are connected
	while (Serial.available() == 0);


	//calibrate and set states
	//dropCalibrate();
	isConnected = true;

	//set LED state to connected
	analogWrite(stateRLed, 0);
	analogWrite(stateBLed, 255);
	analogWrite(stateGLed, 0);

}

void loop() {
	if (Serial.readString() == "begin") {
		isRunning == true;
		analogWrite(stateRLed, 0);
		analogWrite(stateBLed, 0);
		analogWrite(stateGLed, 255);
		dropCalibrate();
		while (isRunning) {
			byteXYZIn = Serial.read();
			if (byteXYZIn == 0x00)
				break;
			thetaCoord = (byteXYZIn - (byteXYZIn % 1000)) / 1000;
			rCoord = byteXYZIn % 1000;
			zCoord = byteXYZIn % 1000;
			acmeStepper.moveTo(PENNY_HEIGHT);
			baseStepper.moveTo(thetaCoord);
			linearStepper.moveTo(rCoord);
			do {
				acmeStepper.moveTo(PENNY_HEIGHT);
				baseStepper.moveTo(thetaCoord);
				linearStepper.moveTo(rCoord);
			} while (baseStepper.currentPosition() != thetaCoord && linearStepper.currentPosition() != rCoord && acmeStepper.currentPosition() != zCoord);
			
			if (pickedUp) {
				acmeStepper.moveTo(2 * PENNY_HEIGHT);
				rotate();
				acmeStepper.moveTo(0);
				endAffect();
				acmeStepper.moveTo(PENNY_HEIGHT);
			}
			else {
				endAffect();
				acmeStepper.moveTo(0);
				endAffect();
				acmeStepper.moveTo(PENNY_HEIGHT);
			}
		}
		acmeStepper.moveTo(FLIP_HEIGHT);
		linearStepper.moveTo(FLIP_EXTENSION);
		baseStepper.moveTo(FLIP_ORIENT);
		//TODO FLIP MOTOR & SOLENOID
	}
}

void rotate() {
	if (isRotated) {
		rotateServo.write(180);
	}
	else {
		rotateServo.write(0);
	}
	isRotated = !isRotated;

}

void endAffect() {

}

void stepperInit() {
	baseStepper.setMaxSpeed(3000);
	acmeStepper.setMaxSpeed(3000);
	linearStepper.setMaxSpeed(3000);

	baseStepper.setAcceleration(1000);
	acmeStepper.setAcceleration(1000);
	linearStepper.setAcceleration(1000);
}

void dropCalibrate() {//should end pulled in, completely down, and all the way to the left
	digitalWrite(solenoidRelayPin, HIGH);
	delay(3);
	digitalWrite(solenoidRelayPin, LOW);
	delay(2);
	calibrate();
}
//TODO WHEN ASSEMBLED -- SIMPLE CALB
void calibrate() {
	while (!digitalRead(verticalSwitch)) {
		acmeStepper.run();
	}
	acmeStepper.stop();
	acmeStepper.setCurrentPosition(0);

	while (!digitalRead(extensionSwitch)) {
		linearStepper.run();
	}
	linearStepper.stop();
	linearStepper.setCurrentPosition(0);

	while (!digitalRead(baseSwitch)) {
		baseStepper.run();
	}
	baseStepper.stop();
	baseStepper.setCurrentPosition(0);
}

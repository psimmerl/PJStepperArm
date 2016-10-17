/*
 Name:		StepperArm_Arduino.ino
 Created:	10/15/2016 11:48:24 AM
 Author:	Paul


 3 stepper 
 2 servo (or 3... variable end affector)
*/
#include "AccelStepper\AccelStepper.h"
#include <Servo.h>
#include <Stepper.h>
#include <math.h>

#define MOTOR_STEPS 400
#define STEP_DEGREE 360/MOTOR_STEPS //.9
#define STEP_RADIAN STEP_DEGREE*(PI/180) //0.0157079633

const byte baseStepperDir	= 1;
const byte baseStepperStep	= 2;
AccelStepper baseStepper(1, baseStepperStep, baseStepperDir);

const byte acmeStepperDir	= 1;
const byte acmeStepperStep	= 2;
AccelStepper acmeStepper(1, acmeStepperStep, acmeStepperDir);


const byte linearStepperDir	= 1;
const byte linearStepperStep = 2;
AccelStepper linearStepper(1, linearStepperStep, linearStepperDir);


/*
TODO WORK ON END
*/

const byte solenoidRelayPin = 1;

const byte verticalSwitch = 1;
const byte baseSwitch = 1;
const byte extensionSwitch = 1;

int byteXYIn;
int byteXYOut;

int xCoord;//(in-in%1000)/1000
int yCoord;//in%1000


void setup() {
	Serial.begin(9600);
	pinMode(solenoidRelayPin, OUTPUT);

}

void loop() {
  
}

void stepperInit() {
	baseStepper.setMaxSpeed(3000);
	acmeStepper.setMaxSpeed(3000);
	linearStepper.setMaxSpeed(3000);

	baseStepper.setAcceleration(1000);
	acmeStepper.setAcceleration(1000);
	linearStepper.setAcceleration(1000);
}

void dropCalibrate() {
	digitalWrite(solenoidRelayPin, HIGH);
	delay(3);
	
}

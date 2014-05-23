
#include <math.h>
#include "MotorCommand.h"
#include "VirtualMotor.h"

#define CCW_PIN 11
#define CW_PIN 12

void doTest();
void runAssay();

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  Serial.println("Motor Command Driver");
  Serial.print("Command size:  "); Serial.println(sizeof(Command));
  Serial.print("Command* size: "); Serial.println(sizeof(Command*));
  Serial.print("Int size:      "); Serial.println(sizeof(int));
  
  runAssay();
}

void loop() {
}

// A function with continuous 1st derivative
// in the domain (0,1), with range (0,1)
double smoothMapping(double x) {
  return 0.5*(1 - cos(x));
}

// delayInterval in microseconds
void doSteps(int pin, int steps, long delayInterval) {
  for (int i = 0; i < steps; ++i) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(delayInterval);
    digitalWrite(pin, LOW);
    delayMicroseconds(delayInterval);
  }
}

// The factor f is choosen such that for a given number
// of sections n, 
// 
//   sum(f * 1/n * sin(pi * ((0.5/n):(1/n):(1-0.5/n)))) == 1.000
//
// This is to ensure that the smooth and linear
// version both have the same duration for
// a given number of steps.
const double integralFactor = 1.5643;

double continuousDerivativeAdjustment(double in) {
  return integralFactor * sin(PI * in);
}

void doSmoothSteps(int pin, int steps, long delayInterval) {
  int section = 10;
  if (steps <= section) {
    doSteps(pin, steps, delayInterval);
    return;
  }
  
  int sectionSteps = steps / section;
  int remainder = steps % section;
  
  for (int i = 0; i < section; ++i) {
    if (remainder == 0) {
      doSteps(pin, sectionSteps, delayInterval * continuousDerivativeAdjustment(double(i)/section));
    } else {
      doSteps(pin, sectionSteps + 1, delayInterval * continuousDerivativeAdjustment(double(i)/section));
      remainder -= 1;
    }
  }
  
}

void doCommand(CmdPtr c) {
  switch(c->dir) {
    case DIR_STATIONARY:
      delay(c->steps*1000);
      break;
    case DIR_CW:
      doSteps(CW_PIN, c->steps, 1000000.0/(c->motorSpeed)/2);
      break;
    case DIR_CCW:
      doSteps(CCW_PIN, c->steps, 1000000.0/(c->motorSpeed)/2);
      break;
  }
}

void printSingleCommand(CmdPtr c) {
  Serial.print("Command(");
  Serial.print(c->motorSpeed);
  Serial.print(", ");
  Serial.print(c->steps);
  Serial.print(", ");
  Serial.print(c->dir);
  Serial.print(", ");
  Serial.print(c->flag);
  Serial.println(")");
}

void printCommand(CmdPtr c) {
  traverseCommands(c, &printSingleCommand);
}

void simpleTest() {
  CmdPtr c = fastCW(50);
  printCommand(c);
  dispose(c);  
}

void combineTest() {
  CmdPtr c = combine(fastCW(50), combine(slowCCW(40), stationary(5)));
  printCommand(c);
  dispose(c);  
}

void repeatTest() {
  CmdPtr c = repeat(5, combine(fastCW(300), slowCCW(150)));
  runVirtualMotor(1000, c);
  dispose(c);
}

void timeRepeatTest() {
  CmdPtr c = repeatForSeconds(58, combine(fastCW(300), slowCCW(150)));
  runVirtualMotor(1000, c);
  dispose(c);
}

void doTest() {
  timeRepeatTest();
}

void runAssay() {
  CmdPtr jerkAway =
    combine(fastSmoothCW(333 + 1000), 
            stationary(5));
  
  CmdPtr windshieldWiper =
    repeatForSeconds(360,
      combine(fastCCW(1000),
      combine(stationary(5),
      combine(fastCW(1000),
              stationary(5)))));
  
  CmdPtr toNext = 
    combine(slowCW(500 + 167),
            stationary(5));
  
  CmdPtr cellCycle =
    combine(jerkAway,
    combine(windshieldWiper,
            toNext));
  
  runVirtualMotor(167, cellCycle);
  dispose(cellCycle);
}


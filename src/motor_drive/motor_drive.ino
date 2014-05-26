
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
  
  doTest();
}

void loop() {
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

void smoothTest() {
  CmdPtr c1 = combine(slowSmoothCW(DEG_180), slowSmoothCCW(DEG_180));
  runVirtualMotor(0, repeat(5, c1));
  dispose(c1);
}

void doTest() {
  smoothTest();
}

void runAssay() {
  CmdPtr jerkAway =
    combine(fastSmoothCW(DEG_60 + DEG_180), 
            stationary(5));
  
  CmdPtr windshieldWiper =
    repeatForSeconds(360,
      combine(fastCCW(DEG_180),
      combine(stationary(5),
      combine(fastCW(DEG_180),
              stationary(5)))));
  
  CmdPtr toNext = 
    combine(slowCW(DEG_90 + DEG_30),
            stationary(5));
  
  CmdPtr cellCycle =
    combine(jerkAway,
    combine(windshieldWiper,
            toNext));
  
  runVirtualMotor(167, cellCycle);
  dispose(cellCycle);
}


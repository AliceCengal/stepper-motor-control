
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

void doSteps(int pin, int steps, int delayInterval) {
  for (int i = 0; i < steps; ++i) {
    digitalWrite(pin, HIGH);
    delay(delayInterval);
    digitalWrite(pin, LOW);
    delay(delayInterval);
  }
}
  
void doCommand(CmdPtr c) {
  int steps = c->steps;
  int pin = (c->dir == DIR_CW) ? CW_PIN : CCW_PIN;
  doSteps(pin, steps, (c->motorSpeed)/2);
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


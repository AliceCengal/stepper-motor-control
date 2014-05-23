
#include "MotorCommand.h"

#define CCW_PIN 11
#define CW_PIN 12

void doTest();

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  Serial.println(42);
  doTest();
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
  CmdPtr c = repeat(5, combine(fastCW(50), slowCCW(30)));
  printCommand(c);
  dispose(c);
}

void doTest() {
  repeatTest();
}

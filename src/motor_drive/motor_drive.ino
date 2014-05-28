
#include <math.h>
#include "MotorCommand.h"
#include "CellStruct.h"

#define CCW_PIN 11
#define CW_PIN 12
#define ASSAY_PIN 7
#define CRAWL_PIN 6

bool assaySwitch = false;
bool crawlSwitch = false;
CmdPtr crawlCmd = slowCCW(2);

void setup() {
  Serial.begin(9600);
  pinMode(CCW_PIN, OUTPUT);
  pinMode(CW_PIN, OUTPUT);
  pinMode(ASSAY_PIN, INPUT);
  pinMode(CRAWL_PIN, INPUT);
}

void loop() {
  assaySwitch = digitalRead(ASSAY_PIN) == HIGH;
  crawlSwitch = digitalRead(CRAWL_PIN) == HIGH;
  
  if (assaySwitch) {
    runAssay();
  } else if (crawlSwitch) {
    runMotor(crawlCmd);
  }
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

void runMotor(CmdPtr c) {
  traverseCommands(c, &doCommand);
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
  CmdPtr c = fastCW(DEG_180);
  runMotor(c);
  dispose(c);  
}

void combineTest() {
  CmdPtr c = combine(fastCW(DEG_180), combine(stationary(5), slowCCW(DEG_90)));
  runMotor(c);
  dispose(c);  
}

void repeatTest() {
  CmdPtr c = repeat(5, combine(fastCW(DEG_90), slowCCW(DEG_45)));
  runMotor(c);
  dispose(c);
}

void timeRepeatTest() {
  CmdPtr c = repeatForSeconds(58, combine(fastCW(DEG_90), slowCCW(DEG_45)));
  runMotor(c);
  dispose(c);
}

void smoothTest() {
  CmdPtr c1 = combine(fastSmoothCW(DEG_180), stationary(3));
  runMotor(repeat(5, c1));
  dispose(c1);
}

void doTest() {
  smoothTest();
}

// create a cell cycle command. wiperDuration in seconds
CmdPtr makeCellCycle(int wiperDuration) {
  CmdPtr jerkAway =
    combine(fastSmoothCCW(DEG_30 + DEG_180), 
            stationary(5));
  
  CmdPtr windshieldWiper =
    repeatForSeconds(wiperDuration,
      combine(fastCW(DEG_180),
      combine(stationary(5),
      combine(fastCCW(DEG_180),
              stationary(5)))));
  
  CmdPtr toNext = 
    combine(slowCCW(DEG_90 + DEG_60),
            stationary(5));
  
  CmdPtr cellCycle =
    combine(jerkAway,
    combine(windshieldWiper,
            toNext));
  
  return cellCycle;
}

CmdPtr makeTransition(int offset) {
  return combine(slowCCW(offset), stationary(5));
}

void runAssay() {
  int assayLength = 4;
  Cell assay[] = {
    makeCell(  0, 100, 18),
    makeCell(140,  84, 18),
    makeCell(292,  92, 18),
    makeCell(440,  92, 60)
  };
  
  for (int i = 0; i < assayLength; ++i) {
    CmdPtr cycle = makeCellCycle(assay[i].duration);
    runMotor(combine(cycle, makeTransition(assay[i].length)));
    dispose(cycle);
  }
  
}


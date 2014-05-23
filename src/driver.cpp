
#include <iostream>
#include "MotorCommand.h"
#include "VirtualMotor.h"
using namespace std;

void printCommand(Command* c) {
  cout << "Command("
       << "motor speed:"  << c->motorSpeed << ","
       << "steps:"        << c->steps      << ","
       << "direction:"    << c->dir        << ","
       << "flag:"         << c->flag       << ")" << endl;
}

void simplePrintCommand(Command* c) {
  cout << "Command" << endl; 
}

void countCommandChain(Command* c) {
  traverseCommands(c, &printCommand);
}

void testRepeatForSeconds() {
  Command* c = repeatForSeconds(100, combine(fastCW(40), fastCCW(40)));
  runVirtualMotor(1000, c);
  dispose(c);
}

void simpleTest() {
  Command* c = combine(fastCW(90), combine(slowCCW(90), stationary(5)));
  runVirtualMotor(1000, c);
  dispose(c);
}

void testWraparound() {
  Command* c = repeat(30, fastCCW(50));
  runVirtualMotor(1000, c);
  dispose(c);
}

int main() {
  cout << "Motor Command Driver" << endl;
  
  testRepeatForSeconds();
  
  return 0;
}


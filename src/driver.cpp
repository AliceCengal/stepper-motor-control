
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

int main() {
  
  cout << "Motor Command Driver" << endl;
  
  Command* c = combine(fastCW(90), combine(slowCCW(90), stationary(5)));
  
  runVirtualMotor(1000, c);
  
  dispose(c);
  return 0;
}


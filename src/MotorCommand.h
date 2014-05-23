//
// Data structures and operations for defining the operation
// of a stepper motor. `Command` is the primitive data structure
// which can be composed together to form complex instructions.
//

#ifndef NULL
#define NULL 0
#endif

#ifndef MOTOR_COMMAND_H
#define MOTOR_COMMAND_H

const double PI        = 3.141592653589793;
const double STEPS_REV = 2000;
const double DEG_STEP  = 360.0 / STEPS_REV;

const int SPEED_FAST = 20;
const int SPEED_SLOW = 5;

const int DIR_CW         = 1;
const int DIR_CCW        = -1;
const int DIR_STATIONARY = 0;

const int FLAG_LINEAR = 1;
const int FLAG_SMOOTH = 2;

typedef struct Command {
  // steps per second
  int motorSpeed;
  
  // number of steps to move
  int steps;
  
  // the direction to turn the shaft
  int dir;
  
  // special flags for unusual operations
  int flag;
  
  // the next Command to execute
  Command* next;
  
} Command;

typedef Command* CmdPtr;

// BEGIN Command factories

Command* fastCW(int steps);

Command* fastCCW(int steps);

Command* slowCW(int steps);

Command* slowCCW(int steps);

Command* stationary(int seconds);

Command* fastSmoothCW(int steps);

Command* fastSmoothCCW(int steps);

Command* slowSmoothCW(int steps);

Command* slowSmoothCCW(int steps);

// END Command factories

// BEGIN Command combinators

Command* combine(Command* c1, Command* c2);

Command* repeat(int count, Command* c);

Command* repeatForSeconds(int seconds, Command* c);

Command* copy(Command* c);

// END Command combinators

void dispose(Command* c);

void traverseCommands(Command* commands, void (*executor)(Command*));

// return stepper square wave period for this angular velocity in rad/sec
int stepperPeriodForOmega(double omega);

#endif

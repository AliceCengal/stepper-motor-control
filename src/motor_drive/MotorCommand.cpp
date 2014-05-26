
#include "MotorCommand.h"
#include <math.h>

const double myPI = 3.141592653589793;

Command* createCommand(int speed, int steps, int dir) {
  Command* c = new Command();
  c->motorSpeed = speed;
  c->steps = steps;
  c->dir = dir;
  c->flag = 0;
  c->next = NULL;
  return c;
}

double continuousDerivativeAdjustment(double in) {
  return integralFactor * sin(myPI * in);
}

Command* fastCW(int steps) {
  return createCommand(SPEED_FAST, steps, DIR_CW);
}

Command* fastCCW(int steps) {
  return createCommand(SPEED_FAST, steps, DIR_CCW);
}

Command* slowCW(int steps) {
  return createCommand(SPEED_SLOW, steps, DIR_CW);
}

Command* slowCCW(int steps) {
  return createCommand(SPEED_SLOW, steps, DIR_CCW);
}

Command* stationary(int seconds) {
  return createCommand(1, seconds, DIR_STATIONARY);
}

Command* createSmoothSequence(int speed, int steps, int dir) {
  int section = 10;
  int sectionStep = steps / section;
  int remainder = steps % section;
  
  CmdPtr c = createCommand(speed * continuousDerivativeAdjustment(0.05), 
                           sectionStep, 
                           dir);
  for (int s = 1; s < section; ++s) {
    combine(c, createCommand(speed * continuousDerivativeAdjustment(double(s)/section + 0.05),
                             (s == 7) ? (sectionStep + remainder) : sectionStep,
                             dir));
  }
  return c;
}

Command* fastSmoothCW(int steps) {
  return createSmoothSequence(SPEED_FAST, steps, DIR_CW);
}

Command* fastSmoothCCW(int steps) {
  return createSmoothSequence(SPEED_FAST, steps, DIR_CCW);
}

Command* slowSmoothCW(int steps) {
  return createSmoothSequence(SPEED_SLOW, steps, DIR_CW);
}

Command* slowSmoothCCW(int steps) {
  return createSmoothSequence(SPEED_SLOW, steps, DIR_CCW);
}

Command* combine(Command* c1, Command* c2) {
  Command* c = c1;
  while (c->next != NULL) {
    c = c->next;
  }
  c->next = c2;
  return c1;
}

Command* repeat(int count, Command* c) {
  if (count < 2) {
    return c;
  } else {
    Command* initialCopy = copy(c);
    for (int i = 3; i <= count; ++i) {
      combine(initialCopy, copy(c));
    }
    combine(c, initialCopy);
  }
  return c;
}

Command* repeatForSeconds(int seconds, Command* c) {
  double commandDuration = 0;
  Command* curr = c;
  while (curr != NULL) {
    commandDuration += (curr->steps / curr->motorSpeed);
    curr = curr->next;
  }
  return repeat((((double)seconds) / commandDuration) + 1, c);
}

Command* copy(Command* c) {
  Command* c2 = new Command();
  c2->motorSpeed = c->motorSpeed;
  c2->steps = c->steps;
  c2->dir = c->dir;
  c2->flag = c->flag;
  
  if (c->next != NULL) {
    c2->next = copy(c->next);
  } else {
    c2->next = NULL;
  }
  return c2;
}

void dispose(Command* c) {
  if (c->next != NULL) {
    dispose(c->next);
  }
  delete c;
}

void traverseCommands(Command* commands, void (*executor)(Command*)) {
  Command* curr = commands;
  while(curr != NULL) {
    executor(curr);
    curr = curr->next;
  }
}

int stepperPeriodForOmega(double omega) {
  return 1000 / (STEPS_REV * omega);
}

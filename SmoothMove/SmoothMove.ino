#include <AccelStepper.h>
#include "hyperspeedchange.h"

AccelStepper stepper1(1, 4, 5); // stepP, dirP

hyperbolicSpeedChange SmoothMove1(1000,10,5000);

void setup()
{
  stepper1.setMaxSpeed(5000.0);

  stepper1.setSpeed(0.0);
  
  Serial.begin(9600);
}

void loop()
{
  SmoothMove1.begin(2000, true);

  stepper1.setSpeed(SmoothMove1.getNextSpeed());
  stepper1.setDirection(SmoothMove1.NewDirection);
  
}


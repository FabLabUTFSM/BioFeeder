#include <TFT.h>


#include <AccelStepper.h>

#define dirPin 8
#define stepPin 9
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);


void setup()
{  
   stepper.setMaxSpeed(1000);
   stepper.setSpeed(0.3);	
}

void loop()
{  
  digitalWrite(dirPin,HIGH);
   stepper.runSpeed();
}

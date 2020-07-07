#include <Arduino.h>
#define CLK 4
#define DT 3
#define SW 2
#define botonPulse 250

bool unitState = false;
int lastStateCLK;
int lastStateSW;
int currentStateCLK;
int unit=0;

void changeUnit() 
{
  
  return;
}

void setup() {
	
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);

	Serial.begin(250000);

	lastStateCLK = digitalRead(CLK);
  lastStateSW = digitalRead(SW);
  
  unit=0;
}

void loop() {
if (digitalRead(SW)==!lastStateSW){
  unitState= true;
  delay(botonPulse);
  while (unitState)
  {
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      if (digitalRead(DT) != currentStateCLK) {
			  unit --;
		  }  
      else {
			  unit ++;
		  }
	  }
    lastStateCLK = currentStateCLK;
  if (digitalRead(SW)==!lastStateSW)
    {
    unitState= false;
    delay(botonPulse);
    }
    Serial.println(unit);
  }
}
}





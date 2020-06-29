/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

#include <PID_v1.h>
#include <Wire.h>
#include <SFE_BMP180.h>

SFE_BMP180 bmp180;

int Altitude = 558; //current altitude in meters
float comp;

#define PIN_OUTPUT 3

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=0.3, Ki=1, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  //Initialize pressure sensor
  Serial.begin(9600);
  bool success = bmp180.begin();
  if (success) {
    Serial.println("BMP180 init success");
  }
  //initialize the variables we're linked to
  //Input = analogRead(PIN_INPUT);
  Setpoint = 1550;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  char status;
  double T, P;
  bool success = false;

  status = bmp180.startTemperature();

  if (status != 0) {
    delay(3);
    status = bmp180.getTemperature(T);

    if (status != 0) {
      status = bmp180.startPressure(3);

      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);

        if (status != 0) {
          comp = bmp180.sealevel(P, Altitude);
        }
      }
    }
  }
  Serial.println(comp);
  Input = comp;
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
}

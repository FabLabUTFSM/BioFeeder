#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLed variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

//I2C comunication
float keyValue=0; //Key value we are looking to modify in the feeder
bool i2c;
int i2cRecieve1;
String i2cDecoder[] = {"Neumatic Extruder" ,"Stepper Extruder", "-1" ,"1"};
#define adress 99
int buf[200];

//Encoder variables
#define CLK 2
#define DT 3
#define SW 4
#define botonPulse 250

bool keyValueState = false;
int lastStateCLK;
int lastStateSW;
int currentStateCLK;


float power(float x, int y)
{ 
  float mult=1.0;
  if (y>0)
  {
    for (int i=0 ; i < y ; i++ )
    {
      mult=mult*x;
    }
  }
  else
  {
    for (int i=0 ; i < abs(y) ; i++ )
    {
      mult=mult/x;
      Serial.println(mult);
    }
  return mult;
}

  
  return mult;
}
void getKeyValue()
{
  if (i2cDecoder[buf[0]]=="Neumatic Extruder")
  {
    keyValue=  power(10,(buf[1]*i2cDecoder[buf[2]].toInt()))+255* buf[3]+buf[4]*i2cDecoder[buf[5]].toInt();
  }
  else
  {
    keyValue = power(10,(buf[1]*i2cDecoder[buf[2]].toInt()))*(255* buf[3]+buf[4]*i2cDecoder[buf[5]].toInt());
    //keyValue = power(10,(buf[1]*i2cDecoder[buf[2]].toInt()));
    Serial.println(keyValue);
  }
}

void receiveEvent(int howMany)
{
  unsigned int pos=0;
  while (Wire.available () > 0)
  {
   byte c = Wire.read ();
   //Serial.println(c);
   if (pos < sizeof buf)
     buf [pos++] = c;
  } 
  getKeyValue();
  i2c=true;
  return;
}


void flag ()
{
  if (keyValueState)
  {
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      if (digitalRead(DT) != currentStateCLK) {
			  keyValue --;
		  }  
      else {
			  keyValue ++;
		  }
	  }
    lastStateCLK = currentStateCLK;
  /*if (digitalRead(SW)==!lastStateSW)
    {
    unitState= false;
    delay(botonPulse);
    }*/
  }
  return;
}

void setup() {
  Serial.begin(250000);
  //I2C initialization
  Wire.begin(adress); 
  Wire.onReceive(receiveEvent); // register event

  //Oled initialization
  /*if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.display();
  display.println("Hello, world!");
  display.display();
  delay(100);
  display.clearDisplay();
  display.display();*/

  //Encoder configuration
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
	lastStateCLK = digitalRead(CLK);
  lastStateSW = digitalRead(SW);
  attachInterrupt(digitalPinToInterrupt(CLK),flag,RISING);
}

void loop() { 
  //Serial.println(unitState);
  if (i2c){
    display.print(i2cDecoder[buf[0]]);
    display.display();
    Serial.println(i2cDecoder[buf[0]]);
    i2c=false;
  }

  //Mostar en pantalla
  if (digitalRead(SW)==!lastStateSW){
    keyValueState= true;
    delay(botonPulse);
    while (keyValueState)
    {
      Serial.println("entre");
      if (digitalRead(SW)==!lastStateSW)
        {
        keyValueState= false;
        delay(botonPulse);
      }
      display.clearDisplay();
      display.setCursor(0, 35);
      display.print("Key Value:");
      display.print(keyValue);
      display.display();
      /*Serial.print("Key Value: ");
      Serial.println(keyValue);*/
  }
  Serial.println(keyValueState);
}
}


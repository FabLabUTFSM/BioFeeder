#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLed variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

//I2C comunication
int keyValue=0; //Key value we are looking to modify in the feeder
bool i2c;
int i2cRecieve1;
String i2cDecoder[] = {"Neumatic Extruder" ,"Stepper Extruder"};
#define adress 99
int buf[200];

//Encoder variables
#define CLK 2
#define DT 3
#define SW 4
#define botonPulse 250

bool unitState = false;
int lastStateCLK;
int lastStateSW;
int currentStateCLK;


void receiveEvent(int howMany)
{
  int pos=0;
  while (Wire.available () > 0)
  {
   byte c = Wire.read ();
   if (pos < sizeof buf)
     buf [pos++] = c;
  } 
  i2c=true;
  return;
}

void flag ()
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
  if (digitalRead(SW)==!lastStateSW)
    {
    unitState= false;
    delay(botonPulse);
    }
  return;
}

void setup() {
  Serial.begin(250000);

  //I2C initialization
  Wire.begin(adress); 
  Wire.onReceive(receiveEvent); // register event

  //Oled initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
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
  display.display();

  //Encoder configuration
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);
	lastStateCLK = digitalRead(CLK);
  lastStateSW = digitalRead(SW);
  attachInterrupt(digitalPinToInterrupt(CLK),flag,RISING);
}

void loop() { 
  if (i2c){
    display.print(i2cDecoder[buf[0]]);
    display.display();
    Serial.println("entre");
    i2c=false;
  }

  //Mostar en pantalla
  if (digitalRead(SW)==!lastStateSW){
    unitState= true;
    delay(botonPulse);
    while (unitState)
    {

    if (digitalRead(SW)==!lastStateSW)
      {
      unitState= false;
      delay(botonPulse);
      }
      display.clearDisplay();
      display.setCursor(0, 35);
      display.print("Key Value:");
      display.print(keyValue);
      display.display();
      Serial.println(keyValue);
  }
}
}


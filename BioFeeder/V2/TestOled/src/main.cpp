#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLed variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

//I2C comunication
int x=0;
bool i2c;
String i2cDecoder[] = {"Neumatic Extruder" ,"Stepper Extruder"};



void receiveEvent(int howMany)
{
  x= Wire.read();
  i2c=true;
  return;
}


void setup() {
  Serial.begin(250000);

  //I2C initialization
  Wire.begin(99); 
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
}

void loop() {
  if (i2c){
    display.print(i2cDecoder[x]);
    display.display();
    i2c=false;
  }
}



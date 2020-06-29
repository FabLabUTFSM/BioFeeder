#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(250000);
  Wire.begin(99);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  pinMode(13,OUTPUT);

}

void loop() {
   Wire.onReceive(receiveEvent); // register event
   delay(100);

}
void receiveEvent(int howMany)
{
    int c = Wire.read(); // receive byte as a character
    Serial.println(c);
    show(c);
    digitalWrite(13,HIGH);
    delay(100);
    return;
}

void show(char c){
       display.setCursor(0, 10);
       display.println(c);
       display.display();   
       delay(100);
       display.clearDisplay();
       return;
}

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



const int PIR = 15; //pin the 1st PIR is connected inbound traffic
const int LED = 13; //LED
int pirActivated = 0;
int pirState = 0; // current state of PIR sensor
int pirPrevState = 0; // Previous state of PIR sensor


void setup() {
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  Serial.println("People Count:");
 }
}

void loop() {
  pirState = digitalRead(LED);
  if(pirState != pirPrevState) {
    if(pirState == HIGH) { // PIR is Active
      digitalWrite(LED, HIGH);
    }
  else { //PIR is not active
    digitalWrite(LED, LOW);
    pirActivated++;
    Serial.println(pirActivated); //Prints the count of people when sensor has motion
  }
  delay(50);
 }
 pirPrevState = pirState;
}

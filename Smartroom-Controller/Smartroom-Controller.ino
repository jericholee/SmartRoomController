//Libraries used in SmartRoom Controller -- Crowd Lens -- //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include <colors.h>

Adafruit_BME280 bme;

const int hexAddress = 0x76;
const int PIR_IN = 20;
const int ButtonCounter = 15;
const int PIXELPIN = 17;
const int PIXELCOUNT = 16;
int buttonState;
int p;
int pirCount = 0;
bool pirDetected;
bool pirState, lastPirState;
bool bmeStatus;
bool status;   //user to ensure port openned correctly
byte thisbyte; //used to get IP address

float tempf;
float tempC; 
float humidRH; 


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(ButtonCounter, INPUT);
  pinMode(PIR_IN, INPUT); 
  pixel.begin();
  pixel.show();
  
  bmeStatus = bme.begin(hexAddress);
   if (bmeStatus == false) {
    Serial.printf("BME280 at address 0x%02x failed to start", hexAddress);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
}


void loop() {

  buttonState = digitalRead(ButtonCounter);
  if(buttonState) {
    Serial.println("Button is pressed");
  }
  else {
    Serial.println("Button is not pressed");
  }

  if(buttonState = digitalRead(ButtonCounter)) {
    pirCount++;
  }
  tempC = bme.readTemperature();
  tempf = (tempC * 9.0/5.0) + 32.0;
  humidRH = bme.readHumidity();
  pirState = digitalRead(PIR_IN);
   if(pirState != lastPirState) {
     if(pirState == HIGH) {
        pirCount++;
        Serial.printf("Person In Count %i\n", pirCount);
      }
       lastPirState = pirState;
      }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.setRotation(0);
    display.printf("Person In Count %i\n", pirCount);
    display.printf("Temp: %.2f\n",tempf);
    display.printf("Hum: %.2f\n",humidRH);
    delay(500);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
    display.setTextSize(2);  // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.display();
     for(p = 0;p < PIXELCOUNT;p++) {
     pixel.setPixelColor(p,200,0,100);
     pixel.setBrightness(255);
     pixel.show();
     delay(100);
     pixel.clear();
     pixel.show();
      }
     }
 

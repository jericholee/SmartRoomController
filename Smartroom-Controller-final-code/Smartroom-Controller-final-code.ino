//Libraries used in SmartRoom Controller -- Crowd Lens -- //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>
#include <colors.h>

#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <wemo.h>


EthernetClient Client;

Adafruit_BME280 bme;

const int hexAddress = 0x76;
const int PIR_IN = 20;
const int ButtonCounter = 15;
const int PIXELPIN = 17;
const int PIXELCOUNT = 16;
int buttonState;
int p;
int pirCount = 0;
int hueChange = 0;
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
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pixel.begin();
  pixel.show();

  Ethernet.begin(mac);          
  printIP();
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());
  
  bmeStatus = bme.begin(hexAddress);
   if (bmeStatus == false) {
    Serial.printf("BME280 at address 0x%02x failed to start", hexAddress);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  Serial.print("My IP address: ");
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {
    //print value of each byte of the IP address
    Serial.printf("%i.",Ethernet.localIP()[thisbyte]);
    }
  Serial.printf("%i\n",Ethernet.localIP()[thisbyte]);
}

void loop() {
  tempC = bme.readTemperature();
  tempf = (tempC * 9.0/5.0) + 32.0;
  humidRH = bme.readHumidity();
  
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
    display.display();
    
     for(p = 0;p < PIXELCOUNT;p++) {
     pixel.setPixelColor(p,0,255,0);
     pixel.setBrightness(255);
     pixel.show();
     delay(100);
     pixel.clear();
     pixel.show();
      }
      if(pirCount <=3) {
        pixel.setPixelColor(p, 0,255,0);
        pixel.setBrightness(255);
        pixel.show();
        delay(100);
        pixel.clear();
        pixel.show();
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.setRotation(0);
        display.println("Safe to Enter");
        display.display();
        switchOFF(3);
        switchOFF(4);
        for(hueChange = 0; hueChange <=6; hueChange++) {
        setHue(hueChange,true,HueGreen,255,255);
     }
        setHue(1,true,HueGreen,255,255);
      }
      if((pirCount >3) && (pirCount <=6)){
        pixel.setPixelColor(p, 0, 0, 255);
        pixel.setBrightness(255);
        pixel.show();
        delay(100);
        pixel.clear();
        pixel.show();
        for(hueChange = 0; hueChange <=6; hueChange++) {
        setHue(hueChange,true,HueBlue,255,255);
     }
        setHue(1,true,HueBlue,255,255);
      }
      if((pirCount >6) && (pirCount <=9)){
        pixel.setPixelColor(p, 255, 255 ,0);
        pixel.setBrightness(255);
        pixel.show();
        delay(100);
        pixel.clear();
        pixel.show();
        for(hueChange = 0; hueChange <=6; hueChange++) {
        setHue(hueChange,true,HueYellow,255,255);
     }
        setHue(1,true,HueYellow,255,255);
      }
      if((pirCount >9) && (pirCount <=12)){
        pixel.setPixelColor(p, 255, 0 ,0);
        pixel.setBrightness(255);
        pixel.show();
        delay(100);
        pixel.clear();
        pixel.show();
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.setRotation(0);
        display.println("Overcrowded, Plz Wait");
        display.display();
        switchON(3);
        switchON(4);
        for(hueChange = 0; hueChange <=6; hueChange++) {
        setHue(hueChange,true,HueRed,255,255);
     }
   }
}

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);
}

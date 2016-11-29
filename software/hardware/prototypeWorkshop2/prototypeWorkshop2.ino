#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
//wifi
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define BUTTON_PIN  D1
#define PIN         D2
#define LED_COUNT    6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);
Servo myservo;

int oldTime = 0;
String chipID;

// This represents a value that, when updated repratedly, simulates a dampened spring
// See: https://en.wikipedia.org/wiki/Harmonic_oscillator
class SpringyValue {
  public:
    float x = 0, v = 0, a = 0, // x (value), v (velocity) and a (acceleration) define the state of the system
          o = 0, // o defines the temporary spring offset w.r.t. its resting position
          c = 1000.0, k = 5, m = 1.0; // c (spring constant), k (damping constant) and m (mass) define the behavior of the system
    //range c (100->1000); range k (0.1->25)

    // Perturb the system to change the "length" of the spring temporarlily
    void perturb(float offset) {
      this->x = offset;
      this->v = 0;
      this->a = 0;
    }

    // Call "update" every now and then to update the system
    // parameter dt specifies the elapsed time since the last update
    void update(float dt) {
      a = (-c * x - k * v ) / m;
      v += a * dt;
      x += v * dt ;
    }

    void resetServo() {
      a = 0;
      v = 0;
      x = 0;
    }
};


uint32_t LEDcolor = ESP.getChipId();
uint32_t id = ESP.getChipId();
float offset = 100;
uint32_t milliSeconds = 0;
SpringyValue v;
int LEDTimer = 0,
    springTimer = 0,
    lastMillis = 0,
    loopDuration = 0,
    servoTimer = 0,
    servoCutOffTimer = 0;


// LEDStrip helper to set the color of all LEDs and optionally their brightness
void setAllPixels(uint8_t r, uint8_t g, uint8_t b, int multiplier = 255) {
  float brightness = (float)multiplier / 255.0;
  for (int iPixel = 0; iPixel < LED_COUNT; iPixel++)
    strip.setPixelColor(iPixel,
                        (byte)((float)r * brightness),
                        (byte)((float)g * brightness),
                        (byte)((float)b * brightness));
  strip.show();
}


void setup() {
  id = id & 0x0000FFFF;
  chipID = String(id, HEX);
  chipID.toUpperCase();
  String wifiNameConcat = "Connectiklaas_" + chipID;
  char wifiName[] = "";
  wifiNameConcat.toCharArray(wifiName, 19);


  //for debugging
  //chipID = "T111";

  Serial.begin(115200);
  delay(1000);
  Serial.print(chipID);
  //  Serial.println();
  //  Serial.print("Last 2 bytes of chip ID: ");
  //  Serial.println(chipID);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  strip.begin();
  strip.setBrightness(255);
  colorFade(0x00ff00);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //wifiManager.resetSettings();

  //set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect(wifiName);
  //wifiManager.autoConnect();

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  colorFade(0xff0000);
  delay(500);
  hideColor();
  delay(500);
}


void loop() {
  // timekeeping
  milliSeconds = millis();
  loopDuration = milliSeconds - lastMillis;
  lastMillis = milliSeconds;
  LEDTimer += loopDuration;
  springTimer += loopDuration;
  servoTimer += loopDuration;
  servoCutOffTimer += loopDuration;

  // update the "spring" each 10 milliseconds
  if (springTimer > 10) {
    v.update(0.01);
    springTimer = 0;
  }

  // strecth the "spring"
  if (digitalRead(BUTTON_PIN) == LOW) {
    myservo.attach(D7);  // attaches the servo on pin 9 to the servo object
    v.perturb(offset);
    sendButtonPress();
    delay(100);
  }

  //cuts of power to the servo after 10 seconds to stop it from randomly twitching
  if (servoCutOffTimer > 10000) {
    myservo.detach();
    servoCutOffTimer = 0;
  }

  // update the LEDs every 15 milliseconds
  if (LEDTimer > 15) {
    // Use the absolute spring offset instead of the relative to determine brightness
    // There's no such thing as negative brightness
    colorFade(LEDcolor);
    strip.show();
    LEDTimer = 0;
  }

  if (servoTimer > 17) {
    int pos = map(v.x, -255, 255, 10, 170);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
  }

  if (millis() > oldTime + 2000) {
    requestMessage();
    oldTime = millis();
  }
}


void sendButtonPress() {
  Serial.println("Sending button press to server");
  HTTPClient http;
  http.begin("http://188.166.37.131/api.php?t=sqi&d=" + chipID);
  uint16_t httpCode = http.GET();
  http.end();
}


void requestMessage() {
  Serial.println("Sending request to server");
  hideColor();

  HTTPClient http;
  http.begin("http://188.166.37.131/api.php?t=gqi&d=" + chipID);
  uint16_t httpCode = http.GET();

  if (httpCode == 200) {
    String response;
    response = http.getString();
    //Serial.println(response);

    if (response == "-1") {
      Serial.println("There are no messages waiting in the queue");
    }
    else {
      int number = (int) strtol( &response[1], NULL, 16);
      LEDcolor = number;
    }
  }
  else {
    ESP.reset();
  }
  http.end();
}

//Turns off the LEDstrip
void hideColor() {
  colorWipe(strip.Color(0, 0, 0));
}

//Makes the LEDstrip turn green
void showColor() {
  colorWipe(strip.Color(255, 0, 0)); // Green 
}

//Sets all the GBR LED's to the color given, i has to loop through all the leds to change them all
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

//Color fading effect
void colorFade(uint32_t c) {
  //Serial.println(c);

  byte red = (c >> 16) & 0xff;
  byte green = (c >> 8) & 0xff;
  byte blue = c & 0xff;

  for (int j = 0; j < 100; j++) {
    float multiplier = ((float)j) / 100.0;
    float r = (float)red * multiplier;
    float g = (float)green * multiplier;
    float b = (float)blue * multiplier;

    setAllPixels(r, g, b, abs(v.x));
  }
  //
  //      for (uint16_t i = 0; i < strip.numPixels(); i++){
  //        strip.setPixelColor(i, (byte)r, (byte)g, (byte)b);
  //      }
  //
  //      strip.show();
  //      delay(5);
  //
  //    for (int j = 100; j > 0; j--){
  //      float multiplier = ((float)j) / 100.0;
  //      float r = (float)red * multiplier;
  //      float g = (float)green * multiplier;
  //      float b = (float)blue * multiplier;
  //
  //      for (uint16_t i = 0; i < strip.numPixels(); i++){
  //        strip.setPixelColor(i, (byte)r, (byte)g, (byte)b);
  //      }
  //
  //      strip.show();
  //      delay(8);
  //    }

}

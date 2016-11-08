#include <OpenWiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   D1   // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIN     D2    // Digital IO pin connected to the NeoPixels.

#define LED_COUNT 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);

OpenWiFi hotspot;

int oldTime = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // make the pushbutton's pin an input:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  String hexstring = "#FF00FF";

// Get rid of '#' and convert it to integer
int number = (int) strtol( &hexstring[1], NULL, 16);

colorWipe(number);
// Split them up into r, g, b values
int r = number >> 16;
int g = number >> 8 & 0xFF;
int b = number & 0xFF;



  // No wifi == no lights because the program will be busy connecting
  hotspot.begin("LAB", "QWERTYUIOP");
}

void loop() 
{
  if(digitalRead(BUTTON_PIN) == LOW)
  {
    sendButtonPress();
    delay(1000);
  }
  
  if(millis() > oldTime + 2000)
  {
    requestMessage();
    
    oldTime = millis();
  }
}

void hideColor() 
{
  colorWipe(strip.Color(0, 0, 0));
}

void showColor() 
{
  colorWipe(strip.Color(255, 0, 0)); // Red
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void sendButtonPress()
{
  Serial.println("Sending button press to server");
    HTTPClient http;
    http.begin("http://188.166.37.131/api.php?t=sqi&d=T111");
    uint16_t httpCode = http.GET();      
    http.end();
}

void requestMessage()
{
  Serial.println("Sending request to server");
  hideColor();
      
  HTTPClient http;
  http.begin("http://188.166.37.131/api.php?t=gqi&d=T111");
  uint16_t httpCode = http.GET();

  if (httpCode == 200) 
  {
    String response;
    response = http.getString();
    Serial.println(response);

    if(response == "-1")
    {
      Serial.println("There are no messages waiting in the queue");
    }
    else
    {
      int number = (int) strtol( &response[1], NULL, 16);
      colorWipe(number);
    }
  } 
    
  http.end();
}


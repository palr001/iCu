#include <OpenWiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN  D1
#define PIN         D2   
#define LED_COUNT    6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);

OpenWiFi hotspot;

int oldTime = 0;
String chipID;

void setup() 
{
  uint32_t id = ESP.getChipId();
  id = id & 0x0000FFFF;
  chipID = String(id, HEX);
  chipID.toUpperCase();

  //for debugging
  //chipID = "T111";
  
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Last 2 bytes of chip ID: ");
  Serial.println(chipID);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  strip.begin();
  strip.setBrightness(255);
  colorWipe(0x00ffff);
  strip.show();

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

void sendButtonPress()
{
  Serial.println("Sending button press to server");
    HTTPClient http;
    http.begin("http://188.166.37.131/api.php?t=sqi&d=" + chipID);
    uint16_t httpCode = http.GET();      
    http.end();
}

void requestMessage()
{
  Serial.println("Sending request to server");
  hideColor();
      
  HTTPClient http;
  http.begin("http://188.166.37.131/api.php?t=gqi&d=" + chipID);
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
      colorFade(number);
    }
  }
  else
  {
    ESP.reset(); 
  }
    
  http.end();
}

void hideColor() 
{
  colorWipe(strip.Color(0, 0, 0));
}

void showColor() 
{
  colorWipe(strip.Color(255, 0, 0)); // Red
}

void colorWipe(uint32_t c) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void colorFade(uint32_t c)
{
  byte red = (c >> 16) & 0xff;
  byte green = (c >> 8) & 0xff;
  byte blue = c & 0xff;

  for(int j = 0; j < 100; j++)
  {
    float multiplier = ((float)j)/100.0;
    float r = (float)red*multiplier;
    float g = (float)green*multiplier;
    float b = (float)blue*multiplier;

    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, (byte)r,(byte)g,(byte)b);
    }

    strip.show();
    delay(5);
  }
  
  for(int j = 100; j > 0; j--)
  {
    float multiplier = ((float)j)/100.0;
    float r = (float)red*multiplier;
    float g = (float)green*multiplier;
    float b = (float)blue*multiplier;

    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, (byte)r,(byte)g,(byte)b);
    }

    strip.show();
    delay(8);
  }

hideColor();
}


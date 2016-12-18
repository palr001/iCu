#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h> 
#include "SpringyValue.h"
#include "config.h"
#include "WS2812_util.h"

Servo myServo;

int oldTime = 0;
int oscillationTime = 500;
String chipID;
String serverURL = SERVER_URL;

void setup() 
{
  chipID = generateChipID();
  strip.begin();
  strip.setBrightness(255);
  WiFiManager wifiManager;
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  int counter = 0;
  while(digitalRead(BUTTON_PIN) == LOW)
  {
    counter++;
    delay(10);

    if(counter > 500)
    {
      wifiManager.resetSettings();
      Serial.println("Remove all wifi settings!");
      setAllPixels(255,0,0,1.0);
      fadeBrightness(255,0,0,1.0);
      ESP.reset();
    }
  }
  
  delay(1000);  
  Serial.print(String("\nLast 2 bytes of chip ID: ") + chipID);
  
  String configSSID = String(CONFIG_SSID) + "_" + chipID;  
  setAllPixels(0,255,255,1.0);
  wifiManager.autoConnect(configSSID.c_str());
  fadeBrightness(0,255,255,1.0);
  myServo.attach(D7);
}

//This method starts an oscillation movement in both the LED and servo
void oscillate(float springConstant, float dampConstant, int c)
{
  SpringyValue spring;
  
  byte red = (c >> 16) & 0xff;
  byte green = (c >> 8) & 0xff;
  byte blue = c & 0xff;
  
  spring.c = springConstant;
  spring.k = dampConstant / 100;
  spring.perturb(255);

  //Start oscillating
  for(int i = 0; i < oscillationTime; i++)
  {
    spring.update(0.01);
    setAllPixels(red, green, blue, abs(spring.x) / 255.0);
    myServo.write(90 + spring.x/4); 

    //Check for button press
    if(digitalRead(BUTTON_PIN) == LOW)
    {
      //Fade the current color out
      fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
      return;
    }
    delay(10);
  }
  fadeBrightness(red, green, blue, abs(spring.x) / 255.0);
}

void loop() 
{
  //Check for button press
  if(digitalRead(BUTTON_PIN) == LOW)
  {    
    sendButtonPress();
    delay(250);
  }

  //Every requestDelay, send a request to the server
  if(millis() > oldTime + REQUEST_DELAY)
  {
    requestMessage();
    oldTime = millis();
  }
}

void sendButtonPress()
{
    Serial.println("Sending button press to server");
    HTTPClient http;
    http.begin(serverURL + "/api.php?t=sqi&d=" + chipID);
    uint16_t httpCode = http.GET();      
    http.end();
}

void requestMessage()
{
  Serial.println("Sending request to server");
  hideColor();
      
  HTTPClient http;
  http.begin(serverURL + "/api.php?t=gqi&d=" + chipID + "&v=2");
  uint16_t httpCode = http.GET();

  if (httpCode == 200) 
  {
    String response;
    response = http.getString();
    //Serial.println(response);

    if(response == "-1")
    {
      Serial.println("There are no messages waiting in the queue");
    }
    else
    {
      //Get the indexes of some commas, will be used to split strings
      int firstComma = response.indexOf(',');
      int secondComma = response.indexOf(',', firstComma+1);
      int thirdComma = response.indexOf(',', secondComma+1);
      
      //Parse data as strings
      String hexColor = response.substring(0,7);
      String springConstant = response.substring(firstComma+1,secondComma);
      String dampConstant = response.substring(secondComma+1,thirdComma);;
      String message = response.substring(thirdComma+1,response.length());;

      Serial.println("Message received from server: \n");
      Serial.println("Hex color received: " + hexColor);
      Serial.println("Spring constant received: " + springConstant);
      Serial.println("Damp constant received: " + dampConstant);
      Serial.println("Message received: " + message);

      //Extract the hex color and fade the led strip
      int number = (int) strtol( &response[1], NULL, 16);
      oscillate(springConstant.toFloat(), dampConstant.toFloat(), number);
    }
  }
  else
  {
    ESP.reset(); 
  }
    
  http.end();
}

String generateChipID()
{
  String chipIDString = String(ESP.getChipId() & 0xffff, HEX);
  chipIDString.toUpperCase();
  while (chipIDString.length() < 4)
    chipIDString = String("0") + chipIDString;

  return chipIDString;
}


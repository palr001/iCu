#include <OpenWiFi.h>
#include <ESP8266HTTPClient.h>

OpenWiFi hotspot;
int pushButton = D1;
int oldButtonState = LOW;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  delay(10);

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);

  hotspot.begin("HomeNetwork", "HomePassword");
}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input pin:
  int buttonState = digitalRead(pushButton);

  delay(1); // delay in between reads for stability
  if (oldButtonState != buttonState) {
    if(buttonState == HIGH) {
      Serial.println("Send request to server");
      
      HTTPClient http;
      http.begin("http://178.62.233.141/add_entry.php");
      uint16_t httpCode = http.GET();
 
      if (httpCode == 200) {
        String response;
        response = http.getString();
        Serial.println(response);
      } else {
        ESP.reset();
      }
      
      http.end();
      oldButtonState = buttonState;
    } else {
      oldButtonState = LOW;
    }
  }
}

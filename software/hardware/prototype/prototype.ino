#include <OpenWiFi.h>
#include <ESP8266HTTPClient.h>
OpenWiFi hotspot;

int pushButton = D1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  delay(10);

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);

  hotspot.begin("HomeNetwork", "HomePassword");
}

int oldstate = 0;

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  // print out the state of the button:

  delay(1);        // delay in between reads for stability

  if (oldstate != buttonState && buttonState == 1) {
    HTTPClient http;
    http.begin("http://178.62.233.141/add_entry.php");
    uint16_t httpCode = http.GET();
    Serial.println("posted");
    String response;

    if (httpCode == 200) {
      response = http.getString();
      Serial.println(response);
    }
    else
      ESP.reset();

    http.end();

    oldstate = buttonState;
  }
}





#include <OpenWiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   D1   // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIN     D2    // Digital IO pin connected to the NeoPixels.

#define LED_COUNT 6

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);

OpenWiFi hotspot;

// State of availability
bool state = false;

// State of button press
int oldState;
int newState;

void setup() {
  Serial.begin(115200);
  delay(10);

  // make the pushbutton's pin an input:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // No wifi == no lights because the program will be busy connecting
  hotspot.begin("HomeNetwork", "HomePassword");
}

void loop() {
  // Get current button state.
  newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (oldState == LOW && newState == HIGH) {
    Serial.println("Send request to server");
      
    HTTPClient http;
    http.begin("http://178.62.201.186/add_entry.php");
    uint16_t httpCode = http.GET();
 
    if (httpCode == 200) {
      String response;
      response = http.getString();
      Serial.println(response);
    } else {
      ESP.reset();
    }
      
    http.end();

    if(state) {
      hideColor();
      state = false;
    } else {
      showColor();
      state = true;
    }
  }
  
  // Set the last button state to the old state.
  oldState = newState;
}

void hideColor() {
  colorWipe(strip.Color(0, 0, 0));
}

void showColor() {
  colorWipe(strip.Color(255, 0, 0)); // Red
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

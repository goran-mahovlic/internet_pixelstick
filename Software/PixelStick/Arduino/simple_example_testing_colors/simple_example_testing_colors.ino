#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

uint8_t r = 0; // define the variable for r
uint8_t g = 255; // define the variable for g
uint8_t b = 0; // define the variable for b
uint8_t x = 0;  // define the variable for led position

void setup() {
  Serial.begin(115200);
  delay(10);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off' 
}

void loop() {

for (x = 1 ; x< 59 ; x++)
{
  uint32_t c = strip.Color(r, g, b);  // define the variable c as (R,G,B)
  strip.setPixelColor(x, c);  // set LED x to the color in variable c (rgb)
  strip.show();               // Update 
  delay(1000);                // wait for a sec
}
Serial.println("NewLine");
}


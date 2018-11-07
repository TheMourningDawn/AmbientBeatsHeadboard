/*
* This is a RGB+W NeoPixel example, see extra-examples.cpp for a version
 * with more explantory documentation, example routines, how to
 * hook up your pixels and all of the pixel types that are supported.
 *
 */

/* ======================= includes ================================= */

#include "Particle.h"
#include "application.h"
#include "FastLED.h"
#include "neopixel.h"

// #include "application.h"
// #include "SpectrumEqualizerClient.h"
FASTLED_USING_NAMESPACE;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 90
#define PIXEL_TYPE SK6812RGBW
#define BRIGHTNESS 200 // 0 - 255

// SpectrumEqualizerClient *spectrum;

bool wasConnected = false;
bool poweredOn = true;
uint8_t hue = 200;
int currentPatternNumber = 0;
uint8_t counter = 0;

/* ======================= prototypes =============================== */

uint32_t Wheel(byte WheelPos);
// TODO: These need to be converted to functions with no args
void colorWipe(uint32_t c, uint8_t wait);
void pulseWhite(uint8_t wait);
void fullWhite();

// typedef void (*AnimationList[])();
// AnimationList animationList = {pulseWhite};

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = { rainbow, colorWipe, fullWhite };

int numberOfPatterns = ARRAY_SIZE(patterns) - 1;

/* ======================= rgbw-strandtest.cpp ====================== */

// SYSTEM_MODE(AUTOMATIC);

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  // Serial.begin(11520);
  setupCloudModeFunctions();

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
  // spectrum = new SpectrumEqualizerClient();
  // animations = new LEDAnimations(spectrum, strip);
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  // Do not run more than 15 seconds of these, or the b/g tasks
  // will be blocked.
  // --------------------------------------------------------------
  if(poweredOn) {
    patterns[currentPatternNumber]();
  }
  //
  // // fullWhite
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void colorAll() {
  colorAll(Wheel(hue), 1000);
  hue+=50;
}

void colorAll(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);
}

void colorWipe() {
  colorWipe(Wheel(hue), 50); // Red
  colorWipe(strip.Color(0 ,0,  0), 20);
  hue += 100;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0, gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }

  for(int j = 255; j >= 0 ; j--){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0, gamma[j] ) );
    }
    delay(wait);
    strip.show();
  }
}

void fullWhite() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
  }
  strip.show();
}

void rainbow() {
  rainbow(100);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle() {
  rainbowCycle(20);
}

// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void setupCloudModeFunctions() {
    Particle.function("next-mode", nextMode);
    Particle.function("previous-mode", previousMode);
    Particle.function("reset-device", resetDevice);
    Particle.function("safe-mode", enterSafeMode);
    Particle.function("power-on", powerOn);
    Particle.function("power-off", powerOff);
    Particle.function("toggle-music", toggleMusicReactive);
    Particle.function("set-rgb", setColor);
    Particle.function("set-hue", setHue);


    Particle.subscribe("NEXT_MODE", handleNextMode);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousMode);
    Particle.subscribe("RESET", handleReset);
}

int resetDevice(String arg) {
   System.reset();

   return 1;
}

void handleReset(const char *eventName, const char *data) {
  resetDevice("whatever");
}

int enterSafeMode(String arg) {
  System.enterSafeMode();

  return 1;
}

int powerOn(String arg) {
    poweredOn = true;

    return 1;
}

int powerOff(String arg) {
  poweredOn = false;

  return 1;
}

int toggleMusicReactive(String arg) {
  // animations->setMusicReactive(!animations->getMusicReactive());

  return 1;
}

int nextMode(String mode) {
    currentPatternNumber++;
    currentPatternNumber = wrapToRange(currentPatternNumber, 0, numberOfPatterns);
    return 0;
}

void handleNextMode(const char *eventName, const char *data) {
    nextMode("notSureWhyIHaveToDoItLikeThis");
}

int previousMode(String mode) {
    currentPatternNumber--;
    currentPatternNumber = wrapToRange(currentPatternNumber, 0, numberOfPatterns);
    return 0;
}

void handlePreviousMode(const char *eventName, const char *data) {
    previousMode("seriouslyWhy?");
}

int wrapToRange(int numberToWrap, int lowerBound, int upperBound) {
    if (numberToWrap > upperBound) {
        return lowerBound;
    } else if (numberToWrap < lowerBound) {
        return upperBound;
    }
    return numberToWrap;
}

int setHue(String hueString) {
  hue = hueString.toInt();
}

//Expects rgb values to be in r,g,b format e.g. 140,200,90
int setColor(String rgbString) {
   char buffer[12];
   rgbString.toCharArray(buffer, 12);
   String r = "";
   String g = "";
   String b = "";

   int rgbItem = 0;
   for(int i=0;i<12;i++) {
     if(buffer[i] != ',') {
       if(rgbItem == 0) {
         r.concat(buffer[i]);
       } else if(rgbItem == 1) {
         g.concat(buffer[i]);
       } else if(rgbItem == 2) {
         b.concat(buffer[i]);
       }
     } else {
       rgbItem++;
     }
   }

   CRGB rgb = CRGB(r.toInt(),g.toInt(),b.toInt());
   CHSV hsv = rgb2hsv_approximate(rgb);

   hue = hsv.hue;

   return hsv.hue;
}

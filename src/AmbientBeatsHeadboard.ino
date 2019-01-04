#include "Particle.h"
#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizerClient.h"
#include "AmbientBeatsCloudFunctions.h"

SpectrumEqualizerClient *audioEqualizer;
LEDAnimations *animations;
AmbientBeatsCloudFunctions *cloudFunctions;
Adafruit_NeoPixel *strip;

int speed = 50;

void setup() {
  // Serial.begin(11520);
  strip = new Adafruit_NeoPixel(96, D2, SK6812RGBW);

  audioEqualizer = new SpectrumEqualizerClient();
  animations = new LEDAnimations(audioEqualizer, strip);
  cloudFunctions = new AmbientBeatsCloudFunctions(animations);
}

void loop() {
  if(animations->poweredOn) {
    animations->runAnimation();
    animations->show();
    delay(speed);
  }
}

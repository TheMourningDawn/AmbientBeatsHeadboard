#ifndef LEDANIMATIONS_H
#define LEDANIMATIONS_H

#include "application.h"
#include "FastLED.h"
#include "SpectrumEqualizerClient.h"
#include "AmbientBeatsLEDAnimations.h"
#include "neopixel.h"

class LEDAnimations : public AmbientBeatsLEDAnimations
{
public:
    LEDAnimations(SpectrumEqualizerClient* eq, Adafruit_NeoPixel *neoPixelStrip);

    Adafruit_NeoPixel *strip;

    bool audioReactiveOn = false;
    int speed = 30;

    void show();
    int runAnimation();
    int toggleAudioReactive();

    void setBrightness(int newBrightness);

    void clearAllLeds();
    void fillColor();
    void fillColor(uint32_t c);

    // "Static" animations
    uint32_t Wheel(byte WheelPos);
    void colorWipe();
    void colorWipeWallReflection();
    void colorWipe(uint32_t c, uint8_t wait);
    void pulseWhite(uint8_t wait);
    void fullWhite();
    void rainbow();
//    void rainbow(uint8_t wait);
    void rainbowCycle();
//    void rainbowCycle(uint8_t wait);

    // Audio reactive animations
    void waterfall();
    void waterfallBorder(int frequencyValue, int sensitivityValueMinThreshold, int brightness);
};

#endif

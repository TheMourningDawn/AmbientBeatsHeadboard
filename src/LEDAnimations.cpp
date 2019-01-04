#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (LEDAnimations::*AnimationList)();

//TODO: Right now these are all the blocking animations, and a blank, because
AnimationList animationsAudioReactive[] = {};

AnimationList animationsRails[] = {&LEDAnimations::fullWhite, &LEDAnimations::colorWipeWallReflection,
  &LEDAnimations::fillColor, &LEDAnimations::colorWipe, &LEDAnimations::rainbowCycle, &LEDAnimations::rainbow };

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

LEDAnimations::LEDAnimations(SpectrumEqualizerClient *eq, Adafruit_NeoPixel *neoPixelStrip) : AmbientBeatsLEDAnimations(eq), strip(neoPixelStrip) {
    strip->setBrightness(brightness);
    strip->begin();
    strip->show(); // Initialize all pixels to 'off'

    animationCount = ARRAY_SIZE(animationsRails);
}

void LEDAnimations::show() {
    strip->show();
}

int LEDAnimations::runAnimation() {
    equalizer->readAudioFrequencies();
    if (audioReactiveOn) {
        (this->*animationsAudioReactive[animation])();
    } else {
        (this->*animationsRails[animation])();
    }
}

int previousColor = 0;
int LEDAnimations::toggleAudioReactive() {
    audioReactiveOn = !audioReactiveOn;
    animation = 0;
    if (audioReactiveOn) {
        animationCount = ARRAY_SIZE(animationsAudioReactive);
    } else {
        animationCount = ARRAY_SIZE(animationsRails);
    }

    return audioReactiveOn;
}

void LEDAnimations::setBrightness(int newBrightness) {
    AmbientBeatsLEDAnimations::setBrightness(newBrightness);
    strip->setBrightness(brightness);
    show();
}

void LEDAnimations::clearAllLeds() {
    for(int i=0;i<strip->numPixels();i++) {
        strip->setPixelColor(i, strip->Color(0,0,0));
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LEDAnimations::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
    }
    WheelPos -= 170;
    return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void LEDAnimations::fillColor() {
    uint32_t color = Wheel(hue);
    if (color != previousColor) {
        previousColor = color;
        fillColor(color);
    }
}

void LEDAnimations::fillColor(uint32_t c) {
    for(int i=0; i<strip->numPixels(); i++) {
        strip->setPixelColor(i, c);
    }
}

int pixelIndex = 0;
void LEDAnimations::colorWipe() {
    colorWipe(Wheel(hue), 25); // Red
    if(pixelIndex >= strip->numPixels() - 1) {
        hue += 80;
    }
}

void LEDAnimations::colorWipeWallReflection() {
    colorWipe(Wheel(hue), 34); // Red
    colorWipe(strip->Color(0, 0, 0), 20);
    hue += 100;
}

void LEDAnimations::colorWipe(uint32_t c, uint8_t wait) {
    pixelIndex = wrapToRange(pixelIndex, 0, strip->numPixels() - 1);
    strip->setPixelColor(pixelIndex, c);
//    strip->show();
    pixelIndex++;
}

void LEDAnimations::pulseWhite(uint8_t wait) {
    for(int j = 0; j < 256 ; j++) {
        for(uint16_t i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(0,0,0, gamma[j] ) );
        }
        delay(wait);
        strip->show();
    }

    for(int j = 255; j >= 0 ; j--){
        for(uint16_t i=0; i<strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(0,0,0, gamma[j] ) );
        }
        delay(wait);
        strip->show();
    }
}

void LEDAnimations::fullWhite() {
    for(uint16_t i=0; i<strip->numPixels(); i++) {
        strip->setPixelColor(i, strip->Color(0, 0, 0, gamma[hue]));
    }
    strip->show();
}

void LEDAnimations::rainbow() {
    static uint16_t j=0;
    for(int i=0; i<strip->numPixels(); i++) {
        strip->setPixelColor(i, Wheel((i+j) & 255));
    }
    j++;
    if(j >= 256) j=0;
}

void LEDAnimations::rainbowCycle() {
    static uint16_t j=0;
    for(int i=0; i< strip->numPixels(); i++) {
        strip->setPixelColor(i, Wheel(((i * 256 / strip->numPixels()) + j) & 255));
    }
    j++;
    if(j >= 256*5) j=0;
}

/************************************
 * Audio reactive animations
 ************************************/
//void LEDAnimations::waterfall() {
//    int sensitivityValueMinThreshold = clampSensitivity(sensitivity);
//    waterfallBorder(equalizer->frequenciesLeftChannel[frequencyMode[4]], sensitivityValueMinThreshold);
//}
//
//void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyValueMinThreshold) {
//    if (!audioReactiveOn || frequencyValue > frequencyValueMinThreshold) {
//        uint16_t mappedFrequencyValue = map(frequencyValue, frequencyValueMinThreshold, 4096, 0, 255);
//        mappedFrequencyValue = (mappedFrequencyValue) % 255; //offsetting the base color...
//        strip->setPixel(strip->numPixels()/2, Wheel(hue));
////        leds[NUM_LEDS / 2] = CHSV(mappedFrequencyValue, saturation, brightness);
//    } else {
//        strip->setPixel(strip->numPixels()/2, strip->Color(0,0,0));
////        leds[NUM_LEDS / 2] = CRGB(0, 0, 0);
//    }
//    // When doing a memmove, don't move outside of the bounds of the array!!!
//    memmove(&leds[0], &leds[1], (NUM_LEDS / 2) * sizeof(CRGB));
//    memmove(&leds[NUM_LEDS / 2 + 1], &leds[NUM_LEDS / 2], (NUM_LEDS / 2) * sizeof(CRGB));
//}

#endif

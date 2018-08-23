#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <FastLED.h>
#include <stdint.h>

// LedStrip led class. This class represents a block of data to leds.
typedef struct LedStrip {
    LedStrip() = default;
    CRGB *leds;
    int pin;
    int numLeds;
    // TODO int ledSegments[LEG_SEGMENTS];
    // int *legSegments;
    uint8_t brightness;
    uint8_t hue;
    uint8_t glitter;
    uint8_t colorIndex;;
    // Rainbow
    bool reverseDirection;
    int cooling;
    int sparking;
    int phaseShift;
    int cycleTime;
}LedStrip;

#endif /* LEDSTRIP_H */

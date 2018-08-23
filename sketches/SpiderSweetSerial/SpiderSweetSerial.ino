#include "FastLED.h"
#include <LedStrip.h>

#define CHIPSET WS2811
#define COLOR_ORDER GRB
// #define MINIME
// #define REPEATER

#define NUM_STRIPS 8
#define NUM_PALETTES 14
#define NUM_PATTERNS 8
#define NUM_EFFECTS NUM_PALETTES + NUM_PATTERNS

#ifdef MINIME
#define J_ONE_LEDS 16
#define J_TWO_LEDS 17
#define J_THREE_LEDS 9
#define J_FOUR_LEDS 10
#define J_FIVE_LEDS 14
#define J_SIX_LEDS 15
#define J_SEVEN_LEDS 21
#define J_EIGHT_LEDS 22
#define MAX_LEDS 22
#define SUM_LEDS 128
#elif defined REPEATER
#define SPIDER_LEG_ONE 606
#define SPIDER_LEG_TWO 606
#define SPIDER_LEG_THREE 342
#define SPIDER_LEG_FOUR 342
#define SPIDER_LEG_FIVE 549
#define SPIDER_LEG_SIX 549
#define SPIDER_LEG_SEVEN 792
#define SPIDER_LEG_EIGHT 792
#define SPIDER_MAX_LEDS 792
#define SUM_LEDS 4578
#else
#define SPIDER_LEG_ONE 202
#define SPIDER_LEG_TWO 202
#define SPIDER_LEG_THREE 114
#define SPIDER_LEG_FOUR 114
#define SPIDER_LEG_FIVE 183
#define SPIDER_LEG_SIX 183
#define SPIDER_LEG_SEVEN 264
#define SPIDER_LEG_EIGHT 264
#define SPIDER_MAX_LEDS 264
#define SUM_LEDS 1526
#endif
// Pin layouts on the teensy 3/3.1:
// WS2811_PORTD: 2,14,7,8,6,20,21,5
// WS2811_    J: 4, 1,7,6,8, 2, 3,5
// strip  order: 3, 5,8,2,1, 4, 6,7
#define J_ONE_PIN 14
#define J_TWO_PIN 20
#define J_THREE_PIN 21
#define J_FOUR_PIN 2
#define J_FIVE_PIN 5
#define J_SIX_PIN 8
#define J_SEVEN_PIN 7
#define J_EIGHT_PIN 6
// Swtich between test and real spider
#ifdef MINIME
#define STRIP_ONE_LEDS J_EIGHT_LEDS
#define STRIP_TWO_LEDS J_SIX_LEDS
#define STRIP_THREE_LEDS J_FOUR_LEDS
#define STRIP_FOUR_LEDS J_TWO_LEDS
#define STRIP_FIVE_LEDS J_ONE_LEDS
#define STRIP_SIX_LEDS J_THREE_LEDS
#define STRIP_SEVEN_LEDS J_FIVE_LEDS
#define STRIP_EIGHT_LEDS J_SEVEN_LEDS
#define NUM_LEDS_PER_STRIP MAX_LEDS
#else
#define STRIP_ONE_LEDS SPIDER_LEG_EIGHT
#define STRIP_TWO_LEDS SPIDER_LEG_SIX
#define STRIP_THREE_LEDS SPIDER_LEG_FOUR
#define STRIP_FOUR_LEDS SPIDER_LEG_TWO
#define STRIP_FIVE_LEDS SPIDER_LEG_ONE
#define STRIP_SIX_LEDS SPIDER_LEG_THREE
#define STRIP_SEVEN_LEDS SPIDER_LEG_FIVE
#define STRIP_EIGHT_LEDS SPIDER_LEG_SEVEN
#define NUM_LEDS_PER_STRIP SPIDER_MAX_LEDS
#endif

// 1:J5 2:J3 3:
#define STRIP_ONE_PIN J_EIGHT_PIN
#define STRIP_TWO_PIN J_SIX_PIN
#define STRIP_THREE_PIN J_FOUR_PIN
#define STRIP_FOUR_PIN J_TWO_PIN
#define STRIP_FIVE_PIN J_ONE_PIN
#define STRIP_SIX_PIN J_THREE_PIN
#define STRIP_SEVEN_PIN J_FIVE_PIN
#define STRIP_EIGHT_PIN J_SEVEN_PIN

// #define BRIGHTNESS  200
#define BRIGHTNESS  64
#define FRAMES_PER_SECOND 60
#define COOLING  55
#define SPARKING 120
#define PHASESHIFT 10
#define CYCLETIME 2500
#define HUE 0
#define GLITTER 80
#define COLOR_INDEX 0
#define UPDATES_PER_SECOND 100


CRGBPalette16 currentPalette;
TBlendType    currentBlending;
/*
 * ColorPalette
 */
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void ChangePalettePeriodically(uint8_t);
void FillLEDsFromPaletteColors(CRGB *leds, int numLeds, uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();

/*
 * DemoReel100
 */

struct Ledstrip;
void NextPattern();
void Rainbow(LedStrip*);
void RainbowWithGlitter(LedStrip*);
void AddGlitter(LedStrip*);
void Confetti(LedStrip*);
void Sinelon(LedStrip*);
void Bpm(LedStrip*);
void Juggle(LedStrip*);


CRGB leds1[STRIP_ONE_LEDS];
CRGB leds2[STRIP_TWO_LEDS];
CRGB leds3[STRIP_THREE_LEDS];
CRGB leds4[STRIP_FOUR_LEDS];
CRGB leds5[STRIP_FIVE_LEDS];
CRGB leds6[STRIP_SIX_LEDS];
CRGB leds7[STRIP_SEVEN_LEDS];
CRGB leds8[STRIP_EIGHT_LEDS];

CLEDController *controllers[NUM_STRIPS];

// Pin layouts on the teensy 3/3.1:
// WS2811_PORTD: 2,14,7,8,6,20,21,5
// WS2811_    J: 4, 1,7,6,8, 2, 3,5
// strip  order: 3, 5,8,2,1, 4, 6,7

LedStrip ledStrips[] = {
    { (leds4),STRIP_FOUR_PIN,STRIP_FOUR_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds1),STRIP_ONE_PIN,STRIP_ONE_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds7),STRIP_SEVEN_PIN,STRIP_SEVEN_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds6),STRIP_SIX_PIN,STRIP_SIX_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds8),STRIP_EIGHT_PIN,STRIP_EIGHT_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds2),STRIP_TWO_PIN,STRIP_TWO_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds3),STRIP_THREE_PIN,STRIP_THREE_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME},
    { (leds5),STRIP_FIVE_PIN,STRIP_FIVE_LEDS,BRIGHTNESS,HUE,GLITTER,COLOR_INDEX,false,COOLING,SPARKING,PHASESHIFT,CYCLETIME}
};

// DemoReel100
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(LedStrip *ledStrip);
SimplePatternList gPatterns = { Rainbow, RainbowWithGlitter, Confetti, Sinelon, Juggle, Bpm };

uint8_t gPalette = 0; // Index number for current palette
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
void setup()
{
	pinMode(PIN_A13, OUTPUT);
	digitalWrite(PIN_A13, LOW);
	// LEDS.addLeds<WS2811_PORTD,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);
    controllers[0] = &FastLED.addLeds<CHIPSET, STRIP_THREE_PIN, COLOR_ORDER>(ledStrips[2].leds, STRIP_THREE_LEDS).setCorrection( TypicalLEDStrip );
    controllers[1] = &FastLED.addLeds<CHIPSET, STRIP_FIVE_PIN, COLOR_ORDER>(ledStrips[4].leds, STRIP_FIVE_LEDS).setCorrection( TypicalLEDStrip );
    controllers[2] = &FastLED.addLeds<CHIPSET, STRIP_EIGHT_PIN, COLOR_ORDER>(ledStrips[7].leds, STRIP_EIGHT_LEDS).setCorrection( TypicalLEDStrip );
    controllers[3] = &FastLED.addLeds<CHIPSET, STRIP_TWO_PIN, COLOR_ORDER>(ledStrips[1].leds, STRIP_TWO_LEDS).setCorrection( TypicalLEDStrip );
    controllers[4] = &FastLED.addLeds<CHIPSET, STRIP_ONE_PIN, COLOR_ORDER>(ledStrips[0].leds, STRIP_ONE_LEDS).setCorrection( TypicalLEDStrip );
    controllers[5] = &FastLED.addLeds<CHIPSET, STRIP_FOUR_PIN, COLOR_ORDER>(ledStrips[3].leds, STRIP_FOUR_LEDS).setCorrection( TypicalLEDStrip );
    controllers[6] = &FastLED.addLeds<CHIPSET, STRIP_SIX_PIN, COLOR_ORDER>(ledStrips[5].leds, STRIP_SIX_LEDS).setCorrection( TypicalLEDStrip );
    controllers[7] = &FastLED.addLeds<CHIPSET, STRIP_SEVEN_PIN, COLOR_ORDER>(ledStrips[6].leds, STRIP_SEVEN_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( BRIGHTNESS );

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void loop()
{
    for (int effect = 0; effect < NUM_EFFECTS; effect++) {
        if (effect < NUM_PATTERNS) {
            // Demo
            // Call the current pattern function once, updating the 'leds' array
            for( int i = 0; i < NUM_STRIPS; i++) {
                // gPatterns[i](startLeds[i], numLedsPerStrip[i]);
                if (i == 0) {gPatterns[i](&ledStrips[i]);}
                else if (i == 1) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 2) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 3) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 4) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 5) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 6) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                else if (i == 7) {gPatterns[gCurrentPatternNumber](&ledStrips[i]);}
                // fill_solid(ledStrips[i].leds, ledStrips[i].numLeds, CRGB::MintCream);
                // controllers[i]->showLeds(ledStrips[i].brightness);
            }
            FastLED.show();
            // FastLED.delay(10);
            FastLED.delay(1000 / UPDATES_PER_SECOND);
            // do some periodic updates
            EVERY_N_MILLISECONDS( 20 ) { 
                for (int i = 0; i < NUM_STRIPS; i++) {
                    ledStrips[i].hue++;
                } // slowly cycle the "base color" through the rainbow
            }
            EVERY_N_SECONDS( 10 ) { NextPattern(); } // change patterns periodically
        } else {
            // ColorPalette
            // ChangePalettePeriodically(gPalette);
            EVERY_N_SECONDS( 10 ) { ChangePalettePeriodically(gPalette); } // change patterns periodically

            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; /* motion speed */

            // FillLEDsFromPaletteColors( startIndex);
            static uint8_t hue = 0;
            for (int i = 0; i < NUM_STRIPS; i++) {
                FillLEDsFromPaletteColors(&ledStrips[i], startIndex);
            }
            hue++;
            gPalette++;
            FastLED.show();
            // FastLED.delay(10);
            FastLED.delay(1000 / UPDATES_PER_SECOND);
        }
    }

}
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void NextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void FillLEDsFromPaletteColors(LedStrip *ledStrip, uint8_t colorIndex)
{
    uint8_t brightness = ledStrip->brightness;

    for( int j = 0; j < ledStrip->numLeds; j++) {
        ledStrip->leds[j] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.


void ChangePalettePeriodically(uint8_t palette)
{
    switch(palette) {
        case  0: currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; break;
        case 1: currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  break;
        case 2: currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; break;
        case 3: SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; break;
        case 4: SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; break;
        case 5: SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; break;
        case 6: SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; break;
        case 7: currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; break;
        case 8: currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; break;
        case 9: currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  break;
        case 10: currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; break;
        case 11: currentPalette = OceanColors_p; currentBlending = LINEARBLEND; break;
        case 12: currentPalette = LavaColors_p; currentBlending = LINEARBLEND; break;
        case 13: currentPalette = ForestColors_p; currentBlending = LINEARBLEND; break;
    }
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green  = CHSV(HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

void Rainbow(LedStrip *ledStrip) {
    // FastLED's built-in rainbow generator
    fill_rainbow(ledStrip->leds, ledStrip->numLeds, ledStrip->hue, 7);
}

void RainbowWithGlitter(LedStrip *ledStrip) {
    // built-in FastLED rainbow, plus some random sparkly glitter
    Rainbow(ledStrip);
    AddGlitter(ledStrip);
}

void AddGlitter(LedStrip *ledStrip) {
    if( random8() < GLITTER)
        ledStrip->leds[ random16(ledStrip->numLeds) ] += CRGB::White;
}

void Confetti(LedStrip *ledStrip) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( ledStrip->leds, ledStrip->numLeds, 10);
    int pos = random16(ledStrip->numLeds);
    ledStrip->leds[pos] += CHSV( ledStrip->hue + random8(64), 200, 255);
}

void Sinelon(LedStrip *ledStrip) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( ledStrip->leds, ledStrip->numLeds, 20);
    int pos = beatsin16( 13, 0, ledStrip->numLeds-1 );
    ledStrip->leds[pos] += CHSV( ledStrip->hue, 255, 192);
}

void Bpm(LedStrip *ledStrip) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < ledStrip->numLeds; i++) //9948
        ledStrip->leds[i] = ColorFromPalette(palette, ledStrip->hue+(i&2), beat-ledStrip->hue+(i&10));
}

void Juggle(LedStrip *ledStrip) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(ledStrip->leds, ledStrip->numLeds, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
        ledStrip->leds[beatsin16( i+7, 0, ledStrip->numLeds-1 )] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

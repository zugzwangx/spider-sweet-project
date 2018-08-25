#include <FastLED.h>
#include <vector>
#include <Floodlight.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

// #define MINIME
// #define REPEATER

#define NUM_STRIPS 8
#define NUM_PALETTES 14
#define NUM_PATTERNS 8
#define NUM_EFFECTS NUM_PALETTES + NUM_PATTERNS

#define NUM_FLOODLIGHTS 6
#define FLOODLIGHT_PIN1 2
#define FLOODLIGHT_PIN2 14
#define FLOODLIGHT_PIN3 7
#define FLOODLIGHT_PIN4 8
#define FLOODLIGHT_PIN5 6
#define FLOODLIGHT_PIN6 20
#define FLOODLIGHT_PIN7 21
#define FLOODLIGHT_PIN8 5

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

#define STRIP_ONE_PIN J_EIGHT_PIN 
#define STRIP_TWO_PIN J_SIX_PIN
#define STRIP_THREE_PIN J_FOUR_PIN
#define STRIP_FOUR_PIN J_TWO_PIN
#define STRIP_FIVE_PIN J_ONE_PIN
#define STRIP_SIX_PIN J_THREE_PIN
#define STRIP_SEVEN_PIN J_FIVE_PIN
#define STRIP_EIGHT_PIN J_SEVEN_PIN

#define BRIGHTNESS  64
#define FRAMES_PER_SECOND 60
#define COOLING  55
#define SPARKING 120
#define PHASESHIFT 10
#define CYCLETIME 2500

#define UPDATES_PER_SECOND 100

struct Floodlight;
struct FloodlightCommand;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
/*
 * ColorPalette
 */
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void ChangePalettePeriodically(uint8_t);
void FillLEDsFromPaletteColors(CRGB *leds, int numLeds, uint8_t colorIndex);
void SetupTotallyRandomPalette();
void SetupPurpleAndGreenPalette();
void SetupBlackAndWhiteStripedPalette();

/*
 * DemoReel100
 */
void NextPattern();
void Rainbow(CRGB *, int);
void RainbowWithGlitter(CRGB *, int);
void AddGlitter(CRGB*, int, fract8);
void Confetti(CRGB *, int);
void Sinelon(CRGB *, int);
void Bpm(CRGB *, int);
void Juggle(CRGB *, int);

void Fire(CRGB *, int);
void Disco(CRGB *leds, int numLeds);


CRGB leds[NUM_LEDS_PER_STRIP*NUM_STRIPS];
// represents legs in clock position facing underneath spider
CRGB* ledStrips[] = {
    (&(leds[(4*NUM_LEDS_PER_STRIP)])), // pin 6:    J8: index: 4
    (&(leds[(3*NUM_LEDS_PER_STRIP)])), // pin 8:    J6: index: 3 :J8
    (&(leds[(0*NUM_LEDS_PER_STRIP)])), // pin 2:    J4: index: 0 offsets: J4
    (&(leds[(5*NUM_LEDS_PER_STRIP)])), // pin 20:   J2: index: 5
    (&(leds[(1*NUM_LEDS_PER_STRIP)])), // pin 14:   J1: index: 1 offset: J8
    (&(leds[(6*NUM_LEDS_PER_STRIP)])), // pin 21:   J3: index: 6
    (&(leds[(7*NUM_LEDS_PER_STRIP)])),  // pin 5:    J5: index: 7
    (&(leds[(2*NUM_LEDS_PER_STRIP)])) // pin 7:    J7: index: 2 :J6
};

FloodlightCommand commandTable;

std::vector<Floodlight> floodlights = {
    { FLOODLIGHT_PIN1, commandTable.FLCommand[1] },
    { FLOODLIGHT_PIN2, commandTable.FLCommand[1] },
    { FLOODLIGHT_PIN3, commandTable.FLCommand[1] },
    { FLOODLIGHT_PIN4, commandTable.FLCommand[1] },
    { FLOODLIGHT_PIN5, commandTable.FLCommand[1] },
    { FLOODLIGHT_PIN6, commandTable.FLCommand[1] }
};

#ifdef MINIME
int numLedsPerStrip[8] = { 21, 14, 9, 16, 16, 9, 14, 21 };
#else
int numLedsPerStrip[NUM_STRIPS] = { NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP };
#endif
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB *leds, int numLeds);
SimplePatternList gPatterns = { Disco, Rainbow, RainbowWithGlitter, Confetti, Sinelon, Juggle, Bpm, Fire };

uint8_t gCommand = 0; // Index for current floodlight command
uint8_t gPalette = 0; // Index number for current palette
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t gReverseDirection = false;

std::vector<int> numEffects;

void setup()
{
	pinMode(PIN_A13, OUTPUT);
	digitalWrite(PIN_A13, LOW);
	LEDS.addLeds<WS2811_PORTD,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    // flooodlights
    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        (&floodlights[i]); // power on
    }
    
    // random generator function:
    // int myrandom (int i) { return std::rand()%i;}

    // std::srand ( unsigned ( std::time(0) ) );

    // set values:
    for (int i=0; i<NUM_EFFECTS; ++i) numEffects.push_back(i); // 1 2 3 4 5 6 7 8 9



}

void loop()
{
    // using built-in random generator:
    std::random_shuffle ( numEffects.begin(), numEffects.end() );

    // using myrandom:
    // std::random_shuffle ( numEffects.begin(), numEffects.end(), myrandom);
    
    // for (int effect = 0; effect < NUM_EFFECTS; effect++) {
    for (std::vector<int>::iterator effect=numEffects.begin(); effect!=numEffects.end(); ++effect) {
        if (*effect < NUM_PATTERNS) {
            // Demo
            // Call the current pattern function once, updating the 'leds' array
            for( int i = 0; i < NUM_STRIPS; i++) {
                // gPatterns[i](startLeds[i], numLedsPerStrip[i]);
                if (i == 0) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 1) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 2) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 3) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 4) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 5) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 6) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
                else if (i == 7) {gPatterns[*effect](ledStrips[i],NUM_LEDS_PER_STRIP);}
            }
            LEDS.show();
            // LEDS.delay(10);
            LEDS.delay(1000 / UPDATES_PER_SECOND);
            // do some periodic updates
            EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
            EVERY_N_SECONDS( 30 ) { NextPattern(); } // change patterns periodically
        } else {
            // ColorPalette
            // ChangePalettePeriodically(gPalette);
            EVERY_N_SECONDS( 10 ) { ChangePalettePeriodically(*effect); } // change patterns periodically

            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; /* motion speed */

            // FillLEDsFromPaletteColors( startIndex);
            static uint8_t hue = 0;
            for (int i = 0; i < NUM_STRIPS; i++) {
                FillLEDsFromPaletteColors(ledStrips[i], numLedsPerStrip[i], startIndex);
            }
            hue++;
            gPalette++;
            LEDS.show();
            // LEDS.delay(10);
            LEDS.delay(1000 / UPDATES_PER_SECOND);
        }
#ifdef SERVER_CLIENT
        EVERY_N_SECONDS( 30 ) {
            /*
             * Run as Floodlight Client, sending UART commands to the
             * Floodlight Server 
             */
            for (int floodlight=1; floodlight<NUM_FLOODLIGHTS; floodlight++) {
                if ((*effect > 2) && (*effect < 24)) {
                    floodlights[floodlight].currentCommand = commandTable.FLCommand[*effect];
                    SendFloodlightCommand(floodlights[floodlight].pin, *effect);
                }
            }
        }
#endif
    }
}

void Disco(CRGB *leds, int numLeds) {
    static uint8_t hue = 0;
    for( int i = 0; i < numLeds; i++)
        leds[i] = CHSV((32 * i) + hue, 192, 255);
    hue++;
}

void FillLEDsFromPaletteColors(CRGB *leds, int numLeds, uint8_t colorIndex)
{
    uint8_t brightness = BRIGHTNESS;

    for( int j = 0; j < numLeds; j++) {
        leds[j] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
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
        case 8: currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; break;
        case 9: currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  break;
        case 10: currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; break;
        case 11: SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; break;
        case 12: SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; break;
        case 13: SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; break;
        case 14: SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; break;
        case 15: currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; break;
        case 16: currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; break;
        case 17: currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  break;
        case 18: currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; break;
        case 19: currentPalette = OceanColors_p; currentBlending = LINEARBLEND; break;
        case 20: currentPalette = LavaColors_p; currentBlending = LINEARBLEND; break;
        case 21: currentPalette = ForestColors_p; currentBlending = LINEARBLEND; break;
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
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
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



void Fire(CRGB *stripLeds, int numLeds) {
    // Array of temperature readings at each simulation cell
    byte heat[numLeds];

    // Step 1.  Cool down every cell a little
    for( int i = 0; i < numLeds; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / numLeds) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= numLeds - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < numLeds; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (numLeds-1) - j;
      } else {
        pixelnumber = j;
      }
      *(stripLeds+pixelnumber) = color;
    }
}

// DemoReel100
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void NextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void Rainbow(CRGB *leds, int numLeds)
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, numLeds, gHue, 7);
}

void RainbowWithGlitter(CRGB *leds, int numLeds)
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  Rainbow(leds, numLeds);
  AddGlitter(leds, numLeds, 80);
}

void AddGlitter(CRGB *leds, int numLeds, fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(numLeds) ] += CRGB::White;
  }
}

void Confetti(CRGB *leds, int numLeds)
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, numLeds, 10);
  int pos = random16(numLeds);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void Sinelon(CRGB *leds, int numLeds)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, numLeds, 20);
  int pos = beatsin16( 13, 0, numLeds-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void Bpm(CRGB *leds, int numLeds)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < numLeds; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void Juggle(CRGB *leds, int numLeds) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, numLeds, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, numLeds-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


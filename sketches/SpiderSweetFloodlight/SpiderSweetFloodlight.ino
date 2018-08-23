#include <FastLED.h>
#include <vector>
#include <Floodlight.h>

// #define MINIME
// #define REPEATER

// Floodlight control
#define NUM_COMMANDS 24
#define START_COMMAND 1
#define END_COMMAND NUM_COMMANDS

// delay
#define DELAY_INBETWEEN_COMMANDS 10 
#define DELAY_INBETWEEN_FLOODLIGHTS 10 

// Control how many floodlights to test
#define NUM_FLOODLIGHTS 6
#define START_FLOODLIGHT 1
#define END_FLOODLIGHT NUM_FLOODLIGHTS

// Control order of floodlights here
// Change pin to the heart floodlight 
#define HEARTLIGHT_PIN 2
#define FLOODLIGHT_PIN1 HEARTLIGHT_PIN
#define FLOODLIGHT_PIN2 14
#define FLOODLIGHT_PIN3 7
#define FLOODLIGHT_PIN4 8
#define FLOODLIGHT_PIN5 6
#define FLOODLIGHT_PIN6 20
#define FLOODLIGHT_PIN7 21
#define FLOODLIGHT_PIN8 5


struct Floodlight;
struct FloodlightCommand;

void NextPattern();

FloodlightCommand commandTable;

std::vector<Floodlight> floodlights;

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])(CRGB *leds, int numLeds);
// SimplePatternList gPatterns = { Disco, Rainbow, RainbowWithGlitter, Confetti, Sinelon, Juggle, Bpm, Fire };

uint8_t gCommand = 0; // Index for current floodlight command
uint8_t gPalette = 0; // Index number for current palette
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
bool gHeart = false;
//
void setup()
{
	pinMode(PIN_A13, OUTPUT);
	digitalWrite(PIN_A13, LOW);
    // flooodlights
    floodlights = {
        { FLOODLIGHT_PIN1, commandTable.FLCommand[1] },
        { FLOODLIGHT_PIN2, commandTable.FLCommand[1] },
        { FLOODLIGHT_PIN3, commandTable.FLCommand[1] },
        { FLOODLIGHT_PIN4, commandTable.FLCommand[1] },
        { FLOODLIGHT_PIN5, commandTable.FLCommand[1] },
        { FLOODLIGHT_PIN6, commandTable.FLCommand[1] }
    };


    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        floodlights[i].writeCommand(); // power on
    }
}

void loop()
{

    for (uint8_t effect = START_COMMAND; effect < END_COMMAND; effect++) {
        EVERY_N_SECONDS( 30 ) {
            if (gHeart) {
                Heartrest(&floodlights[0]);
                gHeart = false;
            } else {
                Heartbeat(&floodlights[0]);
                gHeart = true;
            }
        }
        for (int floodlight=START_FLOODLIGHT; floodlight<END_FLOODLIGHT; floodlight++) {
            floodlights[floodlight].currentCommand = commandTable.FLCommand[effect];
            floodlights[floodlight].writeCommand();
            delay(DELAY_INBETWEEN_FLOODLIGHTS);
        }
        delay(DELAY_INBETWEEN_COMMANDS);
    }
}
void TestFloodlights(Floodlight *floodlights, int numFloodlights, uint32_t command) {
    for (int i=0; i<numFloodlights; i++) {
        floodlights[i].currentCommand = command;
    }
}

void ShowFloodlights(Floodlight *Floodlights, int numFloodlights) {
    for (int i=0; i<numFloodlights; i++) {
        floodlights[i].writeCommand();
    }
}

void Heartbeat(Floodlight *floodlight) {
    floodlight->currentCommand = LIGHT_R;
    floodlight->writeCommand(); // red
    floodlight->currentCommand = LIGHT_STROBE;
    floodlight->writeCommand(); // strobe
}

void Heartrest(Floodlight *floodlight) {
    floodlight->currentCommand = LIGHT_R;
    floodlight->writeCommand(); // red
    floodlight->currentCommand = LIGHT_SMOOTHE;
    floodlight->writeCommand(); // strobe
}


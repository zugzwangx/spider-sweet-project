#include <FastLED.h>
#include <vector>
#include <Floodlight.h>

// Test for sending floodlight commands to remove server
#define SERIAL 0
// Or test locally on Server and execute commands
#define SERVER_CLIENT 1

// Floodlight control
#define NUM_COMMANDS 24
#define START_COMMAND 0
#define END_COMMAND NUM_COMMANDS

// delay
#define DELAY_INBETWEEN_COMMANDS 10 
#define DELAY_INBETWEEN_FLOODLIGHTS 10 

// Control how many floodlights to test
#define NUM_FLOODLIGHTS 6
#define START_FLOODLIGHT 0 // 0-5 indices
#define END_FLOODLIGHT NUM_FLOODLIGHTS

// Control order of floodlights here
// Change pin to the heart floodlight 
#define HEARTLIGHT_PIN 5
#define FLOODLIGHT_PIN1 HEARTLIGHT_PIN
#define FLOODLIGHT_PIN2 20
#define FLOODLIGHT_PIN3 21
#define FLOODLIGHT_PIN4 2
#define FLOODLIGHT_PIN5 14
#define FLOODLIGHT_PIN6 8
#define FLOODLIGHT_PIN7 7
#define FLOODLIGHT_PIN8 6

#define TEENSY_LED 13

struct Floodlight;
struct FloodlightCommand;

void NextPattern();

FloodlightCommand commandTable;

std::vector<Floodlight> floodlights;


bool gHeart = false;
bool gShowFloodlights = true;
//
void setup()
{
	pinMode(PIN_A13, OUTPUT);
	digitalWrite(PIN_A13, LOW);
    //
    pinMode(TEENSY_LED, OUTPUT);
    // Open serial communications and wait for port to open:
    Serial5.begin(38400);
    // flooodlights
    floodlights = {
        { FLOODLIGHT_PIN1, commandTable.FLCommand[0] },
        { FLOODLIGHT_PIN2, commandTable.FLCommand[0] },
        { FLOODLIGHT_PIN3, commandTable.FLCommand[0] },
        { FLOODLIGHT_PIN4, commandTable.FLCommand[0] },
        { FLOODLIGHT_PIN5, commandTable.FLCommand[0] },
        { FLOODLIGHT_PIN6, commandTable.FLCommand[0] }
    };


    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        floodlights[i].writeCommand(); // power off
    }
}

void loop()
{
    int byte;

    uint8_t cmdTableIndex, cmdPin;

    for (uint8_t effect = START_COMMAND; effect < END_COMMAND; effect++) {
        if (SERVER_CLIENT ) {
            // Testing Floodlight Server
            while (Serial5.available()) {
                byte = Serial5.read();
                cmdTableIndex |= cmd & 0x1F;
                cmdPin |= (cmd >> 5) & 0x03;
                if ((cmdTableIndex < 32) && (cmdPin < 8)) {
                    floodlights[cmdPin].currentCommand = commandTable.FLCommand[cmdTableIndex];
                    floodlights[cmdPin].writeCommand();
                }
                Blink();
                delay(10000);
            }
        }

        if (SERIAL) {
            // Testing Floodlight Server
            while (Serial5.available()) {
                byte = Serial5.read();

            }
            // Testing Floodlight Client
            if (Serial5.availableForWrite()) {
                byte = 5;
                
                Serial5.write(byte);
                Blink();
            }
        }

        EVERY_N_SECONDS( 30 ) {
            if (gHeart) {
                Heartrest(&floodlights[0]);
                gHeart = false;
            } else {
                Heartbeat(&floodlights[0]);
                gHeart = true;
            }
            gShowFloodlights = true;
        }
        for (int floodlight=START_FLOODLIGHT; floodlight<END_FLOODLIGHT; floodlight++) {
            floodlights[floodlight].currentCommand = commandTable.FLCommand[effect];
            if (LOCAL) {
                floodlights[floodlight].writeCommand();
            } else if (SERVER_CLIENT) {
                SendFloodlightCommand(&floodlights[floodlight]);
            }
            delay(DELAY_INBETWEEN_FLOODLIGHTS);
        }
        delay(DELAY_INBETWEEN_COMMANDS);
    }
}

void SendFloodlightCommand(Floodlight *floodlight) {
    int byte;
    if (Serial5.availableForWrite()) {
        byte |= (5 << floodlight->pin) & 0xE0;
        byte |= floodlight->currentCommand & 0x1F;
        Serial5.write(byte);
        Blink();
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

void Blink() {
    digitalWrite(TEENSY_LED, HIGH);
    delay(1000);
    digitalWrite(TEENSY_LED, LOW);
    delay(1000);
}

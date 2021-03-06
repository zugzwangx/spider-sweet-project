#include <FastLED.h>
#include <vector>
#include "Floodlight.h"

// Test for sending floodlight commands to remove server
#define SERVER_CLIENT 0
// Test for UART
#define UART 0

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

// FloodlightCommand commandTable;

std::vector<Floodlight> floodlights;

bool gHeart = false;
bool gShowFloodlights = true;
//

uint32_t command_table[] = {LIGHT_OFF, LIGHT_ON, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_W, LIGHT_UP, LIGHT_DN,
                        LIGHT_R2, LIGHT_R3, LIGHT_R4, LIGHT_G2, LIGHT_G3, LIGHT_G4, LIGHT_G5,
                        LIGHT_B2, LIGHT_B3, LIGHT_B4, LIGHT_B5, LIGHT_FLASH, LIGHT_STROBE, LIGHT_FADE,
                        LIGHT_SMOOTHE};
void setup()
{
	pinMode(PIN_A13, OUTPUT);
	digitalWrite(PIN_A13, LOW);
    //
    pinMode(TEENSY_LED, OUTPUT);
    // Open serial communications and wait for port to open:
    Serial5.begin(38400, SERIAL_8N1);
    // flooodlights
    floodlights = {
        { FLOODLIGHT_PIN1, command_table[0] },
        { FLOODLIGHT_PIN2, command_table[0] },
        { FLOODLIGHT_PIN3, command_table[0] },
        { FLOODLIGHT_PIN4, command_table[0] },
        { FLOODLIGHT_PIN5, command_table[0] },
        { FLOODLIGHT_PIN6, command_table[0] }
    };


    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        floodlights[i].writeCommand(); // power off
    }

}

void loop()
{

#ifdef SERVER_CLIENT
    uint8_t byte = 0, cmdTableIndex, cmdPin;
    uint32_t command;

    /*
     * Run as Floodlight Server, receiving UART commands from the
     * Floodlight Client
     */
    while (Serial5.available()) {
        byte = Serial5.read();
        // Bits 0-4 represent the indexes for the command_table
        cmdTableIndex |= byte & 0x1F;
        // Bits 5-7 represent the indexes for the floodlights
        cmdPin |= (byte >> 5) & 0x03;
        if ((cmdTableIndex < 32) && (cmdPin < 8)) {
            floodlights[cmdPin].currentCommand = command_table[cmdTableIndex];
            floodlights[cmdPin].writeCommand();
        }
        Blink();
        delay(10000);
    }
        
#elif defined UART
    uint8_t byte = 0;

    /* Just comment out the Client if running Server, and 
     * vice versa
     */
    // Testing Floodlight Server
    while (Serial5.available()) {
        byte = Serial5.read();

    }
    // Testing Floodlight Client
    if (Serial5.availableForWrite()) {
        byte = 5;
        
        Serial5.write(byte);
        //Blink();
    }
#else
    /*
     * Outer Loop to test each command
     */

    for (uint8_t effect = 0; effect < NUM_FLOODLIGHTS; effect++) {

        /*
         * Inner Loop to test each floodlight
         *  
        for (int floodlight=START_FLOODLIGHT; floodlight<END_FLOODLIGHT; floodlight++) {
            floodlights[floodlight].currentCommand = commandTable.FLCommand[effect];
            floodlights[floodlight].writeCommand();

            delay(DELAY_INBETWEEN_FLOODLIGHTS);
        }
         */
        command = command_table[effect];
        TestFloodlights(floodlights, command);

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

        delay(DELAY_INBETWEEN_COMMANDS);
    }
#endif
}



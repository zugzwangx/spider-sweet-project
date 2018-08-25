#include <Arduino.h>
#include <vector>
#include <iterator>
#include <Floodlight.h>


struct FLCommandTable;
struct Floodlight;

static const uint32_t command_table[] = {LIGHT_OFF, LIGHT_ON, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_W, LIGHT_UP, LIGHT_DN,
                        LIGHT_R2, LIGHT_R3, LIGHT_R4, LIGHT_G2, LIGHT_G3, LIGHT_G4, LIGHT_G5,
                        LIGHT_B2, LIGHT_B3, LIGHT_B4, LIGHT_B5, LIGHT_FLASH, LIGHT_STROBE, LIGHT_FADE,
                        LIGHT_SMOOTHE};

const uint32_t *FloodlightCommand::FLCommand = command_table;

void Floodlight::writeCommand() {
    write_command(Floodlight::pin, Floodlight::currentCommand);
}

void SendFloodlightCommand(uint8_t cmdPin, uint8_t cmdTableIndex) {
    uint8_t byte = 0;
    if (Serial5.availableForWrite()) {
        byte |= (5 << cmdPin) & 0xE0;
        byte |= cmdTableIndex & 0x1F;
        Serial5.write(byte);
    }
}

void TestFloodlights(std::vector<Floodlight*> floodlights, uint32_t command) {
    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        floodlights[i]->currentCommand = command;
    }
}

void ShowFloodlights(std::vector<Floodlight*> floodlights) {
    for (int i=0; i<NUM_FLOODLIGHTS; i++) {
        floodlights[i]->writeCommand();
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

void write_command(uint8_t pin, uint32_t bitstream)
{ 
  digitalWrite(pin,0);
  delayMicroseconds(9000);

  digitalWrite(pin,1);
  delayMicroseconds(4500);

  for (int i = 31; i >= 0; i--)
  {
    int n = (bitstream >> i) & 1;
    output_bit(pin, n);
  }
  digitalWrite(pin,0);
  delayMicroseconds(562);
  digitalWrite(pin,1);
}

void output_bit(uint8_t pin, int n)
{
  if (n == 0)
  {
    digitalWrite(pin,0);
    delayMicroseconds(590);
    digitalWrite(pin,1);
    delayMicroseconds(520);
  }
  else
  {
    digitalWrite(pin,0);
    delayMicroseconds(540);
    digitalWrite(pin,1);
    delayMicroseconds(1700);
 }
}

/*

*/

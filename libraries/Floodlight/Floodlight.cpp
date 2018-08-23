#include "Floodlight.h"

class FLCommandTable;
struct Floodlight;

static const uint32_t command_table[] = {LIGHT_OFF, LIGHT_ON, LIGHT_R, LIGHT_G, LIGHT_B, LIGHT_W, LIGHT_UP, LIGHT_DN,
                        LIGHT_R2, LIGHT_R3, LIGHT_R4, LIGHT_G2, LIGHT_G3, LIGHT_G4, LIGHT_G5,
                        LIGHT_B2, LIGHT_B3, LIGHT_B4, LIGHT_B5, LIGHT_FLASH, LIGHT_STROBE, LIGHT_FADE,
                        LIGHT_SMOOTHE};
const uint32_t *FloodlightCommand::FLCommand = command_table;

void Floodlight::writeCommand() {
    write_command(this->pin, this->currentCommand);
}

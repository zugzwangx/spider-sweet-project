#ifndef FLOODLIGHT_H
#define FLOODLIGHT_H

#include <Arduino.h>
#include <stdint.h>
#include <vector>

 // Test serial output

//uint8_t pinTable[] = {2,14,7,8,6,20,21,5,24};
#define LED_OUT_5         5
#define T32_TO_LS_OE_N    32
#define ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define LIGHT_OFF 0x00F740BF
#define LIGHT_ON  0x00F7C03F
#define LIGHT_R   0x00F720DF
#define LIGHT_G   0x00F7A05F
#define LIGHT_B   0x00F7609F
#define LIGHT_W   0x00F7E01F
#define LIGHT_UP  0x00F700FF
#define LIGHT_DN  0x00F7807F
#define LIGHT_R2  0x00F710EF
#define LIGHT_R3  0x00F730CF
#define LIGHT_R4  0x00F708F7
#define LIGHT_R5  0x00F728D7
#define LIGHT_G2  0x00F7906F
#define LIGHT_G3  0x00F7B04F
#define LIGHT_G4  0x00F78877
#define LIGHT_G5  0x00F7A857
#define LIGHT_B2  0x00F750AF
#define LIGHT_B3  0x00F7708F
#define LIGHT_B4  0x00F758A7
#define LIGHT_B5  0x00F76897
#define LIGHT_FLASH   0x00F7D02F
#define LIGHT_STROBE  0x00F7F00F
#define LIGHT_FADE    0x00F7C837
#define LIGHT_SMOOTHE 0x00F7E817
// For Blink
#define TEENSY_LED 13

struct Floodlight;
void SendFloodlightCommand(uint8_t, uint8_t);
void TestFloodlights(std::vector<Floodlight>*, uint32_t);
void ShowFloodlights(std::vector<Floodlight>*);
void Heartbeat(Floodlight*);
void Heartrest(Floodlight*);
void Blink();
void write_command(uint8_t, uint32_t);
void output_bit(uint8_t, int);

typedef struct FloodlightCommand {
    static const uint32_t *FLCommand;
}FloodlightCommand;


typedef struct Floodlight {
    uint8_t pin;
    uint32_t currentCommand;
    void writeCommand();
}Floodlight;

/*
const int cmd_array_len = ELEMENTS(command_table);
static const int pin_array_len = ELEMENTS(pinTable);

void setup()
{
//  for (int i=0; i<pin_array_len; i++) {
//    pinMode(pinTable[i],OUTPUT);
//  }
  pinMode(T32_TO_LS_OE_N,OUTPUT);
  pinMode(LED_OUT_5,OUTPUT);

  digitalWrite(T32_TO_LS_OE_N,0); // #OE
  digitalWrite(LED_OUT_5,1);
}

void loop() {
//  write_command(0x00F7C03F);

  for (int i = 0; i < cmd_array_len; i++)
  {
    write_command(command_table[i]);
  }
}
*/

#endif /* FLOODLIGHT_H */

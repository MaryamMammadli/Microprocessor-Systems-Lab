// Slave Code
#include <avr/io.h>
#include <Arduino.h>

void SPI_SlaveInit() {
  // PB4 = MISO as output   
  DDRB |= (1 << PB4);

  // PB2 = SS, PB3 = MOSI, PB5 = SCK as inputs
  DDRB &= ~((1 << PB2) | (1 << PB3) | (1 << PB5));

  SPCR = (1 << SPE);

  SPDR = 60;
}

void setup() {
  Serial.begin(9600);
  SPI_SlaveInit();
}

void loop() {
  if (SPSR & (1 << SPIF)) { // SPI Interrupt Flag
    uint8_t received = SPDR;   // reading SPDR clears SPIF after SPSR was read

    Serial.println(received);

    SPDR = 60;
  }

}

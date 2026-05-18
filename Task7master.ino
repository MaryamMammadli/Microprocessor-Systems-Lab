// Master Code
#include <avr/io.h>
#include <util/delay.h>

uint8_t dataSequence[3] = {85, 170, 255};
uint8_t indexTx = 0;

void SPI_MasterInit() {
  // PB2 = SS, PB3 = MOSI, PB5 = SCK as outputs
  DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

  // PB4 = MISO as input
  DDRB &= ~(1 << PB4);

  // SS idle HIGH
  PORTB |= (1 << PB2);

  // fosc/128 = 16MHz/128 = 125kHz
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

uint8_t SPI_MasterTransmit(uint8_t data) {
  // Select slave: SS LOW
  PORTB &= ~(1 << PB2);

  // Load data to SPI data register
  SPDR = data;

  // Wait until transmission complete
  while (!(SPSR & (1 << SPIF))) { // interrupt flag
  }

  // Read SPDR to clear SPIF properly
  uint8_t received = SPDR;

  // Deselect slave: SS HIGH
  PORTB |= (1 << PB2);

  return received;
}

void setup() {
  SPI_MasterInit();
}

void loop() {
  SPI_MasterTransmit(dataSequence[indexTx]);

  indexTx++;
  if (indexTx >= 3) {
    indexTx = 0;
  }

  _delay_ms(1000);
}

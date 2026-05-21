// Master Code
#include <avr/io.h>
#include <util/delay.h>

// Array of data values to send
uint8_t dataSequence[3] = {85, 170, 255};

// Variable for array index
uint8_t indexTx = 0;

// Function to initialize SPI as Master
void SPI_MasterInit() {

  // Set PB2 (SS), PB3 (MOSI), and PB5 (SCK) as outputs
  DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

  // Set PB4 (MISO) as input
  DDRB &= ~(1 << PB4);

  // Keep SS pin HIGH when idle
  PORTB |= (1 << PB2);

  // Enable SPI in Master mode
  // Set clock speed to fosc/128 = 125kHz
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

// Function to send one byte with SPI
uint8_t SPI_MasterTransmit(uint8_t data) {

  // Select slave by making SS LOW
  PORTB &= ~(1 << PB2);

  // Put data into SPI register
  SPDR = data;

  // Wait until transmission is finished
  while (!(SPSR & (1 << SPIF))) { // SPI interrupt flag
  }

  // Read received data from SPDR
  uint8_t received = SPDR;

  // Deselect slave by making SS HIGH
  PORTB |= (1 << PB2);

  // Return received data
  return received;
}

void setup() {

  // Initialize SPI
  SPI_MasterInit();
}

void loop() {

  // Send current value from array
  SPI_MasterTransmit(dataSequence[indexTx]);

  // Move to next array value
  indexTx++;

  // Return to first value after last element
  if (indexTx >= 3) {
    indexTx = 0;
  }

  // Wait 1 second
  _delay_ms(1000);
}

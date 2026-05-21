// Slave Code
#include <avr/io.h>
#include <Arduino.h>

// Function to initialize SPI as Slave
void SPI_SlaveInit() {

  // Set PB4 (MISO) as output
  DDRB |= (1 << PB4);

  // Set PB2 (SS), PB3 (MOSI), and PB5 (SCK) as inputs
  DDRB &= ~((1 << PB2) | (1 << PB3) | (1 << PB5));

  // Enable SPI
  SPCR = (1 << SPE);

  // Load default reply value into SPDR
  SPDR = 60;
}

void setup() {

  // Start Serial communication
  Serial.begin(9600);

  // Initialize SPI Slave
  SPI_SlaveInit();
}

void loop() {

  // Check if SPI transmission is complete
  if (SPSR & (1 << SPIF)) { // SPI Interrupt Flag

    // Read received data
    // Reading SPDR clears SPIF
    uint8_t received = SPDR;

    // Print received value to Serial Monitor
    Serial.println(received);

    // Load reply value for next transmission
    SPDR = 60;
  }

}

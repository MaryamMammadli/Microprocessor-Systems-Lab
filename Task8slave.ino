// slave code

// CPU frequency
#define F_CPU 16000000UL

#include <avr/io.h>

// I2C address of slave
#define SLAVE_ADDR 0x08

// Variable to store current state
volatile uint8_t state = 0x00;

void setup() {

  // Set slave address
  // Shift left because last bit of TWAR is GCE
  TWAR = (SLAVE_ADDR << 1);

  // Enable TWI
  // Enable ACK
  // Set TWINT to continue communication
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

  // Enable pull-up resistors for SDA and SCL
  PORTC |= (1 << PC4) | (1 << PC5);

  // Set onboard LED pin as output
  DDRB |= (1 << PB5);

  // Set button pin as input
  DDRD &= ~(1 << PD2);

  // Enable pull-up resistor for button
  PORTD |= (1 << PD2);
}

void loop() {

      // Check if button is pressed
      if (!(PIND & (1 << PD2))) {

          // Save state value
          state = 0x02;
      }

      // Check if TWI event happened
      if (TWCR & (1 << TWINT)) {

          // Read TWI status
          // Ignore last 3 bits
          uint8_t status = TWSR & 0b11111000;

          // Own address + Write received
          if (status == 0x60) {

              // Continue communication
              TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

          // Data received after address
          } else if (status == 0x80) {

              // Read received data
              uint8_t received = TWDR;

              // If received value is 0x01
              // Turn LED on
              if (received == 0x01) {

                  PORTB |= (1 << PB5);

              } else {

                  // Otherwise turn LED off
                  PORTB &= ~(1 << PB5);
              }

              // Continue communication
              TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

          // Own address + Read received
          } else if (status == 0xA8) {

              // Load state value into data register
              TWDR = state;

              // Continue communication
              TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

              // Reset state after sending
              state = 0x00;

          } else {

              // Ignore other states
              // Continue communication
              TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
          }
      }
}

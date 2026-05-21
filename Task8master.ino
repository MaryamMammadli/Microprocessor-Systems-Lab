// master code

// CPU frequency for delay calculations
#define F_CPU 16000000UL

// I2C address of slave device
#define SLAVE_ADDR 0x08

/* PROTOCOL VALUES:
    0x00 - do nothing
    0x01 - send
    0x02 - receive */

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// Function to send START condition
void TWI_start() {

    // Send START signal and enable TWI
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    // Wait until START is sent
    while (!(TWCR & (1 << TWINT)));
}

// Function to send STOP condition
void TWI_stop() {

    // Send STOP signal
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

// Function to write one byte
void TWI_write(uint8_t data) {

    // Put data into data register
    TWDR = data;

    // Start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait until transmission finishes
    while (!(TWCR & (1 << TWINT)));
}

// Function to read one byte without ACK
uint8_t TWI_read_nack() {

    // Enable TWI for receiving data
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait until data is received
    while (!(TWCR & (1 << TWINT)));

    // Return received data
    return TWDR;
}

// Function to send data to slave
void send_to_slave(uint8_t data) {

    // Send START
    TWI_start();

    // Send slave address with write bit
    TWI_write((SLAVE_ADDR << 1) | 0);

    // Send data
    TWI_write(data);

    // Send STOP
    TWI_stop();
}

// Function to request data from slave
uint8_t request_from_slave() {

    uint8_t data;

    // Send START
    TWI_start();

    // Send slave address with read bit
    TWI_write((SLAVE_ADDR << 1) | 1);

    // Read data from slave
    data = TWI_read_nack();

    // Send STOP
    TWI_stop();

    // Return received data
    return data;
}

void setup() {

    // Set prescaler to 1
    TWSR = 0x00;

    // Set SCL frequency to 100kHz
    TWBR = 72;

    // Enable I2C (TWI)
    TWCR = (1 << TWEN);

    // Enable pull-up resistors on SDA and SCL
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

        // Send value 0x01 to slave
        send_to_slave(0x01);

    } else {

        // Send value 0x00 to slave
        send_to_slave(0x00);
    }

    // Request data from slave
    uint8_t received = request_from_slave();

    // If slave sends 0x02, turn on LED
    if (received == 0x02) {

        PORTB |= (1 << PB5);

    } else {

        // Otherwise turn off LED
        PORTB &= ~(1 << PB5);
    }

    // Small delay
    _delay_ms(10);
}

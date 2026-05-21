#include <avr/io.h>
#include <avr/interrupt.h>

// Variable for counting numbers
volatile uint8_t count = 0;

// Variable to check if timer is running
volatile uint8_t is_running = 1;

// Array for 7-segment display numbers
const uint8_t segments[10] = {
    0b10111111, // 0
    0b10000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Function to show digit on 7-segment display
static inline void displayDigit(uint8_t digit) {

    // Get segment value of digit
    uint8_t val = segments[digit];

    // Send first 5 bits to PORTD
    PORTD = (PORTD & 0x07) | ((val & 0x1F) << 3);

    // Send last 3 bits to PORTB
    PORTB = (PORTB & 0xF8) | ((val >> 5) & 0x07);
}

// Function to start Timer1
static inline void timer1_start(void) {

    // CTC mode and prescaler 256
    TCCR1B = (1 << WGM12) | (1 << CS12);
}

// Function to stop Timer1
static inline void timer1_stop(void) {

    // Keep CTC mode but stop clock
    TCCR1B = (1 << WGM12);
}

void setup() {

    // Disable global interrupts
    cli();

    // Set PD3–PD7 as output
    DDRD |= 0xF8;
    
    // Set PD2 as input for button
    DDRD &= ~(1 << PD2);

    // Enable pull-up resistor for PD2
    PORTD |= (1 << PD2);

    // Set PB0–PB2 as output
    DDRB |= 0x07;

    // Clear display outputs
    PORTD &= 0x07;
    PORTB &= 0xFC;

    // Reset Timer1 registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    // Compare value for 1 second delay
    OCR1A = 31249;

    // Enable Timer1 compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Interrupt on falling edge for INT0
    EICRA |= (1 << ISC01);

    // Enable external interrupt INT0
    EIMSK |= (1 << INT0);

    // Show first digit
    displayDigit(count);

    // Start timer
    timer1_start();

    // Enable global interrupts
    sei();
}

// Timer1 interrupt function
ISR(TIMER1_COMPA_vect) {

    // Increase count
    count++;

    // Return to 0 after 9
    if (count > 9) {
        count = 0;
    }

    // Show current count
    displayDigit(count);
}

// External interrupt function
ISR(INT0_vect) {

    // Change running state
    is_running ^= 1;

    // If running, start timer
    if (is_running) {
        timer1_start();

    // Otherwise stop timer
    } else {
        timer1_stop();
    }
}

void loop() {

    // Empty loop
}

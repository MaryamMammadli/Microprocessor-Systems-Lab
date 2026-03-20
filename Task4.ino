#include <avr/io.h> 
#include <avr/interrupt.h>

volatile uint8_t countdown = 0;

ISR(INT0_vect) { 
    PORTB |= (1 << 5); 
    countdown = 10;  
    TCNT1 = 0;                
    TIFR1 |= (1 << OCF1A);  
    TIMSK1 |= (1 << OCIE1A);   
}
ISR(TIMER1_COMPA_vect) { 
    if (countdown > 0) {
        countdown--;
        if (countdown == 0) {
            PORTB &= ~(1 << 5); 
            TIMSK1 &= ~(1 << OCIE1A);
        }
        }
    }

void setup() {

    DDRB |= (1 << 5);   
    PORTB &= ~(1 << 5);   

    DDRD &= ~(1 << 2);  
    PORTD |= (1 << 2); 

    EICRA &= ~((1 << ISC00));
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    TCCR1A = 0;                         // Clear control register A
    TCCR1B = 0;                         // Clear control register B
    TCNT1  = 0;                         // Reset counter
    
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler = 64

    OCR1A = 249; // 1 ms interval
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

void loop() {
    
}

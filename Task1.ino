// Task 1 - Register Method (Set/Clear)
void setup() {
  DDRB |= (1 << PORTB5);   
}

void loop() {
  PORTB |= (1 << PORTB5);    
  delay(500);

  PORTB &= ~(1 << PORTB5);  
  delay(500);
}

// Task 1 - Register Method (Toggle)
void setup() {
  DDRB |= (1 << PORTB5);   
}

void loop() {
  PORTB ^= (1 << PORTB5);
  delay(500);
}

// Task 1 - digitalWrite Method
void setup() { 
  pinMode(13, OUTPUT); 
}

void loop() {
  digitalWrite(13, HIGH); 
  delay(500);
  digitalWrite(13, LOW); 
  delay(500);
}

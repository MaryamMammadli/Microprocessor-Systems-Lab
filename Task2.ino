#include <Arduino.h>
// A 1st number from Serial
// B 2nd number from Serial
// R = subtraction result copied from CPU register
// S = copy of SREG (status register)
uint8_t A, B, R, S; 

void setup() {
  Serial.begin(9600); }

void loop() {
  if (!Serial.available()) 
    return; 

  A = Serial.parseInt(); // 1st typed number 
  B = Serial.parseInt(); // 2nd typed number

  while (Serial.available()) 
    Serial.read();

  // Inline AVR Assembly Block
  asm volatile( 

    "mov r20, %[a]      \n\t" 
    "mov r21, %[b]      \n\t" 
    "sub r20, r21       \n\t" //r20 = r20 - r21  updates SREG automatically
    "in  r22, __SREG__  \n\t"  
    "mov %[r], r20     \n\t" 
    "mov %[s], r22     \n\t"  

    : [r] "=r"(R),   
      [s] "=r"(S)
    : [a] "r"(A),   
      [b] "r"(B)     
    : "r20", "r21", "r22" 
  );

  Serial.print("A="); Serial.print(A);
  Serial.print(" B="); Serial.print(B);
  Serial.print(" R="); Serial.print(R);
  Serial.print(" S="); Serial.print(S);
  Serial.print(" Z="); Serial.print((S >> 1) & 1); 
  Serial.print(" C="); Serial.println(S & 1); 
  Serial.println(); 
}

#include <Arduino.h>
#include <avr/io.h>

#define LED_PIN 13
#define BUTTON_PIN 8

extern "C" void modeA();
extern "C" void modeB();
extern "C" void modeC();

extern "C" void modeA_entry();
extern "C" void modeB_entry();
extern "C" void modeC_entry();

volatile uint8_t action = 0;

void action0() {
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(200);
}

void action1() {
  digitalWrite(LED_PIN, LOW);
  delay(200);
  digitalWrite(LED_PIN, HIGH);
  delay(200);
}

void action2() {
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(300);
}

void action3() {
  action = 0;
}

/* Jump table for IJMP */
void (*actionTable[4])() = { //table of function addresses
  action0,
  action1,
  action2,
  action3
};

void dispatchAction() { //get the function pointer from the table
  void (*handler)() = actionTable[action];

  asm volatile(
    "movw r30,%A0\n"
    "ijmp\n"
    :
    : "r"(handler)
  );
}

void waitForFirstPress() {

  asm volatile(
    "boot_wait:\n"
    "sbic %[pin],0\n"
    "rjmp boot_wait\n"
    :
    : [pin] "I" (_SFR_IO_ADDR(PINB))
  );
}

int chooseMode() {

  int presses = 0;
  unsigned long start = millis();

  while (millis() - start < 2000) {
    if (digitalRead(BUTTON_PIN) == LOW) {

      presses++;

      while (digitalRead(BUTTON_PIN) == LOW); //wait for release
      delay(50);
    }
  }

  if (presses < 1) presses = 1;
  if (presses > 3) presses = 3;

  return presses;
}

extern "C" void modeA() {

  while (1) {
    if (digitalRead(BUTTON_PIN) == LOW) {

      action++;
      if (action > 3) action = 0;

      while (digitalRead(BUTTON_PIN) == LOW);
      delay(50);
    }

    dispatchAction();
  }
}

extern "C" void modeB() {

  while (1) {
    if (digitalRead(BUTTON_PIN) == LOW) {

      action++;
      if (action > 3) action = 0;

      while (digitalRead(BUTTON_PIN) == LOW);
      delay(50);
    }

    dispatchAction();
    dispatchAction();
    delay(400);         // pause between double blinks
  }
}

extern "C" void modeC() {

  while (1) {
    digitalWrite(LED_PIN, HIGH);
    delay(40);
    digitalWrite(LED_PIN, LOW);
    delay(40);
  }
}

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  waitForFirstPress();

  // start confirmation
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);

  int mode = chooseMode();

  if (mode == 1) {
    asm volatile("jmp modeA_entry");
  }

  if (mode == 2) {
    asm volatile("jmp modeB_entry");
  }

  if (mode == 3) {
    asm volatile("jmp modeC_entry");
  }

  while (1);
}


extern "C" __attribute__((used)) void modeA_entry() {
  modeA();
}

extern "C" __attribute__((used)) void modeB_entry() {
  modeB();
}

extern "C" __attribute__((used)) void modeC_entry() {
  modeC();
}

void loop() {}

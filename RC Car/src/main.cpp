#include <Arduino.h>

// ===== Relay outputs (each relay switches one LED) =====
#define RELAY1 25   // controls LED1
#define RELAY2 26   // controls LED2
#define RELAY3 27   // controls LED3
#define RELAY4 14   // controls LED4

// ===== Buttons =====
#define BTN1   32   // press -> LED1 on
#define BTN2   33   // press -> LED2 on
#define BTN3   18   // press -> LED3 on
#define BTN4   19   // press -> LED4 on

// Most relay modules turn ON when the input is LOW.
// If LEDs come on by default and turn off when pressed, swap these two.
#define RELAY_ON  HIGH
#define RELAY_OFF LOW

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("=== Booting up ===");

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);

  digitalWrite(RELAY1, RELAY_OFF);
  digitalWrite(RELAY2, RELAY_OFF);
  digitalWrite(RELAY3, RELAY_OFF);
  digitalWrite(RELAY4, RELAY_OFF);

  Serial.println("=== Setup complete, entering loop ===");
}

void loop() {
  bool b1 = digitalRead(BTN1) == LOW;
  bool b2 = digitalRead(BTN2) == LOW;
  bool b3 = digitalRead(BTN3) == LOW;
  bool b4 = digitalRead(BTN4) == LOW;

  digitalWrite(RELAY1, b1 ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY2, b2 ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY3, b3 ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY4, b4 ? RELAY_ON : RELAY_OFF);

  // Print only when something changes, so the Serial Monitor isn't flooded
  static bool last1=false, last2=false, last3=false, last4=false;
  if (b1 != last1) { Serial.println(b1 ? "BTN1 pressed -> RELAY1 ON" : "BTN1 released -> RELAY1 OFF"); last1=b1; }
  if (b2 != last2) { Serial.println(b2 ? "BTN2 pressed -> RELAY2 ON" : "BTN2 released -> RELAY2 OFF"); last2=b2; }
  if (b3 != last3) { Serial.println(b3 ? "BTN3 pressed -> RELAY3 ON" : "BTN3 released -> RELAY3 OFF"); last3=b3; }
  if (b4 != last4) { Serial.println(b4 ? "BTN4 pressed -> RELAY4 ON" : "BTN4 released -> RELAY4 OFF"); last4=b4; }
}
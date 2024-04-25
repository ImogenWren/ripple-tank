#include <autoDelay.h>




#define SPEAKER_PIN 3

#define BRIGHTNESS_PIN A0
#define STROBE_PIN A2
#define FREQUENCY_PIN A4

autoDelay printDelay;
#define PRINT_DELAY 1000

void setup() {
  Serial.begin(115200);
  Serial.println("\nRipple Tank - Prototype\n");
}

void loop() {
  uint16_t brightness_val, strobe_val, frequency_val;

  brightness_val = analogRead(BRIGHTNESS_PIN);
  strobe_val = analogRead(STROBE_PIN);
  frequency_val = analogRead(FREQUENCY_PIN);

  char buffer[64];
  sprintf(buffer, "brightness: %4u, strobe: %4u, frequency: %4u", brightness_val, strobe_val, frequency_val);

  if (printDelay.millisDelay(PRINT_DELAY)) {
    Serial.println(buffer);
  }
}

#include <autoDelay.h>




#define SPEAKER_PIN 3
#define LED_PWM_PIN 9

#define BRIGHTNESS_PIN A0
#define STROBE_PIN A2
#define FREQUENCY_PIN A4

autoDelay printDelay;
#define PRINT_DELAY 1000
autoDelay strobeDelay;


void setup() {
  Serial.begin(115200);
  Serial.println("\nRipple Tank - Prototype\n");
  analogWrite(LED_PWM_PIN, 0);
}


bool strobe_state = true;


void loop() {
  uint16_t brightness_val, strobe_val, frequency_val;

  brightness_val = map(analogRead(BRIGHTNESS_PIN), 0, 1024, 0, 255);
  strobe_val = analogRead(STROBE_PIN);
  frequency_val = map(analogRead(FREQUENCY_PIN), 0, 1024, 32, 2000);
  tone(SPEAKER_PIN, frequency_val);


  if (strobe_val < 2) {
    analogWrite(LED_PWM_PIN, brightness_val);
  } else {
    strobe_val = map(strobe_val, 0, 1024, 1000, 1);
    if (strobeDelay.millisDelay(strobe_val)) {
      if (strobe_state) {
        analogWrite(LED_PWM_PIN, brightness_val);
        strobe_state = false;
      } else {
        analogWrite(LED_PWM_PIN, 0);
        strobe_state = true;
      }
    }
  }

  char buffer[64];
  sprintf(buffer, "brightness: %4u, strobe: %4u, frequency: %4u", brightness_val, strobe_val, frequency_val);

  if (printDelay.millisDelay(PRINT_DELAY)) {
    Serial.println(buffer);
  }
}

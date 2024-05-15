#include <TimerInterrupt.h>
#include <TimerInterrupt.hpp>
#include <ISR_Timer.h>
#include <ISR_Timer.hpp>

#include <autoDelay.h>




#define SPEAKER_PIN 3
#define LED_PWM_PIN 9

#define BRIGHTNESS_PIN A0
#define STROBE_PIN A2
#define FREQUENCY_PIN A4

autoDelay printDelay;
#define PRINT_DELAY 1000
autoDelay strobeDelay;
autoDelay rippleDelay;



void setup() {
  Serial.begin(115200);
  Serial.println("\nRipple Tank - Prototype\n");
  TCCR1B = TCCR1B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  analogWrite(LED_PWM_PIN, 0);
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
}


bool strobe_state = true;

bool wave_state = false;

void run_LFO(uint32_t period) {
  if (rippleDelay.microsDelay(period)) {
    if (wave_state) {
      wave_state = false;
    } else {
      wave_state = true;
    }
    digitalWrite(SPEAKER_PIN, wave_state);
  }
}

void loop() {
  uint16_t brightness_val, strobe_val, frequency_val;

  brightness_val = map(analogRead(BRIGHTNESS_PIN), 0, 1024, 0, 255);
  strobe_val = analogRead(STROBE_PIN);
  frequency_val = map(analogRead(FREQUENCY_PIN), 0, 1024, 0, 204);
  // frequency_val = analogRead(FREQUENCY_PIN);

  // frequency = frequency * 0.1
  float Hz = float(frequency_val * frequency_val * 0.01);
  //  if (frequency_val < 32) {

  // } else {
  //   tone(SPEAKER_PIN, frequency_val);
  //  }

  uint32_t period_uS = uint32_t((1000000 / Hz) + 1);

  if (period_uS) {
    run_LFO(period_uS);
  } else {
    digitalWrite(SPEAKER_PIN, false);
  }

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
  char HzBuffer[16];
  char periodBuffer[16];
  dtostrf(Hz, 4, 2, HzBuffer);
  // dtostrf(period_uS, 4, 2, periodBuffer);
  sprintf(buffer, "brightness: %4u, strobe: %4u, frequency: %4s Hz %4u uS", brightness_val, strobe_val, HzBuffer, period_uS);

  if (printDelay.millisDelay(PRINT_DELAY)) {
    Serial.println(buffer);
  }
}

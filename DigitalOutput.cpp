#include <Arduino.h>
#include <DigitalOutput.h>

Feature* DigitalOutput::create(FeatureSpec spec) {
  if (spec.pinCount == 1 && spec.configSize == sizeof(DigitalOutputConfig)) {
    return new DigitalOutput(spec.pins[0], (DigitalOutputConfig*)spec.config);
  }

  return 0;
}

DigitalOutput::DigitalOutput(uint8_t pin, DigitalOutputConfig *config) {
  this->pin = pin;
  this->flags = config->flags;

  pinMode(pin, OUTPUT);
}

void DigitalOutput::readMessage(unsigned char *buf, int16_t msgsize) {
  int state = buf[0] == 'H' ? HIGH : LOW;

  if (isSet(DO_FLAG_REVERSED)) {
    state = state ^ 1; // reverse
  }

  digitalWrite(pin, state);
}

bool DigitalOutput::isSet(uint8_t flag) {
  return flags & flag;
}


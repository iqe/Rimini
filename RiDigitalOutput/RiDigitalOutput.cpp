#include <Arduino.h>
#include <RiDigitalOutput.h>

Feature* RiDigitalOutput::create(FeatureSpec spec) {
  if (spec.pinCount == 1 && spec.configSize == sizeof(RiDigitalOutputConfig)) {
    return new RiDigitalOutput(spec.pins[0], (RiDigitalOutputConfig*)spec.config);
  }

  return 0;
}

RiDigitalOutput::RiDigitalOutput(uint8_t pin, RiDigitalOutputConfig *config) {
  this->pin = pin;
  this->flags = config->flags;

  pinMode(pin, OUTPUT);
}

void RiDigitalOutput::readMessage(unsigned char *buf, int16_t msgsize) {
  int state = buf[0] == 'H' ? HIGH : LOW;

  if (isSet(DO_FLAG_REVERSED)) {
    state = state ^ 1; // reverse
  }

  digitalWrite(pin, state);
}

bool RiDigitalOutput::isSet(uint8_t flag) {
  return flags & flag;
}


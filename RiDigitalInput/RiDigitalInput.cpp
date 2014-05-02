#include <Arduino.h>
#include <RiDigitalInput.h>

Feature* RiDigitalInput::create(FeatureSpec spec) {
  if (spec.pinCount == 1 && spec.configSize == sizeof(RiDigitalInputConfig)) {
    return new RiDigitalInput(spec.pins[0], (RiDigitalInputConfig*)spec.config);
  }

  return 0;
}

RiDigitalInput::RiDigitalInput(uint8_t pin, RiDigitalInputConfig *config) {
  this->pin = pin;
  flags = config->flags;
  debounceMillis = config->debounceMillis;

  value = LOW;
  changed = false;
  lastChangedMillis = 0;

  if (isSet(DI_FLAG_INPUT_PULLUP)) {
    pinMode(pin, INPUT_PULLUP);
  } else {
    pinMode(pin, INPUT);
  }
}

void RiDigitalInput::update() {
  uint32_t now = millis();

  changed = false;

  if (now - lastChangedMillis >= debounceMillis) {
    int16_t newValue = digitalRead(pin);

    if (isSet(DI_FLAG_INPUT_PULLUP)) {
      newValue = newValue ^ 1; // reverse; with pullup, GND is logical HIGH
    }

    if (isSet(DI_FLAG_REVERSED)) {
      newValue = newValue ^ 1; // reverse
    }

    if (newValue != value) {
      lastChangedMillis = now;
      value = newValue;
      changed = true;
    }
  }
}

bool RiDigitalInput::isChanged() {
  return changed;
}

int16_t RiDigitalInput::writeMessage(unsigned char *buf, int16_t bufsize) {
  if (bufsize >= sizeof(uint8_t)) {
    buf[0] = value == HIGH ? 'H' : 'L';
    return sizeof(uint8_t);
  }
  return 0;
}

bool RiDigitalInput::isSet(uint8_t flag) {
  return flags & flag;
}


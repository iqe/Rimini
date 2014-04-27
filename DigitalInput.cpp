#include <Arduino.h>
#include <DigitalInput.h>

Feature* DigitalInput::create(FeatureSpec spec) {
  if (spec.pinCount == 1 && spec.configSize == sizeof(DigitalInputConfig)) {
    return new DigitalInput(spec.pins[0], (DigitalInputConfig*)spec.config);
  }

  return 0;
}

DigitalInput::DigitalInput(uint8_t pin, DigitalInputConfig *config) {
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

void DigitalInput::update() {
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

bool DigitalInput::isChanged() {
  return changed;
}

int16_t DigitalInput::sendMessage(unsigned char *buf, int16_t bufsize) {
  if (bufsize >= sizeof(uint8_t)) {
    buf[0] = value == HIGH ? 'H' : 'L';
    return sizeof(uint8_t);
  }
  return 0;
}

bool DigitalInput::isSet(uint8_t flag) {
  return flags & flag;
}


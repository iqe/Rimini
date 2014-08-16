#include <Arduino.h>
#include <RiDigitalInput.h>

Feature* RiDigitalInput::create() {
  return new RiDigitalInput();
}

RiDigitalInput::RiDigitalInput() {
  pin = 0;
  flags = 0;
  debounceMillis = 0;

  value = LOW;
  changed = false;
  lastChangedMillis = 0;
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

int16_t RiDigitalInput::readConfig(unsigned char *buf, int16_t msgsize) {
  // verify message size
  if (msgsize < DI_CONFIG_SIZE) { // FIXME check != DI_CONFIG_SIZE ?
    return -E_MESSAGE_TOO_SMALL;
  }

  int p = 1; // start at 1 to skip pinCount

  // read config (no verification required)
  pin = buf[p++];
  flags = buf[p++];
  debounceMillis = buf[p++] << 24 | buf[p++] << 16 | buf[p++] << 8 | buf[p++];

  if (isSet(DI_FLAG_INPUT_PULLUP)) {
    pinMode(pin, INPUT_PULLUP);
  } else {
    pinMode(pin, INPUT);
  }
}

int16_t RiDigitalInput::writeConfig(unsigned char *buf, int16_t bufsize) {
  // verify buffer size
  if (bufsize < DI_CONFIG_SIZE) {
    return -E_BUFFER_TOO_SMALL;
  }

  int p = 0;

  buf[p++] = 1; // pinCount
  buf[p++] = pin;
  buf[p++] = flags;

  buf[p++] = debounceMillis >> 24;
  buf[p++] = debounceMillis >> 16;
  buf[p++] = debounceMillis >> 8;
  buf[p++] = debounceMillis;

  return p;
}

bool RiDigitalInput::isSet(uint8_t flag) {
  return flags & flag;
}


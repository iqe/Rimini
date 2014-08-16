#include <Arduino.h>
#include <RiDigitalOutput.h>

Feature* RiDigitalOutput::create() {
  return new RiDigitalOutput();
}

RiDigitalOutput::RiDigitalOutput() {
  pin = 0;
  flags = 0;
}

void RiDigitalOutput::readMessage(unsigned char *buf, int16_t msgsize) {
  int state = buf[0] == 'H' ? HIGH : LOW;

  if (isSet(DO_FLAG_REVERSED)) {
    state = state ^ 1; // reverse
  }

  digitalWrite(pin, state);
}

int16_t RiDigitalOutput::readConfig(unsigned char *buf, int16_t msgsize) {
  // verify config size
  if (msgsize < DO_CONFIG_SIZE) {
    return -E_MESSAGE_TOO_SMALL;
  }

  int p = 1; // start at 1 to skip pinCount

  pin = buf[p++];
  flags = buf[p++];

  pinMode(pin, OUTPUT);

  return p;
}

int16_t RiDigitalOutput::writeConfig(unsigned char *buf, int16_t bufsize) {
  if (bufsize < DO_CONFIG_SIZE) {
    return -E_BUFFER_TOO_SMALL;
  }

  int p = 0;

  buf[p++] = 1; // pin count
  buf[p++] = pin;
  buf[p++] = flags;

  return p;
}

bool RiDigitalOutput::isSet(uint8_t flag) {
  return flags & flag;
}


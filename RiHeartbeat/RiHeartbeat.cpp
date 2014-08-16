#include <Arduino.h>
#include <RiHeartbeat.h>

Feature* RiHeartbeat::create(FeatureSpec spec) {
  if (spec.configSize == sizeof(RiHeartbeatConfig)) {
    return new RiHeartbeat((RiHeartbeatConfig*)spec.config);
  }

  return 0;
}

RiHeartbeat::RiHeartbeat(RiHeartbeatConfig *config) {
  for (int i = 0; i < HB_NAME_PIN_COUNT; i++) {
    this->config.namePins[i] = config->namePins[i];
  }
  this->config.intervalMillis = config->intervalMillis;

  heartbeatRequired = false;
  lastHeartbeatMillis = 0;
}

uint8_t RiHeartbeat::readName() {
  uint8_t name = 0;
  for (int i = 0; i < HB_NAME_PIN_COUNT; i++) {
    pinMode(config.namePins[i], INPUT_PULLUP);
    name |= digitalRead(config.namePins[i]) << i;
  }
  return name;
}

void RiHeartbeat::update() {
  heartbeatRequired = millis() - lastHeartbeatMillis > config.intervalMillis;
}

bool RiHeartbeat::isChanged() {
  return heartbeatRequired;
}

int16_t RiHeartbeat::writeMessage(unsigned char *buf, int16_t bufsize) {
  uint8_t name = readName();
  if (bufsize >= sizeof(name)) {
    lastHeartbeatMillis = millis();
    buf[0] = name;
    return sizeof(name);
  }
  return 0;
}

int16_t RiHeartbeat::writeConfig(unsigned char *buf, int16_t bufsize) {
  int16_t p = 0;

  int16_t pinsSize = HB_NAME_PIN_COUNT;
  int16_t configSize = HB_NAME_PIN_COUNT + sizeof(config.intervalMillis);

  if (bufsize < 1 + pinsSize + 2 + configSize) {
    return -E_BUFFER_TOO_SMALL;
  }

  // used pins
  buf[p++] = HB_NAME_PIN_COUNT;
  for (int i = 0; i < HB_NAME_PIN_COUNT; i++) {
    buf[p++] = config.namePins[i];
  }

  // config
  buf[p++] = configSize >> 8;
  buf[p++] = configSize;
  for (int i = 0; i < HB_NAME_PIN_COUNT; i++) {
    buf[p++] = config.namePins[i];
  }

  buf[p++] = config.intervalMillis >> 24;
  buf[p++] = config.intervalMillis >> 16;
  buf[p++] = config.intervalMillis >> 8;
  buf[p++] = config.intervalMillis;

  return p;
}


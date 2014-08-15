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

FeatureSpec RiHeartbeat::getFeatureSpec() {
  FeatureSpec spec;

  spec.pinCount = HB_NAME_PIN_COUNT;
  spec.pins = config.namePins;
  spec.configSize = sizeof(config);
  spec.config = &config;

  return spec;
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


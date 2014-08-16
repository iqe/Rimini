#include <Arduino.h>
#include <RiHeartbeat.h>

Feature* RiHeartbeat::create() {
  return new RiHeartbeat();
}

RiHeartbeat::RiHeartbeat() {
  for (int i = 0; i < HB_STREAMID_PIN_COUNT; i++) {
    streamIdPins[i] = 0;
  }
  intervalMillis = 0;

  heartbeatRequired = false;
  lastHeartbeatMillis = 0;
}

RiHeartbeat::RiHeartbeat(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint32_t intervalMillis) {
  streamIdPins[0] = pin0;
  streamIdPins[1] = pin1;
  streamIdPins[2] = pin2;

  this->intervalMillis = intervalMillis;

  heartbeatRequired = false;
  lastHeartbeatMillis = 0;
}

void RiHeartbeat::update() {
  heartbeatRequired = millis() - lastHeartbeatMillis > intervalMillis;
}

bool RiHeartbeat::isChanged() {
  return heartbeatRequired;
}

int16_t RiHeartbeat::writeMessage(unsigned char *buf, int16_t bufsize) {
  uint8_t streamId = readStreamId();

  if (bufsize < sizeof(streamId)) {
    return -E_BUFFER_TOO_SMALL;
  }

  lastHeartbeatMillis = millis();
  buf[0] = streamId;
  return sizeof(streamId);
}

uint8_t RiHeartbeat::readStreamId() {
  uint8_t streamId = 0;
  for (int i = 0; i < HB_STREAMID_PIN_COUNT; i++) {
    pinMode(streamIdPins[i], INPUT_PULLUP);
    streamId |= digitalRead(streamIdPins[i]) << i;
  }
  return streamId;
}

int16_t RiHeartbeat::writeConfig(unsigned char *buf, int16_t bufsize) {
  int16_t p = 0;

  // verify buffer size
  if (bufsize < HB_CONFIG_SIZE) {
    return -E_BUFFER_TOO_SMALL;
  }

  // write pins
  buf[p++] = HB_STREAMID_PIN_COUNT;
  for (int i = 0; i < HB_STREAMID_PIN_COUNT; i++) {
    buf[p++] = streamIdPins[i];
  }

  // write interval
  buf[p++] = intervalMillis >> 24;
  buf[p++] = intervalMillis >> 16;
  buf[p++] = intervalMillis >> 8;
  buf[p++] = intervalMillis;

  return p;
}

int16_t RiHeartbeat::readConfig(unsigned char *buf, int16_t msgsize) {
  uint8_t pins[HB_STREAMID_PIN_COUNT];
  uint32_t interval;

  int16_t p = 1; // start at 1 to ignore incoming pin count

  // verify message size
  if (msgsize < HB_CONFIG_SIZE) { // FIXME check if != HB_CONFIG_SIZE?
    return -E_MESSAGE_TOO_SMALL;
  }

  // read and verify pins
  for (int i = 0; i < HB_STREAMID_PIN_COUNT; i++) {
    pins[i] = buf[p++];
  }

  if (pins[0] == pins[1] || pins[0] == pins[2] || pins[1] == pins[2]) {
    return -E_HB_STREAMID_PINS_NOT_UNIQUE;
  }

  // read and verify interval
  interval = buf[p++] << 24 | buf[p++] << 16 | buf[p++] << 8 | buf[p++];

  if (interval < HB_MIN_INTERVAL_MILLIS) {
    return -E_HB_INTERVAL_TOO_SMALL;
  }

  // everything ok, apply
  for (int i = 0; i < HB_STREAMID_PIN_COUNT; i++) {
    streamIdPins[i] = pins[i];
  }
  intervalMillis = interval;

  return p;
}


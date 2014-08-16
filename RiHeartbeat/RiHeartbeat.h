#ifndef RiHeartbeat_h
#define RiHeartbeat_h

#include <Feature.h>

#define HB_STREAMID_PIN_COUNT 3
#define HB_MIN_INTERVAL_MILLIS 1000

#define E_HB_INVALID_PIN_COUNT 301
#define E_HB_STREAMID_PINS_NOT_UNIQUE 302
#define E_HB_INTERVAL_TOO_SMALL 303

#define HB_CONFIG_SIZE (sizeof(uint8_t) + (HB_STREAMID_PIN_COUNT * sizeof(uint8_t)) + sizeof(uint32_t))

class RiHeartbeat : Feature {
public:
  static Feature *create();

  void update();
  bool isChanged();
  int16_t writeMessage(unsigned char *buf, int16_t bufsize);

  int16_t readConfig(unsigned char *buf, int16_t msgsize);
  int16_t writeConfig(unsigned char *buf, int16_t bufsize);

private:
  uint8_t streamIdPins[HB_STREAMID_PIN_COUNT];
  uint32_t intervalMillis;

  bool heartbeatRequired;
  uint32_t lastHeartbeatMillis;

  RiHeartbeat();
  uint8_t readStreamId();
};

#endif


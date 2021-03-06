#ifndef RiHeartbeat_h
#define RiHeartbeat_h

#include <Feature.h>

#define HB_NAME_PIN_COUNT 3

struct RiHeartbeatConfig {
  uint8_t namePins[HB_NAME_PIN_COUNT];
  uint32_t intervalMillis;
};

class RiHeartbeat : Feature {
public:
  static Feature *create(FeatureSpec spec);

  void update();
  bool isChanged();
  int16_t writeMessage(unsigned char *buf, int16_t bufsize);

private:
  uint8_t name;
  uint32_t intervalMillis;

  bool heartbeatRequired;
  uint32_t lastHeartbeatMillis;

  RiHeartbeat(RiHeartbeatConfig *config);
  uint8_t readName(RiHeartbeatConfig *config);
};

#endif


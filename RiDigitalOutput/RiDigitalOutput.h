#ifndef RiDigitalOutput_h
#define RiDigitalOutput_h

#include <Feature.h>

#define DO_FLAG_REVERSED 1

struct RiDigitalOutputConfig {
  uint8_t flags;
};

class RiDigitalOutput : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void readMessage(unsigned char *buf, int16_t msgsize);

private:
  uint8_t pin;
  uint8_t flags;

  RiDigitalOutput(uint8_t pin, RiDigitalOutputConfig *config);

  bool isSet(uint8_t flag);
};

#endif


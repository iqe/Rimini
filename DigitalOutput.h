#ifndef DigitalOutput_h
#define DigitalOutput_h

#include <Feature.h>

#define DO_FLAG_REVERSED 1

struct DigitalOutputConfig {
  uint8_t flags;
};

class DigitalOutput : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void readMessage(unsigned char *buf, int16_t msgsize);

private:
  uint8_t pin;
  uint8_t flags;

  DigitalOutput(uint8_t pin, DigitalOutputConfig *config);

  bool isSet(uint8_t flag);
};

#endif


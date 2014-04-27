#ifndef DigitalInput_h
#define DigitalInput_h

#include <Feature.h>

#define DI_FLAG_INPUT_PULLUP 1
#define DI_FLAG_REVERSED     2

struct DigitalInputConfig {
  uint8_t flags;
  uint32_t debounceMillis;
};

class DigitalInput : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void update();
  bool isChanged();

  int16_t sendMessage(unsigned char *buf, int16_t bufsize);

private:
  uint8_t pin;
  uint8_t flags;
  uint32_t debounceMillis;

  int16_t value;
  bool changed;
  uint32_t lastChangedMillis;

  DigitalInput(uint8_t pin, DigitalInputConfig *config);

  bool isSet(uint8_t flag);
};

#endif


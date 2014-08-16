#ifndef RiDigitalInput_h
#define RiDigitalInput_h

#include <Feature.h>

#define DI_FLAG_INPUT_PULLUP 1
#define DI_FLAG_REVERSED     2

#define DI_CONFIG_SIZE (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t))

struct RiDigitalInputConfig {
  uint8_t flags;
  uint32_t debounceMillis;
};

class RiDigitalInput : Feature {
public:
  static Feature* create();

  void update();
  bool isChanged();

  int16_t writeMessage(unsigned char *buf, int16_t bufsize);

  int16_t readConfig(unsigned char *buf, int16_t msgsize);
  int16_t writeConfig(unsigned char *buf, int16_t bufsize);

private:
  uint8_t pin;
  uint8_t flags;
  uint32_t debounceMillis;

  int16_t value;
  bool changed;
  uint32_t lastChangedMillis;

  RiDigitalInput();

  bool isSet(uint8_t flag);
};

#endif


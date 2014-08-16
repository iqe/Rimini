#ifndef RiDigitalOutput_h
#define RiDigitalOutput_h

#include <Feature.h>

#define DO_FLAG_REVERSED 1

#define DO_CONFIG_SIZE (sizeof(uint8_t) + sizeof(uint8_t))

struct RiDigitalOutputConfig {
  uint8_t flags;
};

class RiDigitalOutput : Feature {
public:
  static Feature* create();

  void readMessage(unsigned char *buf, int16_t msgsize);

  int16_t readConfig(unsigned char *buf, int16_t msgsize);
  int16_t writeConfig(unsigned char *buf, int16_t bufsize);

private:
  uint8_t pin;
  uint8_t flags;

  RiDigitalOutput();

  bool isSet(uint8_t flag);
};

#endif


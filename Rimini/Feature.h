#ifndef Feature_h
#define Feature_h

#include <inttypes.h>

class Feature {
public:
  ~Feature() {}

  // static Feature* create(FeatureSpec spec);  <-- Required by all implementations

  virtual void update() {}
  virtual bool isChanged() { return false; }

  virtual int16_t writeMessage(unsigned char *buf, int16_t bufsize) { return 0; }
  virtual void readMessage(unsigned char *buf, int16_t msgsize) {}
};

struct FeatureSpec {
  uint8_t* pins;
  uint8_t pinCount;
  void* config;
  int16_t configSize;
};

typedef Feature* (*FeatureFactoryMethod)(FeatureSpec);

#endif


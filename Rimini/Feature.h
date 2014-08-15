#ifndef Feature_h
#define Feature_h

#include <inttypes.h>
#include <Errors.h>

struct FeatureSpec {
  uint8_t pinCount;
  uint8_t* pins;
  int16_t configSize;
  void* config;
};

class Feature {
public:
  ~Feature() {}

  // static Feature* create(FeatureSpec spec);  <-- Required by all implementations

  // FIXME Make this method abstract and implement it in all existing features
  virtual FeatureSpec getFeatureSpec() { FeatureSpec spec = {}; return spec; }

  virtual void update() {}
  virtual bool isChanged() { return false; }

  virtual int16_t writeMessage(unsigned char *buf, int16_t bufsize) { return 0; }
  virtual void readMessage(unsigned char *buf, int16_t msgsize) {}
};

typedef Feature* (*FeatureFactoryMethod)(FeatureSpec);

#endif


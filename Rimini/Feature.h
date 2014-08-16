#ifndef Feature_h
#define Feature_h

#include <inttypes.h>
#include <Errors.h>

class Feature {
public:
  ~Feature() {}

  // static Feature* create(unsigned char *buf, int16_t bufsize);  <-- Required by all implementations

  virtual void update() {}
  virtual bool isChanged() { return false; }

  virtual int16_t writeMessage(unsigned char *buf, int16_t bufsize) { return 0; }
  virtual void readMessage(unsigned char *buf, int16_t msgsize) {}

  virtual int16_t readConfig(unsigned char *buf, int16_t msgsize) { return 0; }
  virtual int16_t writeConfig(unsigned char *buf, int16_t bufsize) { return 0; }
};

typedef Feature* (*FeatureFactoryMethod)(void);

#endif


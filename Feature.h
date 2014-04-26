#ifndef Feature_h
#define Feature_h

#include <inttypes.h>

class Feature {
public:
  ~Feature() {}
  virtual void initialize() {}

  virtual void update() {}
  virtual bool isChanged() { return false; }

  virtual int16_t sendMessage(char* buf, int16_t bufsize) { return 0; }
  virtual void receiveMessage(char* buf, int16_t msgsize) {}
};

typedef struct {
  uint8_t* pins;
  uint8_t pinCount;
  void* config;
  int16_t configSize;
} FeatureSpec;

typedef Feature* (*feature_factory_method)(FeatureSpec);

#endif


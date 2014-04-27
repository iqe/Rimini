#ifndef Config_h
#define Config_h

#include <Feature.h>
#include <Repository.h>

struct FeatureConfigHeader {
  uint8_t type;
  int16_t id;
  uint8_t pinCount;
  uint8_t configSize;
};

class Config : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void receiveMessage(unsigned char *buf, int16_t msgsize);

private:
  Repository *repo;

  Config(Repository *repo);
};

#endif


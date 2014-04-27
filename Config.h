#ifndef Config_h
#define Config_h

#include <Feature.h>
#include <Repository.h>

struct FeatureConfigHeader {
  uint8_t mode;
  int16_t id;
  uint8_t pinCount;
  uint8_t configSize;
};

class Config : Feature {
public:
  Config(Repository *repo);

  void receiveMessage(char* buf, int16_t msgsize);

private:
  Repository *repo;
};

#endif


#ifndef Repository_h
#define Repository_h

#include <Feature.h>
#include <PinKeeper.h>
#include <Factory.h>

struct FeatureConfigHeader {
  uint8_t mode;
  int16_t id;
  uint8_t pinCount;
  uint8_t configSize;
};

class Repository {
public:
  Repository(PinKeeper *pinKeeper, Factory *factory, uint8_t featureCount);
  ~Repository();

  void createFeature(char *cfgBuf, int16_t cfgBufSize);
  void deleteFeature(int16_t id);

  Feature* getFeature(int16_t id);

private:
  PinKeeper *pinKeeper;
  Factory *factory;

  int16_t featureCount;
  Feature **features;

  bool isValidFeatureId(int16_t id);
  bool isUsedFeatureId(int16_t id);
};

#endif


#ifndef FeatureRepository_h
#define FeatureRepository_h

#include <Feature.h>
#include <PinRepository.h>
#include <FeatureFactory.h>

class FeatureRepository {
public:
  FeatureRepository(PinRepository *pinRepository, FeatureFactory *factory, int16_t maxFeatureCount);
  ~FeatureRepository();

  int16_t createFeature(uint8_t type, int16_t featureId, FeatureSpec spec);
  int16_t deleteFeature(int16_t featureId);

  Feature* getFeature(int16_t featureId);

private:
  PinRepository *pinRepository;
  FeatureFactory *factory;

  int16_t featureCount;
  Feature **features;

  bool isValidFeatureId(int16_t id);
  bool isUsedFeatureId(int16_t id);
};

#endif


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

  /* Interface for retrieving all feature IDs.
   * There is no getFeatureIds() because that would require dynamic memory handling
   * which we want to avoid if it doesn't bring a real benefit.
   */
  int16_t getFeatureIdCount();
  int16_t getFeatureId(int16_t index);

private:
  PinRepository *pinRepository;
  FeatureFactory *factory;

  int16_t maxFeatureCount;
  Feature **features;

  bool isValidFeatureId(int16_t id);
  bool isUsedFeatureId(int16_t id);
};

#endif


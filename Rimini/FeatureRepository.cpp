#include <FeatureRepository.h>

FeatureRepository::FeatureRepository(PinRepository *pinRepository, Factory *factory, uint8_t featureCount) {
  this->pinRepository = pinRepository;
  this->factory = factory;

  this->featureCount = featureCount;
  this->features = new Feature*[featureCount];
}

FeatureRepository::~FeatureRepository() {
  for (uint8_t i = 0; i < featureCount; i++) {
    deleteFeature(i);
  }
  delete features;
}

/* Feature management */

void FeatureRepository::createFeature(uint8_t type, int16_t featureId, FeatureSpec spec) {
  if (pinRepository->areAllPinsUnused(spec.pins, spec.pinCount)) {
    FeatureFactoryMethod new_feature = factory->getFactoryMethod(type);

    if (new_feature != 0) {
      Feature *feature = new_feature(spec); // memory allocation!

      if (feature != 0) {
        features[featureId] = feature;
        pinRepository->registerPinsForFeature(featureId, spec.pins, spec.pinCount);
      }
    }
  }
}

void FeatureRepository::deleteFeature(int16_t featureId) {
  if (isUsedFeatureId(featureId)) {
    Feature *feature = features[featureId];

    delete feature;
    features[featureId] = 0;
    pinRepository->unregisterPinsOfFeature(featureId);
  }
}

Feature* FeatureRepository::getFeature(int16_t featureId) {
  return isUsedFeatureId(featureId) ? features[featureId] : 0;
}

bool FeatureRepository::isUsedFeatureId(int16_t id) {
  return isValidFeatureId(id) && features[id] != 0;
}

bool FeatureRepository::isValidFeatureId(int16_t id) {
  return id >= 0 && id < featureCount;
}


#include <FeatureRepository.h>

FeatureRepository::FeatureRepository(PinRepository *pinRepository, FeatureFactory *factory, int16_t maxFeatureCount) {
  this->pinRepository = pinRepository;
  this->factory = factory;

  this->featureCount = featureCount;
  this->features = new Feature*[maxFeatureCount]();
}

FeatureRepository::~FeatureRepository() {
  for (int16_t featureId = 0; featureId < featureCount; featureId++) {
    deleteFeature(featureId);
  }
  delete features;
}

/* Feature management */

int16_t FeatureRepository::createFeature(uint8_t type, int16_t featureId, FeatureSpec spec) {
  bool somePinsInUse = !pinRepository->areAllPinsUnused(spec.pins, spec.pinCount);
  if (somePinsInUse) {
    return -E_PINS_IN_USE;
  }

  FeatureFactoryMethod new_feature = factory->getFactoryMethod(type);
  if (new_feature == 0) {
    return -E_NO_FEATURE_FACTORY_METHOD;
  }

  Feature *feature = new_feature(spec); // memory allocation!
  if (feature == 0) {
    return -E_CREATE_FEATURE_FAILED;
  }

  features[featureId] = feature;
  pinRepository->registerPinsForFeature(featureId, spec.pins, spec.pinCount);

  return 0;
}

int16_t FeatureRepository::deleteFeature(int16_t featureId) {
  bool unknownFeatureId = isUsedFeatureId(featureId);
  if (unknownFeatureId) {
    return -E_UNKNOWN_FEATURE_ID;
  }

  Feature *feature = features[featureId];

  delete feature;
  features[featureId] = 0;
  pinRepository->unregisterPinsOfFeature(featureId);

  return 0;
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


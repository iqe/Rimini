#include <Repository.h>

Repository::Repository(PinKeeper *pinKeeper, Factory *factory, uint8_t featureCount) {
  this->pinKeeper = pinKeeper;
  this->factory = factory;

  this->featureCount = featureCount;
  this->features = new Feature*[featureCount];
}

Repository::~Repository() {
  for (uint8_t i = 0; i < featureCount; i++) {
    deleteFeature(i);
  }
  delete features;
}

/* Feature management */

void Repository::createFeature(uint8_t type, int16_t id, FeatureSpec spec) {
  if (pinKeeper->areAllPinsUnused(spec.pins, spec.pinCount)) {
    FeatureFactoryMethod new_feature = factory->getFactoryMethod(type);

    if (new_feature != 0) {
      Feature *feature = new_feature(spec); // memory allocation!

      if (feature != 0) {
        features[id] = feature;
        pinKeeper->markPinsUsed(id, spec.pins, spec.pinCount);
      }
    }
  }
}

void Repository::deleteFeature(int16_t id) {
  if (isUsedFeatureId(id)) {
    Feature *feature = features[id];

    delete feature;
    features[id] = 0;
    pinKeeper->markPinsUnused(id);
  }
}

Feature* Repository::getFeature(int16_t id) {
  return isUsedFeatureId(id) ? features[id] : 0;
}

bool Repository::isUsedFeatureId(int16_t id) {
  return isValidFeatureId(id) && features[id] != 0;
}

bool Repository::isValidFeatureId(int16_t id) {
  return id >= 0 && id < featureCount;
}


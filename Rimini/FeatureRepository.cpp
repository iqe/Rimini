#include <FeatureRepository.h>

FeatureRepository::FeatureRepository(PinRepository *pinRepository, FeatureFactory *factory, int16_t maxFeatureCount) {
  this->pinRepository = pinRepository;
  this->factory = factory;

  this->maxFeatureCount = maxFeatureCount;
  this->features = new Feature*[maxFeatureCount]();
}

FeatureRepository::~FeatureRepository() {
  for (int16_t featureId = 0; featureId < maxFeatureCount; featureId++) {
    deleteFeature(featureId);
  }
  delete features;
}

/* Feature management */

int16_t FeatureRepository::createFeature(int16_t featureId, unsigned char *buf, int16_t msgsize) {
  if (msgsize < 3) {
    return -E_MESSAGE_TOO_SMALL;
  }

  // verify feature ID
  if (!isValidFeatureId(featureId)) {
    return -E_INVALID_FEATURE_ID;
  }

  if (isUsedFeatureId(featureId)) {
    return -E_FEATURE_ID_IN_USE;
  }

  // read feature type and pins
  int16_t featureType = buf[0] << 8 | buf[1];
  uint8_t pinCount = buf[2];
  uint8_t *pins = &buf[3];

  // verify pin usage
  bool somePinsUsed = !pinRepository->areAllPinsUnused(pins, pinCount);
  if (somePinsUsed) {
    return -E_PINS_IN_USE;
  }

  // create feature
  FeatureFactoryMethod new_feature = factory->getFactoryMethod(featureType);
  if (new_feature == 0) {
    return -E_NO_FEATURE_FACTORY_METHOD; // FIXME E_UNKNOWN_FEATURE_TYPE
  }

  Feature *feature = new_feature(); // memory allocation!
  if (feature == 0) {
    return -E_CREATE_FEATURE_FAILED;
  }

  int16_t errorcode = feature->readConfig(&buf[2], msgsize - 2);
  if (errorcode < 0) {
    delete feature;
    return errorcode;
  }

  features[featureId] = feature;

  // register used pins
  pinRepository->registerPinsForFeature(featureId, pins, pinCount);

  return 0;
}

int16_t FeatureRepository::updateFeature(int16_t featureId, unsigned char *buf, int16_t msgsize) {
  // verify feature ID
  if (!isUsedFeatureId(featureId)) {
    return -E_UNKNOWN_FEATURE_ID;
  }

  // verify message size
  if (msgsize < 3) {
    return -E_MESSAGE_TOO_SMALL;
  }

  // read feature type and pins
  int16_t featureType = buf[0] << 8 | buf[1];
  uint8_t pinCount = buf[2];
  uint8_t *pins = &buf[3];

  // verify pin usage
  bool onlyOwnPins = !pinRepository->areAllPinsUsedByFeature(featureId, pins, pinCount);
  if (!onlyOwnPins) {
    return -E_CANNOT_CHANGE_PINS;
  }

  // update feature
  Feature *feature = features[featureId];

  return feature->readConfig(&buf[2], msgsize - 2);
}

int16_t FeatureRepository::deleteFeature(int16_t featureId) {
  if (!isUsedFeatureId(featureId)) {
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

/* Feature discovery */

int16_t FeatureRepository::getFeatureIdCount() {
  int16_t count = 0;
  for (int16_t featureId = 0; featureId < maxFeatureCount; featureId++) {
    if (isUsedFeatureId(featureId)) {
      count++;
    }
  }
  return count;
}

int16_t FeatureRepository::getFeatureId(int16_t index) {
  int16_t pos = 0;
  for (int16_t featureId = 0; featureId < maxFeatureCount; featureId++) {
    if (isUsedFeatureId(featureId)) {
      if (pos == index) {
        return featureId;
      }
      pos++;
    }
  }
  return -E_UNKNOWN_FEATURE_ID;
}

bool FeatureRepository::isUsedFeatureId(int16_t id) {
  return isValidFeatureId(id) && features[id] != 0;
}

bool FeatureRepository::isValidFeatureId(int16_t id) {
  return id >= 0 && id < maxFeatureCount;
}


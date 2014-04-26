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

void Repository::createFeature(char *cfgBuf, int16_t cfgBufSize) {
  if (cfgBufSize >= sizeof(FeatureConfigHeader)) {
    FeatureConfigHeader *header = (FeatureConfigHeader*)cfgBuf;

    if (cfgBufSize == sizeof(FeatureConfigHeader) + header->pinCount + header->configSize) {

      if (isValidFeatureId(header->id) && !isUsedFeatureId(header->id)) {

        uint8_t *usedPins = (uint8_t*)(cfgBuf + sizeof(FeatureConfigHeader));
        void *config = (void*)(cfgBuf + sizeof(FeatureConfigHeader) + header->pinCount);

        if (pinKeeper->areAllPinsUnused(usedPins, header->pinCount)) {
          feature_factory_method new_feature = factory->getFactoryMethod(header->mode);

          if (new_feature != 0) {
            FeatureSpec spec;
            spec.pins = usedPins;
            spec.pinCount = header->pinCount;
            spec.config = config;
            spec.configSize = header->configSize;

            Feature *feature = new_feature(spec); // memory allocation!
            if (feature != 0) {
              features[header->id] = feature;
              pinKeeper->markPinsUsed(header->id, usedPins, header->pinCount);
            }
          }
        }
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

bool Repository::isUsedFeatureId(int16_t id) {
  return isValidFeatureId(id) && features[id] != 0;
}

bool Repository::isValidFeatureId(int16_t id) {
  return id >= 0 && id < featureCount;
}


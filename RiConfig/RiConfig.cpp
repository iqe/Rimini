#include <RiConfig.h>

Feature* RiConfig::create(FeatureSpec spec) {
  if (spec.pinCount == 0 && spec.configSize == sizeof(Repository*)) {
    Repository *repo = (Repository*)spec.config;
    return new RiConfig(repo);
  }
  return 0;
}

RiConfig::RiConfig(Repository *repo) {
  this->repo = repo;
}

void RiConfig::readMessage(unsigned char *buf, int16_t msgsize) {
  if (msgsize >= 3) { // action + featureId
    uint8_t action = buf[0];
    int16_t featureId = *((int16_t*)&buf[1]);

    switch (action) {
    case CONFIG_ACTION_CREATE:
      createFeature(featureId, &buf[3], msgsize - 3);
      break;
    case CONFIG_ACTION_DELETE:
      repo->deleteFeature(featureId);
      break;
    }
  }
}

void RiConfig::createFeature(int16_t featureId, unsigned char *buf, int16_t msgsize) {
  if (msgsize >= 3) { // featureType + pinCount + configSize
    uint8_t featureType = buf[0];
    uint8_t pinCount = buf[1];

    if (msgsize >= 3 + pinCount) {
      uint8_t configSize = buf[3 + pinCount];

      uint8_t *pins = (uint8_t*)(&buf[3]);
      void *config = (void*)(buf + 3 + pinCount);

      FeatureSpec spec;
      spec.pinCount = pinCount;
      spec.pins = pins;
      spec.configSize = configSize;
      spec.config = config;

      repo->createFeature(featureType, featureId, spec);
    }
  }
}


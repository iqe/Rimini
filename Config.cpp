#include <Config.h>

Config::Config(Repository *repo) {
  this->repo = repo;
}

void Config::receiveMessage(unsigned char *buf, int16_t msgsize) {
  if (msgsize >= sizeof(FeatureConfigHeader)) {
    FeatureConfigHeader *header = (FeatureConfigHeader*)buf;

    if (msgsize == sizeof(FeatureConfigHeader) + header->pinCount + header->configSize) {

      uint8_t *usedPins = (uint8_t*)(buf + sizeof(FeatureConfigHeader));
      void *config = (void*)(buf + sizeof(FeatureConfigHeader) + header->pinCount);

      FeatureSpec spec;
      spec.pins = usedPins;
      spec.pinCount = header->pinCount;
      spec.config = config;
      spec.configSize = header->configSize;

      repo->createFeature(header->mode, header->id, spec);
    }
  }
}


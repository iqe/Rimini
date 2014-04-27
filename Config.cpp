#include <Config.h>

Feature* Config::create(FeatureSpec spec) {
  if (spec.pinCount == 0 && spec.configSize == sizeof(Repository*)) {
    Repository *repo = (Repository*)spec.config;
    return new Config(repo);
  }
  return 0;
}

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

      repo->createFeature(header->type, header->id, spec);
    }
  }
}


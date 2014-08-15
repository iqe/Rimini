#include <RiConfig.h>

Feature* RiConfig::create(FeatureSpec spec) {
  if (spec.pinCount == 0 && spec.configSize == sizeof(FeatureRepository*)) {
    FeatureRepository *repo = (FeatureRepository*)spec.config;
    return new RiConfig(repo);
  }
  return 0;
}

RiConfig::RiConfig(FeatureRepository *repo) {
  this->repo = repo;
  response = 0;
  errorcode = 0;
}

bool RiConfig::isChanged() {
  return response != 0;
}

void RiConfig::readMessage(unsigned char *buf, int16_t msgsize) {
  if (msgsize >= 1) { // action
    uint8_t action = buf[0];
    int16_t featureId;

    switch (action) {
    case CONFIG_REQ_VERSION:
      errorcode = 0;
      response = CONFIG_RSP_VERSION;
      break;
    case CONFIG_REQ_CREATE:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = createFeature(featureId, &buf[3], msgsize - 3);
        response = CONFIG_RSP_CREATE;
      }
      break;
    case CONFIG_REQ_DELETE:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = repo->deleteFeature(featureId);
        response = CONFIG_RSP_DELETE;
      }
      break;
    case CONFIG_REQ_FEATURES:
      errorcode = 0;
      response = CONFIG_RSP_FEATURES;
      break;
    }
  }
}

int16_t RiConfig::writeMessage(unsigned char *buf, int16_t bufsize) {
  int16_t responseSize = 0;

  switch(response) {
    case CONFIG_RSP_VERSION:
      if (bufsize >= 4) {
        buf[0] = response;
        buf[1] = RIMINI_VERSION_MAJOR;
        buf[2] = RIMINI_VERSION_MINOR;
        buf[3] = RIMINI_VERSION_PATCH;
        responseSize = 4;
      }
      break;
    case CONFIG_RSP_CREATE:
    case CONFIG_RSP_DELETE:
      if (bufsize >= 3) {
        buf[0] = response;
        buf[1] = errorcode >> 8;
        buf[2] = errorcode;
        responseSize = 3;
      }
      break;
    case CONFIG_RSP_FEATURES:
      int16_t featureId;
      int16_t featureCount = repo->getFeatureIdCount();

      if (bufsize >= 3 + featureCount * sizeof(featureCount)) {
        buf[responseSize++] = response;
        buf[responseSize++] = featureCount >> 8;
        buf[responseSize++] = featureCount;

        for (int i = 0; i < featureCount; i++) {
          featureId = repo->getFeatureId(i);

          buf[responseSize++] = featureId >> 8;
          buf[responseSize++] = featureId;
        }
      }
      break;
  }

  response = 0;
  errorcode = 0;

  return responseSize;
}

int16_t RiConfig::createFeature(int16_t featureId, unsigned char *buf, int16_t msgsize) {
  if (msgsize < 3) { // featureType + pinCount + configSize
    return -E_MESSAGE_TOO_SMALL;
  }

  uint8_t featureType = buf[0];
  uint8_t pinCount = buf[1];

  if (msgsize < 3 + pinCount) {
    return -E_MESSAGE_TOO_SMALL;
  }

  uint8_t configSize = buf[3 + pinCount];

  uint8_t *pins = (uint8_t*)(&buf[3]);
  void *config = (void*)(buf + 3 + pinCount);

  FeatureSpec spec;
  spec.pinCount = pinCount;
  spec.pins = pins;
  spec.configSize = configSize;
  spec.config = config;

  return repo->createFeature(featureType, featureId, spec);
}


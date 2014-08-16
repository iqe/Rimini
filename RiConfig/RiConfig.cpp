#include <RiConfig.h>

Feature* RiConfig::create() {
  return new RiConfig();
}

RiConfig::RiConfig() {
  repo = 0;
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
    case CONFIG_REQ_FEATURES:
      errorcode = 0;
      response = CONFIG_RSP_FEATURES;
      break;
    case CONFIG_REQ_CREATE:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = repo->createFeature(featureId, &buf[3], msgsize - 3);
        response = CONFIG_RSP_CREATE;
      }
      break;
    case CONFIG_REQ_READ:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = featureId; // Hack to pass featureId to response
        response = CONFIG_RSP_READ;
      }
      break;
    case CONFIG_REQ_UPDATE:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = repo->updateFeature(featureId, &buf[3], msgsize - 3);
        response = CONFIG_RSP_UPDATE;
      }
      break;
    case CONFIG_REQ_DELETE:
      if (msgsize >= 3) {
        featureId = buf[1] << 8 | buf[2];
        errorcode = repo->deleteFeature(featureId);
        response = CONFIG_RSP_DELETE;
      }
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
    case CONFIG_RSP_UPDATE:
    case CONFIG_RSP_DELETE:
      if (bufsize >= 3) {
        buf[0] = response;
        buf[1] = errorcode >> 8;
        buf[2] = errorcode;
        responseSize = 3;
      }
      break;
    case CONFIG_RSP_READ:
      if (bufsize >= 5) {
        int16_t featureId = errorcode; // set in readMessage()
        Feature *feature = repo->getFeature(featureId);
        errorcode = feature == 0 ? -E_UNKNOWN_FEATURE_ID : 0;

        buf[responseSize++] = response;
        buf[responseSize++] = featureId >> 8;
        buf[responseSize++] = featureId;
        buf[responseSize++] = errorcode >> 8;
        buf[responseSize++] = errorcode;

        if (feature != 0) {
          int16_t count = feature->writeConfig(&buf[responseSize], bufsize - responseSize);
          if (count >= 0) {
              responseSize += count;
          } else {
            // set error code from count
            responseSize -= 2;
            buf[responseSize++] = count >> 8;
            buf[responseSize++] = count;
          }
        }
      }
      break;
    case CONFIG_RSP_FEATURES:
      int16_t featureId;
      int16_t featureCount = repo->getFeatureIdCount();

      if (bufsize >= 3 + featureCount * sizeof(featureId)) {
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

int16_t RiConfig::readConfiguration(unsigned char *buf, int16_t msgsize) {
  // Note: This must only be called during Arduino setup() to bootstrap the system
  if (msgsize < CONFIG_CONF_SIZE) {
    return -E_MESSAGE_TOO_SMALL;
  }
  int p = 1; // start at 1 to skip pin count

  uint32_t repoPointer = buf[p++] >> 24 | buf[p++] >> 16 | buf[p++] >> 8 | buf[p++];
  repo = (FeatureRepository*)repoPointer;

  return p;
}


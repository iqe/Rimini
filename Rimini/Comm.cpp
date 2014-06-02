#include <Comm.h>

Comm::Comm(FeatureRepository *repo) {
  this->repo = repo;
}

void Comm::readMessage(unsigned char *buf, int16_t msgsize) {
  int16_t featureId = readFeatureId(buf, msgsize);

  Feature *feature = repo->getFeature(featureId);
  if (feature != 0) {
    feature->readMessage(buf + sizeof(featureId), msgsize - sizeof(featureId));
  }
}

int16_t Comm::readFeatureId(unsigned char *buf, int16_t msgsize) {
  if (msgsize >= sizeof(int16_t)) {
    return *(int16_t*)buf;
  }
  return -1; // -1 because 0 is used for Config feature
}

int16_t Comm::writeMessageIfChanged(int16_t featureId, unsigned char *buf, int16_t bufsize) {
  Feature *feature = repo->getFeature(featureId);

  if (feature != 0) {
    feature->update();
    if (feature->isChanged()) {
      return writeMessage(featureId, feature, buf, bufsize);
    }
  }
  return 0;
}

int16_t Comm::writeMessage(int16_t featureId, Feature *feature, unsigned char *buf, int16_t bufsize) {
  int msgsize = 0;

  msgsize += writeFeatureId(featureId, buf, bufsize);
  msgsize += feature->writeMessage(buf + msgsize, bufsize - msgsize);

  return msgsize;
}

int16_t Comm::writeFeatureId(int16_t featureId, unsigned char *buf, int16_t bufsize) {
  if (bufsize >= sizeof(featureId)) {
    buf[0] = featureId >> 8;
    buf[1] = featureId;

    return sizeof(featureId);
  }

  return 0;
}


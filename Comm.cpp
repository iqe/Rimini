#include <Comm.h>

Comm::Comm(Repository *repo) {
  this->repo = repo;
}

void Comm::receiveMessage(char *buf, int16_t msgsize) {
  if (msgsize >= 2) { // sizeof(int16_t)
    int *featureId = (int*)buf;
    Feature *feature = repo->getFeature(*featureId);
    if (feature != 0) {
      feature->receiveMessage(&buf[2], msgsize - 2);
    }
  }
}


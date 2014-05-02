#ifndef Comm_h
#define Comm_h

#include <Feature.h>
#include <Repository.h>

class Comm {
public:
  Comm(Repository* repo);

  void readMessage(unsigned char *buf, int16_t msgsize);

  int16_t writeMessageIfChanged(int16_t featureId, unsigned char *buf, int16_t bufsize);

private:
  Repository *repo;

  int16_t writeMessage(int16_t featureId, Feature *feature, unsigned char *buf, int16_t bufsize);

  int16_t readFeatureId(unsigned char *buf, int16_t msgsize);
  int16_t writeFeatureId(int16_t featureId, unsigned char *buf, int16_t bufsize);
};

#endif


#ifndef Comm_h
#define Comm_h

#include <Feature.h>
#include <Repository.h>

class Comm : Feature {
public:
  Comm(Repository* repo);

  void readMessage(unsigned char *buf, int16_t msgsize);

private:
  Repository *repo;
};

#endif


#ifndef RiConfig_h
#define RiConfig_h

#include <Feature.h>
#include <Repository.h>

/*
  RiConfig Message looks like this:

  uint8_t type;        <-- Type of configured feature
  int16_t featureId;   <-- ID of feature that should be configured
  uint8_t pinCount;
  uint8_t pins[pinCount];      <-- Pins used by the configured feature
  uint8_t configSize;
  uint8_t config[configSize];  <-- Feature-specific configuration
  
*/

#define CONFIG_MESSAGE_MIN_SIZE 5 // type + featureId + pinCount + configSize

class RiConfig : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void readMessage(unsigned char *buf, int16_t msgsize);

private:
  Repository *repo;

  RiConfig(Repository *repo);
};

#endif


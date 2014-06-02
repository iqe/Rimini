#ifndef RiConfig_h
#define RiConfig_h

#include <Feature.h>
#include <FeatureRepository.h>

/*
  RiConfig Message looks like this:

  For all actions:
  uint8_t action;      <-- create, delete, ..., see below
  int16_t featureId;   <-- ID of feature that should be configured

  Only for ACTION_CREATE:
  uint8_t featureType; <-- Type of configured feature
  uint8_t pinCount;
  uint8_t pins[pinCount];      <-- Pins used by the configured feature
  uint8_t configSize;
  uint8_t config[configSize];  <-- Feature-specific configuration
*/

#define CONFIG_ACTION_CREATE 1
#define CONFIG_ACTION_DELETE 2

class RiConfig : Feature {
public:
  static Feature* create(FeatureSpec spec);

  void readMessage(unsigned char *buf, int16_t msgsize);

private:
  FeatureRepository *repo;

  RiConfig(FeatureRepository *repo);

  void createFeature(int16_t featureId, unsigned char *buf, int16_t msgsize);
};

#endif


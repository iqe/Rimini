#ifndef RiConfig_h
#define RiConfig_h

#include <Feature.h>
#include <FeatureRepository.h>
#include <Version.h>

/*
  RiConfig Message looks like this:

  ALL ACTIONS:
  uint8_t action;      <-- create, delete, ..., see below

  CONFIG_REQ_VERSION:
  - no more data

  CONFIG_RSP_VERSION:
  uint8_t major;
  uint8_t minor;
  uint8_t patch;

  CONFIG_REQ_CREATE:
  int16_t featureId;   <-- ID of feature that should be created
  uint8_t featureType; <-- Type of created feature
  uint8_t pinCount;
  uint8_t pins[pinCount];      <-- Pins used by the created feature
  uint8_t configSize;
  uint8_t config[configSize];  <-- Feature-specific configuration

  CONFIG_RSP_CREATE:
  int16_t errorcode
*/

#define CONFIG_REQ_VERSION 10
#define CONFIG_RSP_VERSION 11

#define CONFIG_REQ_CREATE 20
#define CONFIG_RSP_CREATE 21

#define CONFIG_REQ_READ 30
#define CONFIG_RSP_READ 31

#define CONFIG_REQ_UPDATE 40
#define CONFIG_RSP_UPDATE 41

#define CONFIG_REQ_DELETE 50
#define CONFIG_RSP_DELETE 51

#define CONFIG_REQ_FEATURES 60
#define CONFIG_RSP_FEATURES 61

#define CONFIG_REQ_TYPES 70
#define CONFIG_RSP_TYPES 71

class RiConfig : Feature {
public:
  static Feature* create(FeatureSpec spec);

  bool isChanged();
  void readMessage(unsigned char *buf, int16_t msgsize);
  int16_t writeMessage(unsigned char *buf, int16_t bufsize);

private:
  FeatureRepository *repo;
  uint8_t response;
  int16_t errorcode;

  RiConfig(FeatureRepository *repo);

  int16_t createFeature(int16_t featureId, unsigned char *buf, int16_t msgsize);
};

#endif


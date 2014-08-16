#ifndef RiConfig_h
#define RiConfig_h

#include <Feature.h>
#include <FeatureRepository.h>
#include <Version.h>

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

#define CONFIG_CONF_SIZE (sizeof(uint8_t) + sizeof(uint32_t))

class RiConfig : Feature {
public:
  static Feature* create();

  bool isChanged();
  void readMessage(unsigned char *buf, int16_t msgsize);
  int16_t writeMessage(unsigned char *buf, int16_t bufsize);

  int16_t readConfig(unsigned char *buf, int16_t msgsize);

private:
  FeatureRepository *repo;
  uint8_t response;
  int16_t errorcode;

  RiConfig();
};

#endif


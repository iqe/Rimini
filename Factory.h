#ifndef Factory_h
#define Factory_h

#include <Feature.h>

class Factory {
public:
  Factory(uint8_t methodCount);
  ~Factory();

  feature_factory_method getFactoryMethod(uint8_t mode);
  void setFactoryMethod(uint8_t mode, feature_factory_method method);

private:
  uint8_t methodCount;
  feature_factory_method *methods;

  bool isUsedMode(uint8_t mode);
  bool isValidMode(uint8_t mode);
};

#endif


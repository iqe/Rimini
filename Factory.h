#ifndef Factory_h
#define Factory_h

#include <Feature.h>

class Factory {
public:
  Factory(uint8_t methodCount);
  ~Factory();

  feature_factory_method getFactoryMethod(uint8_t type);
  void setFactoryMethod(uint8_t type, feature_factory_method method);

private:
  uint8_t methodCount;
  feature_factory_method *methods;

  bool isUsedType(uint8_t type);
  bool isValidType(uint8_t type);
};

#endif


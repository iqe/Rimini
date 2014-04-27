#ifndef Factory_h
#define Factory_h

#include <Feature.h>

class Factory {
public:
  Factory(uint8_t methodCount);
  ~Factory();

  FeatureFactoryMethod getFactoryMethod(uint8_t type);
  void setFactoryMethod(uint8_t type, FeatureFactoryMethod method);

private:
  uint8_t methodCount;
  FeatureFactoryMethod *methods;

  bool isUsedType(uint8_t type);
  bool isValidType(uint8_t type);
};

#endif


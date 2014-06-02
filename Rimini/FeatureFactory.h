#ifndef FeatureFactory_h
#define FeatureFactory_h

#include <Feature.h>

class FeatureFactory {
public:
  FeatureFactory(uint8_t methodCount);
  ~FeatureFactory();

  FeatureFactoryMethod getFactoryMethod(uint8_t type);
  void setFactoryMethod(uint8_t type, FeatureFactoryMethod method);

private:
  uint8_t methodCount;
  FeatureFactoryMethod *methods;

  bool isUsedType(uint8_t type);
  bool isValidType(uint8_t type);
};

#endif


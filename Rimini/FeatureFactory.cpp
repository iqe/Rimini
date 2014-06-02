#include <FeatureFactory.h>

FeatureFactory::FeatureFactory(uint8_t methodCount) {
  this->methodCount = methodCount;

  methods = new FeatureFactoryMethod[methodCount] {};
}

FeatureFactory::~FeatureFactory() {
  delete methods;
}

FeatureFactoryMethod FeatureFactory::getFactoryMethod(uint8_t type) {
  return isUsedType(type) ? methods[type] : 0;
}

void FeatureFactory::setFactoryMethod(uint8_t type, FeatureFactoryMethod method) {
  if (isValidType(type)) {
    methods[type] = method;
  }
}

bool FeatureFactory::isUsedType(uint8_t type) {
  return isValidType(type) && methods[type] != 0;
}

bool FeatureFactory::isValidType(uint8_t type) {
  return type < methodCount;
}


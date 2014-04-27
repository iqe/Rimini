#include <Factory.h>

Factory::Factory(uint8_t methodCount) {
  this->methodCount = methodCount;

  methods = new FeatureFactoryMethod[methodCount] {};
}

Factory::~Factory() {
  delete methods;
}

FeatureFactoryMethod Factory::getFactoryMethod(uint8_t type) {
  return isUsedType(type) ? methods[type] : 0;
}

void Factory::setFactoryMethod(uint8_t type, FeatureFactoryMethod method) {
  if (isValidType(type)) {
    methods[type] = method;
  }
}

bool Factory::isUsedType(uint8_t type) {
  return isValidType(type) && methods[type] != 0;
}

bool Factory::isValidType(uint8_t type) {
  return type < methodCount;
}


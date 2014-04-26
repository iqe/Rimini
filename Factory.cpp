#include <Factory.h>

Factory::Factory(uint8_t methodCount) {
  this->methodCount = methodCount;

  methods = new feature_factory_method[methodCount] {};
}

Factory::~Factory() {
  delete methods;
}

feature_factory_method Factory::getFactoryMethod(uint8_t mode) {
  return isUsedMode(mode) ? methods[mode] : 0;
}

void Factory::setFactoryMethod(uint8_t mode, feature_factory_method method) {
  if (isValidMode(mode)) {
    methods[mode] = method;
  }
}

bool Factory::isUsedMode(uint8_t mode) {
  return isValidMode(mode) && methods[mode] != 0;
}

bool Factory::isValidMode(uint8_t mode) {
  return mode < methodCount;
}


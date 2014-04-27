#include <Factory.h>

Factory::Factory(uint8_t methodCount) {
  this->methodCount = methodCount;

  methods = new feature_factory_method[methodCount] {};
}

Factory::~Factory() {
  delete methods;
}

feature_factory_method Factory::getFactoryMethod(uint8_t type) {
  return isUsedType(type) ? methods[type] : 0;
}

void Factory::setFactoryMethod(uint8_t type, feature_factory_method method) {
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


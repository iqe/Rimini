#include <PinRepository.h>

PinRepository::PinRepository(uint8_t maxPinCount) {
  this->usedPinCount = maxPinCount;
  usedPins = new int16_t[maxPinCount]();
}

PinRepository::~PinRepository() {
  delete usedPins;
}

bool PinRepository::areAllPinsUnused(uint8_t *pins, uint8_t pinCount) {
  return areAllPinsUsedByFeature(0, pins, pinCount);
}

bool PinRepository::areAllPinsUsedByFeature(int16_t featureId, uint8_t *pins, uint8_t pinCount) {
  for (int i = 0; i < pinCount; i++) {
    if (!isValidPin(pins[i]) || usedPins[i] != featureId) {
      return false;
    }
  }
  return true;
}

void PinRepository::registerPinsForFeature(int16_t featureId, uint8_t *pins, uint8_t pinCount) {
  for (int i = 0; i < pinCount; i++) {
    uint8_t pin = pins[i];

    if (isValidPin(pin)) {
      usedPins[pin] = featureId;
    }
  }
}

void PinRepository::unregisterPinsOfFeature(int16_t featureId) {
  for (int i = 0; i < usedPinCount; i++) {
    if (usedPins[i] == featureId) {
      usedPins[i] = 0;
    }
  }
}

bool PinRepository::isValidPin(uint8_t pin) {
  return pin >= 0 && pin < usedPinCount;
}


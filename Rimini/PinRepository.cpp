#include <PinRepository.h>

PinRepository::PinRepository(uint8_t pinCount) {
  this->pinCount = pinCount;
  pins = new int16_t[pinCount];
}

PinRepository::~PinRepository() {
  delete pins;
}

bool PinRepository::areAllPinsUnused(uint8_t *checkedPins, uint8_t checkedPinsCount) {
  for (int i = 0; i < checkedPinsCount; i++) {
    uint8_t pin = checkedPins[i];

    if (isValidPin(pin) && pins[pin] != 0) {
      return false;
    }
  }
  return true;
}

void PinRepository::markPinsUsed(int16_t marker, uint8_t *usedPins, uint8_t usedPinsCount) {
  for (int i = 0; i < usedPinsCount; i++) {
    uint8_t pin = usedPins[i];

    if (isValidPin(pin)) {
      pins[pin] = marker;
    }
  }
}

void PinRepository::markPinsUnused(int16_t marker) {
  for (int i = 0; i < pinCount; i++) {
    if (pins[i] == marker) {
      pins[i] = 0;
    }
  }
}

bool PinRepository::isValidPin(uint8_t pin) {
  return pin >= 0 && pin < pinCount;
}


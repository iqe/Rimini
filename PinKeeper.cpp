#include <PinKeeper.h>

PinKeeper::PinKeeper(uint8_t pinCount) {
  this->pinCount = pinCount;
  pins = new int16_t[pinCount];
}

PinKeeper::~PinKeeper() {
  delete pins;
}

bool PinKeeper::areAllPinsUnused(uint8_t *checkedPins, uint8_t checkedPinsCount) {
  for (int i = 0; i < checkedPinsCount; i++) {
    uint8_t pin = checkedPins[i];

    if (isValidPin(pin) && pins[pin] != 0) {
      return false;
    }
  }
  return true;
}

void PinKeeper::markPinsUsed(int16_t marker, uint8_t *usedPins, uint8_t usedPinsCount) {
  for (int i = 0; i < usedPinsCount; i++) {
    uint8_t pin = usedPins[i];

    if (isValidPin(pin)) {
      pins[pin] = marker;
    }
  }
}

void PinKeeper::markPinsUnused(int16_t marker) {
  for (int i = 0; i < pinCount; i++) {
    if (pins[i] == marker) {
      pins[i] = 0;
    }
  }
}

bool PinKeeper::isValidPin(uint8_t pin) {
  return pin >= 0 && pin < pinCount;
}


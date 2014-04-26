#ifndef PinKeeper_h
#define PinKeeper_h

#include <inttypes.h>

class PinKeeper {

public:
  PinKeeper(uint8_t pinCount);
  ~PinKeeper();

  bool areAllPinsUnused(uint8_t *checkedPins, uint8_t checkedPinCount);

  void markPinsUsed(int16_t marker, uint8_t *usedPins, uint8_t usedPinCount);
  void markPinsUnused(int16_t marker);

private:
  uint8_t pinCount; // up to 256 pins
  int16_t *pins; // pins[5] = 42 -> pin 5, marker 42

  bool isValidPin(uint8_t pin);
};

#endif


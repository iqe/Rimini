#ifndef PinRepository_h
#define PinRepository_h

#include <inttypes.h>

class PinRepository {

public:
  PinRepository(uint8_t maxPinCount);
  ~PinRepository();

  bool areAllPinsUnused(uint8_t *pins, uint8_t pinCount);
  bool areAllPinsUsedByFeature(int16_t featureId, uint8_t *pins, uint8_t pinCount);

  void registerPinsForFeature(int16_t featureId, uint8_t *pins, uint8_t pinCount);
  void unregisterPinsOfFeature(int16_t featureId);

private:
  uint8_t usedPinCount; // up to 256 pins
  int16_t *usedPins; // usedPins[5] = 42 -> pin 5 is used by feature with ID 42

  bool isValidPin(uint8_t pin);
};

#endif


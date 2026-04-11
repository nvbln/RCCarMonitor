#pragma once

#include "interfaces/IGattCharacteristicHandler.h"
#include <gmock/gmock.h>

class MockGattCharacteristicHandler : public IGattCharacteristicHandler {
public:
  MOCK_METHOD(bool, isAvailable, (), (const, override));
  MOCK_METHOD(void, onAvailable, (OnAvailableCallback), (override));
  MOCK_METHOD(void, onUnavailable, (OnUnavailableCallback), (override));
  MOCK_METHOD(std::optional<std::vector<uint8_t>>, read, (), (override));
  MOCK_METHOD(bool, write, (const std::vector<uint8_t>&), (override));
};

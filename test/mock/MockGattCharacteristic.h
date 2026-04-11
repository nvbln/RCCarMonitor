#pragma once

#include "interfaces/IGattCharacteristic.h"

#include <gmock/gmock.h>

class MockGattCharacteristic : public IGattCharacteristic {
public:
  MOCK_METHOD(std::string, uuid, (), (override));
  MOCK_METHOD(std::vector<uint8_t>, read, (), (override));
  MOCK_METHOD(void, write, (const std::vector<uint8_t>& value), (override));
};

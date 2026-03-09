#pragma once

#include "proxies/IDBusCharacteristicProxy.h"
#include <gmock/gmock.h>

#include <string>
#include <vector>

class MockDBusCharacteristicProxy : public IDBusCharacteristicProxy {
public:
  MOCK_METHOD(std::string, objectPath, (), (override));
  MOCK_METHOD(std::string, uuid, (), (override));
  MOCK_METHOD(std::vector<uint8_t>, read, (), (override));
  MOCK_METHOD(void, write, (const std::vector<uint8_t> &value), (override));
};

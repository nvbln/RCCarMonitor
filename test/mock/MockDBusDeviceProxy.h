#pragma once

#include "proxies/IDBusDeviceProxy.h"
#include <gmock/gmock.h>

class MockDBusDeviceProxy : public IDBusDeviceProxy {
public:
  MOCK_METHOD(void, connect, (), (override));
  MOCK_METHOD(void, disconnect, (), (override));
  MOCK_METHOD(bool, isConnected, (), (override));
  MOCK_METHOD(std::string, name, (), (override));
  MOCK_METHOD(std::string, address, (), (override));
};

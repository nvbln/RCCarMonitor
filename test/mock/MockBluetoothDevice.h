#pragma once

#include "interfaces/IBluetoothDevice.h"
#include <gmock/gmock.h>

class MockBluetoothDevice : public IBluetoothDevice {
public:
  MOCK_METHOD(void, connect, (), (override));
  MOCK_METHOD(void, disconnect, (), (override));
  MOCK_METHOD(bool, isConnected, (), (override));
  MOCK_METHOD(std::string, name, (), (override));
  MOCK_METHOD(std::string, address, (), (override));
  MOCK_METHOD(std::optional<std::shared_ptr<IGattCharacteristic>>, findCharacteristic,
              (std::string uuid), (override));
  MOCK_METHOD(void, subscribeToAddCharacteristic, (Callback callback), (override));
  MOCK_METHOD(void, subscribeToRemoveCharacteristic, (Callback callback), (override));
};

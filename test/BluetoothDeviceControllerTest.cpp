#include "controllers/BluetoothDeviceController.h"
#include "IBluetoothDevice.h"
#include "IBluetoothManager.h"
#include "views/IItemPickerView.h"

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Return;

class MockItemPickerView : public IItemPickerView {
public:
  MOCK_METHOD(void, draw, (), (override));
  MOCK_METHOD(void, update, (const std::vector<std::string> labels), (override));
  MOCK_METHOD(void, subscribe, (Callback callback), (override));
};

class MockBluetoothManager : public IBluetoothManager {
public:
  MOCK_METHOD(std::vector<std::shared_ptr<IBluetoothDevice>>, getDevices, (), (const, override));
  MOCK_METHOD(std::optional<std::shared_ptr<IBluetoothDevice>>, findDevice,
              (std::string deviceName), (const, override));
};

class MockBluetoothDevice : public IBluetoothDevice {
public:
  MOCK_METHOD(void, connect, (), (override));
  MOCK_METHOD(void, disconnect, (), (override));
  MOCK_METHOD(bool, isConnected, (), (override));
  MOCK_METHOD(std::string, name, (), (override));
  MOCK_METHOD(std::string, address, (), (override));
  MOCK_METHOD(std::optional<std::shared_ptr<IGattCharacteristic>>, findCharacteristic,
              (std::string uuid), (override));
};

TEST(BluetoothDeviceControllerTests, whenItemSelectedInViewCallCallback) {
  auto mockItemPicker = std::make_shared<MockItemPickerView>();
  IItemPickerView::Callback callback;
  ON_CALL(*mockItemPicker, subscribe).WillByDefault([&](IItemPickerView::Callback cb) {
    callback = cb;
  });

  auto mockBluetoothDevice = std::make_shared<MockBluetoothDevice>();
  ON_CALL(*mockBluetoothDevice, name).WillByDefault(Return("TestDevice"));

  auto mockBluetoothManager = std::make_shared<MockBluetoothManager>();
  auto mockDevices = std::vector<std::shared_ptr<IBluetoothDevice>>{mockBluetoothDevice};
  ON_CALL(*mockBluetoothManager, getDevices).WillByDefault(Return(mockDevices));

  auto controller = BluetoothDeviceController(mockBluetoothManager, mockItemPicker);
  controller.subscribe([mockBluetoothDevice](std::shared_ptr<IBluetoothDevice> device) {
    EXPECT_EQ(mockBluetoothDevice, device);
  });

  callback(0);
}

#include "controllers/BluetoothDeviceController.h"
#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IBluetoothManager.h"
#include "views/IItemPickerView.h"

#include "mock/MockBluetoothDevice.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;
using ::testing::Return;

class MockItemPickerView : public IItemPickerView {
public:
  MOCK_METHOD(void, draw, (), (override));
  MOCK_METHOD(void, update, (const std::vector<std::string> labels), (override));
  MOCK_METHOD(void, subscribe, (Callback callback), (override));
  MOCK_METHOD(void, enable, (), (override));
  MOCK_METHOD(void, disable, (), (override));
};

class MockBluetoothManager : public IBluetoothManager {
public:
  MOCK_METHOD(std::vector<IBluetoothAdapter*>, getAdapters, (), (const, override));
  MOCK_METHOD(std::vector<IBluetoothDevice*>, getDevices, (), (const, override));
  MOCK_METHOD(std::optional<IBluetoothDevice*>, findDevice, (std::string deviceName),
              (const, override));
};

TEST(BluetoothDeviceControllerTests, whenItemSelectedInViewCallCallback) {
  // auto mockItemPicker = NiceMock<MockItemPickerView>();
  NiceMock<MockItemPickerView> mockItemPicker;
  IItemPickerView::Callback callback;
  ON_CALL(mockItemPicker, subscribe).WillByDefault([&](IItemPickerView::Callback cb) {
    callback = cb;
  });

  // auto mockBluetoothDevice = NiceMock<MockBluetoothDevice>();
  NiceMock<MockBluetoothDevice> mockBluetoothDevice;
  ON_CALL(mockBluetoothDevice, name).WillByDefault(Return("TestDevice"));

  auto mockBluetoothManager = NiceMock<MockBluetoothManager>();
  auto mockDevices = std::vector<IBluetoothDevice*>{&mockBluetoothDevice};
  ON_CALL(mockBluetoothManager, getDevices).WillByDefault(Return(mockDevices));

  auto controller = BluetoothDeviceController(&mockBluetoothManager, &mockItemPicker);
  controller.subscribe([&mockBluetoothDevice](IBluetoothDevice* device) {
    EXPECT_EQ(&mockBluetoothDevice, device);
  });

  callback(0);
}

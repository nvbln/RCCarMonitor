#include "DBusBluetoothManager.h"
#include "IDBusObjectManagerProxy.h"

#include "mock/MockDBusDeviceProxy.h"

#include <sdbus-c++/sdbus-c++.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using InterfacesAndPropertiesMap = IDBusObjectManagerProxy::InterfacesAndPropertiesMap;

using ::testing::NiceMock;
using ::testing::Return;

class MockDBusObjectManagerProxy : public IDBusObjectManagerProxy {
public:
  MOCK_METHOD(void, addOnInterfacesAddedCallback, (const OnInterfacesAddedCallback callback),
              (override));
  MOCK_METHOD(void, addOnInterfacesRemovedCallback, (const OnInterfacesRemovedCallback callback),
              (override));
  MOCK_METHOD(std::shared_ptr<IDBusDeviceProxy>, createDevice, (std::string objectPath),
              (override));
  MOCK_METHOD(std::shared_ptr<IDBusCharacteristicProxy>, createCharacteristic,
              (std::string objectPath), (override));
  MOCK_METHOD((std::map<sdbus::ObjectPath, InterfacesAndPropertiesMap>), managedObjects, (),
              (override));
};

TEST(DBusBluetoothManagerTests, shouldGetAndFindDevices) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusObjectManagerProxy>>();

  // Save callbacks such that it can be triggered from the test.
  IDBusObjectManagerProxy::OnInterfacesAddedCallback addedCallback;
  IDBusObjectManagerProxy::OnInterfacesRemovedCallback removedCallback;
  EXPECT_CALL(*mockProxy, addOnInterfacesAddedCallback)
      .WillOnce(::testing::SaveArg<0>(&addedCallback));
  EXPECT_CALL(*mockProxy, addOnInterfacesRemovedCallback)
      .WillOnce(::testing::SaveArg<0>(&removedCallback));

  auto bluetoothManager = std::make_shared<DBusBluetoothManager>(mockProxy);

  // Create a fake device, including a fake proxy, that can give the
  // address back when requested (for finding devices).
  auto deviceInterface = sdbus::InterfaceName{"org.bluez.Device1"};
  auto fakeDeviceAddress = "FE:D5:17:D7:DC:B8";
  auto fakeDeviceName = "Arduino device";
  auto fakeDevicePath = sdbus::ObjectPath{"/org/bluez/hci0/dev_FE_D5_17_D7_DC_B8"};
  InterfacesAndPropertiesMap fakeDeviceProperties = {
      {deviceInterface,
       {{sdbus::PropertyName{"Address"}, sdbus::Variant{fakeDeviceAddress}},
        {sdbus::PropertyName{"Name"}, sdbus::Variant{fakeDeviceName}},
        {sdbus::PropertyName{"Alias"}, sdbus::Variant{fakeDeviceName}},
        {sdbus::PropertyName{"Paired"}, sdbus::Variant{false}},
        {sdbus::PropertyName{"UUIDs"},
         sdbus::Variant{std::vector<std::string>{"00001800-0000-1000-8000-00805f9b34fb"}}}}}};

  auto mockDeviceProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  ON_CALL(*mockDeviceProxy, address).WillByDefault(Return(fakeDeviceAddress));
  ON_CALL(*mockDeviceProxy, name).WillByDefault(Return(fakeDeviceName));
  ON_CALL(*mockProxy, createDevice).WillByDefault(Return(mockDeviceProxy));

  // Test the code
  addedCallback(fakeDevicePath, fakeDeviceProperties);
  EXPECT_EQ(1, bluetoothManager->getDevices().size());
  EXPECT_TRUE(bluetoothManager->findDevice(fakeDeviceName));
  removedCallback(fakeDevicePath, std::vector{deviceInterface});
  EXPECT_EQ(0, bluetoothManager->getDevices().size());
}

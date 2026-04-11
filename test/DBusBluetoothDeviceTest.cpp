#include "DBusBluetoothDevice.h"

#include "mock/MockDBusDeviceProxy.h"
#include "mock/MockDBusPropertiesProxy.h"
#include "proxies/IDBusPropertiesProxy.h"

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;
using ::testing::Return;

class MockDBusCharacteristicProxy : public IDBusCharacteristicProxy {
public:
  MOCK_METHOD(std::string, objectPath, (), (override));
  MOCK_METHOD(std::string, uuid, (), (override));
  MOCK_METHOD(std::vector<uint8_t>, read, (), (override));
  MOCK_METHOD(void, write, (const std::vector<uint8_t>& value), (override));
};

TEST(DBusBluetoothDeviceTests, shouldConnectDeviceOnConnect) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();
  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);
  EXPECT_CALL(*mockProxy, connect());

  bluetoothDevice->connect();
}

TEST(DBusBluetoothDeviceTests, shouldConnectDeviceOnDisconnect) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();
  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);
  EXPECT_CALL(*mockProxy, disconnect());

  bluetoothDevice->disconnect();
}

TEST(DBusBluetoothDeviceTests, shouldReturnCorrectBoolForIsConnected) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();
  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);

  EXPECT_CALL(*mockProxy, isConnected()).Times(1).WillOnce(Return(true));
  EXPECT_TRUE(bluetoothDevice->isConnected());

  EXPECT_CALL(*mockProxy, isConnected()).Times(1).WillOnce(Return(false));
  EXPECT_FALSE(bluetoothDevice->isConnected());
}

TEST(DBusBluetoothDeviceTests, shouldReturnDeviceNameUponRequest) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();

  EXPECT_CALL(*mockProxy, name()).Times(1).WillOnce(Return("Test"));
  IDBusPropertiesProxy::OnPropertiesChangedCallback callback;
  EXPECT_CALL(*mockPropertiesProxy, subscribeToOnPropertiesChanged)
      .WillOnce(::testing::SaveArg<0>(&callback));

  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);

  EXPECT_EQ("Test", bluetoothDevice->name());

  std::map<sdbus::MemberName, sdbus::Variant> properties = {
      std::make_pair(sdbus::MemberName{"Name"}, sdbus::Variant{"Test2"})};
  callback(sdbus::InterfaceName{"org.bluez.Device1"}, properties, std::vector<sdbus::MemberName>{});
  EXPECT_EQ("Test2", bluetoothDevice->name());
}

TEST(DBusBluetoothDeviceTests, shouldReturnDeviceAddressUponRequest) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();

  EXPECT_CALL(*mockProxy, address()).Times(1).WillOnce(Return("00:1A:2B:3C:4D:5E"));
  IDBusPropertiesProxy::OnPropertiesChangedCallback callback;
  EXPECT_CALL(*mockPropertiesProxy, subscribeToOnPropertiesChanged)
      .WillOnce(::testing::SaveArg<0>(&callback));

  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);

  EXPECT_EQ("00:1A:2B:3C:4D:5E", bluetoothDevice->address());

  std::map<sdbus::MemberName, sdbus::Variant> properties = {
      std::make_pair(sdbus::MemberName{"Address"}, sdbus::Variant{"00:1B:2C:3D:4E:5F"})};
  callback(sdbus::InterfaceName{"org.bluez.Device1"}, properties, std::vector<sdbus::MemberName>{});
  EXPECT_EQ("00:1B:2C:3D:4E:5F", bluetoothDevice->address());
}

TEST(DBusBluetoothDeviceTests, shouldFindCharacteristic) {
  auto mockProxy = std::make_shared<NiceMock<MockDBusDeviceProxy>>();
  auto mockPropertiesProxy = std::make_shared<NiceMock<MockDBusPropertiesProxy>>();
  auto bluetoothDevice = std::make_unique<DBusBluetoothDevice>(mockProxy, mockPropertiesProxy);

  auto mockCharacteristicProxy1 = std::make_shared<NiceMock<MockDBusCharacteristicProxy>>();
  auto mockCharacteristicProxy2 = std::make_shared<NiceMock<MockDBusCharacteristicProxy>>();
  auto mockCharacteristicProxy3 = std::make_shared<NiceMock<MockDBusCharacteristicProxy>>();

  ON_CALL(*mockCharacteristicProxy1, uuid()).WillByDefault(Return("1"));
  ON_CALL(*mockCharacteristicProxy2, uuid()).WillByDefault(Return("2"));
  ON_CALL(*mockCharacteristicProxy3, uuid()).WillByDefault(Return("3"));

  auto characteristic1 = std::make_shared<DBusGattCharacteristic>(mockCharacteristicProxy1);
  auto characteristic2 = std::make_shared<DBusGattCharacteristic>(mockCharacteristicProxy2);
  auto characteristic3 = std::make_shared<DBusGattCharacteristic>(mockCharacteristicProxy3);

  bluetoothDevice->addCharacteristic(characteristic1);
  bluetoothDevice->addCharacteristic(characteristic2);
  bluetoothDevice->addCharacteristic(characteristic3);

  // Regular check if correct characteristic is returned.
  EXPECT_EQ(characteristic2, bluetoothDevice->findCharacteristic("2").value_or(nullptr));

  // Check if uuids that do not exist do not return anything.
  EXPECT_EQ(nullptr, bluetoothDevice->findCharacteristic("5").value_or(nullptr));

  // Check if removing a characteristic works.
  bluetoothDevice->removeCharacteristic(characteristic2);
  EXPECT_EQ(nullptr, bluetoothDevice->findCharacteristic("2").value_or(nullptr));
}

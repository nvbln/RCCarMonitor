#include <cctype>
#include <sdbus-c++/sdbus-c++.h>

#include "DBusBluetoothDevice.h"

void DBusBluetoothDevice::addCharacteristic(
    std::unique_ptr<DBusGattCharacteristic> characteristic) {
  mCharacteristics.push_back(std::move(characteristic));
}

void DBusBluetoothDevice::removeCharacteristic(DBusGattCharacteristic* characteristic) {
  mCharacteristics.erase(
      std::remove_if(mCharacteristics.begin(), mCharacteristics.end(),
                     [characteristic](const std::unique_ptr<DBusGattCharacteristic>& p) {
                       return p.get() == characteristic;
                     }),
      mCharacteristics.end());
}

std::optional<IGattCharacteristic*> DBusBluetoothDevice::findCharacteristic(std::string uuid) {
  auto characteristic = this->findDBusCharacteristic(uuid);
  if (characteristic) {
    return characteristic;
  }

  return std::nullopt;
}

std::optional<DBusGattCharacteristic*>
DBusBluetoothDevice::findDBusCharacteristic(std::string value, std::string property) {
  auto iterator = std::find_if(
      mCharacteristics.begin(), mCharacteristics.end(), [&](const auto& characteristic) {
        if (property == "uuid") {
          std::ranges::transform(value, value.begin(),
                                 [](unsigned char c) { return std::tolower(c); });
          return characteristic->uuid() == value;
        } else {
          return characteristic->objectPath() == value;
        }
      });

  if (iterator == mCharacteristics.end()) {
    return std::nullopt;
  }

  return (*iterator).get();
}

void DBusBluetoothDevice::onPropertiesChanged(
    const sdbus::InterfaceName& interface,
    const std::map<sdbus::MemberName, sdbus::Variant>& changedProperties,
    const std::vector<sdbus::MemberName>& invalidatedProperties) {

  // Sanity check such that only property changes for the device are processed.
  if (interface == "org.bluez.Device1") {
    for (const auto& [name, value] : changedProperties) {
      if (name == "Name") {
        mName = value.get<std::string>();
      } else if (name == "Address") {
        mAddress = value.get<std::string>();
      }
    }
  }
}

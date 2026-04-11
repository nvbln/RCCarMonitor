#include <cctype>
#include <sdbus-c++/sdbus-c++.h>

#include "DBusBluetoothDevice.h"

void DBusBluetoothDevice::addCharacteristic(
    std::shared_ptr<DBusGattCharacteristic> characteristic) {
  mCharacteristics.push_back(characteristic);
}

void DBusBluetoothDevice::removeCharacteristic(
    std::shared_ptr<DBusGattCharacteristic> characteristic) {
  mCharacteristics.erase(
      std::remove(mCharacteristics.begin(), mCharacteristics.end(), characteristic),
      mCharacteristics.end());
}

std::optional<std::shared_ptr<IGattCharacteristic>>
DBusBluetoothDevice::findCharacteristic(std::string uuid) {
  auto characteristic = this->findDBusCharacteristic(uuid);
  if (characteristic) {
    return std::static_pointer_cast<IGattCharacteristic>(*characteristic);
  }

  return std::nullopt;
}

std::optional<std::shared_ptr<DBusGattCharacteristic>>
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

  return *iterator;
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

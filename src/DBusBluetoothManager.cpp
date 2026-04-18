#include "DBusBluetoothManager.h"
#include "proxies/IDBusObjectManagerProxy.h"

#include <iostream>
#include <optional>
#include <sdbus-c++/sdbus-c++.h>

using InterfacesAndPropertiesMap = IDBusObjectManagerProxy::InterfacesAndPropertiesMap;

DBusBluetoothManager::DBusBluetoothManager(std::unique_ptr<IDBusObjectManagerProxy> proxy)
    : mProxy(std::move(proxy)) {
  mProxy->addOnInterfacesAddedCallback(
      [this](const sdbus::ObjectPath& objectPath,
             const InterfacesAndPropertiesMap& interfacesAndProperties) {
        this->onInterfacesAdded(objectPath, interfacesAndProperties);
      });

  mProxy->addOnInterfacesRemovedCallback(
      [this](const sdbus::ObjectPath& objectPath,
             const std::vector<sdbus::InterfaceName>& interfaces) {
        this->onInterfacesRemoved(objectPath, interfaces);
      });

  handleExistingObjects();
}

void DBusBluetoothManager::handleExistingObjects() {
  auto managedObjects = mProxy->managedObjects();
  for (const auto& [objectPath, interfacesAndPropertiesMap] : managedObjects) {
    onInterfacesAdded(objectPath, interfacesAndPropertiesMap);
  }
}

void DBusBluetoothManager::onInterfacesAdded(
    const sdbus::ObjectPath& objectPath,
    const InterfacesAndPropertiesMap& interfacesAndProperties) {

  // Filter on Bluetooth devices.
  for (const auto& [interface, properties] : interfacesAndProperties) {
    if (interface == "org.bluez.Adapter1") {
      mAdapters.push_back(
          std::make_unique<DBusBluetoothAdapter>(mProxy->createAdapter(objectPath)));
    }

    // Filter on devices that are currently available (have a receive signal strength property)
    if (interface == "org.bluez.Device1" && properties.count(sdbus::MemberName{"RSSI"}) > 0) {
      mDevices.push_back(std::make_unique<DBusBluetoothDevice>(
          mProxy->createDevice(objectPath), mProxy->createProperties(objectPath)));
      auto aliasProperty = properties.find(sdbus::PropertyName{"Alias"});
      if (aliasProperty != properties.end()) {
        std::cout << "Device with alias: ";
        std::cout << properties.at(sdbus::PropertyName{"Alias"}).get<std::string>();
        std::cout << " was added." << std::endl;
      }
    } else if (interface == "org.bluez.GattCharacteristic1") {
      std::cout << "Added characteristic: " << objectPath << std::endl;
      std::string address = this->extractDeviceAddressFromObjectPath(objectPath);
      auto optDevice = findDBusDevice(address, "address");

      if (optDevice) {
        auto device = *optDevice;
        auto dbusChar =
            std::make_unique<DBusGattCharacteristic>(mProxy->createCharacteristic(objectPath));
        device->addCharacteristic(std::move(dbusChar));
      } else {
        std::cout << "Couldn't find device: " << address << std::endl;
      }
    }
  }
}

void DBusBluetoothManager::onInterfacesRemoved(
    const sdbus::ObjectPath& objectPath, const std::vector<sdbus::InterfaceName>& interfaces) {
  std::cout << "removed: " << objectPath << std::endl;
  for (const auto& interface : interfaces) {
    // Extract device address
    std::string address = this->extractDeviceAddressFromObjectPath(objectPath);
    auto devicePtr = findDBusDevice(address, "address");

    if (interface == "org.bluez.Device1") {
      if (devicePtr) {
        removeDevice(devicePtr.value());
        std::cout << "Device " << (*devicePtr)->name() << " removed." << std::endl;
      }
    } else if (interface == "org.bluez.GattCharacteristic1") {
      if (!devicePtr) {
        continue;
      }

      auto device = *devicePtr;
      auto characteristicPtr = device->findDBusCharacteristic(objectPath, "path");
      if (!characteristicPtr) {
        continue;
      }
      device->removeCharacteristic(*characteristicPtr);
    }
  }
}

std::string
DBusBluetoothManager::extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath) {
  const std::string prefix = "dev_";
  size_t pos = objectPath.find(prefix);
  if (pos == std::string::npos) {
    return "";
  }

  std::string address = objectPath.substr(pos + prefix.size());
  std::replace(address.begin(), address.end(), '_', ':');

  const std::string separator = "/";
  pos = address.find(separator);
  if (pos == std::string::npos) {
    return address;
  }

  return address.substr(0, pos);
}

std::vector<IBluetoothAdapter*> DBusBluetoothManager::getAdapters() const {
  std::vector<IBluetoothAdapter*> result;
  result.reserve(mAdapters.size());
  for (auto& ptr : mAdapters) {
    result.push_back(ptr.get());
  }
  return result;
}

std::vector<IBluetoothDevice*> DBusBluetoothManager::getDevices() const {
  std::vector<IBluetoothDevice*> result;
  result.reserve(mDevices.size());
  for (auto& ptr : mDevices) {
    result.push_back(ptr.get());
  }
  return result;
}

std::optional<IBluetoothDevice*> DBusBluetoothManager::findDevice(std::string deviceName) const {
  return this->findDBusDevice(deviceName);
}

std::optional<DBusBluetoothDevice*>
DBusBluetoothManager::findDBusDevice(std::string value, std::string property) const {
  auto iterator = std::find_if(mDevices.begin(), mDevices.end(), [&](const auto& device) {
    std::string deviceValue;
    if (property == "name") {
      deviceValue = device->name();
    } else {
      deviceValue = device->address();
    }

    return deviceValue == value;
  });

  if (iterator == mDevices.end()) {
    return std::nullopt;
  }

  return (*iterator).get();
}

void DBusBluetoothManager::removeDevice(DBusBluetoothDevice* device) {
  mDevices.erase(std::remove_if(mDevices.begin(), mDevices.end(),
                                [device](const std::unique_ptr<DBusBluetoothDevice>& p) {
                                  return p.get() == device;
                                }),
                 mDevices.end());
}

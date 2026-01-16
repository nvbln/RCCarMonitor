#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <optional>

#include "ObjectManager.generated.h"
#include "DBusBluetoothManager.h"

void DBusBluetoothManager::handleExistingObjects() {
    auto objectsInterfacesAndProperties = GetManagedObjects();
    for (const auto& [object, interfacesAndProperties] : objectsInterfacesAndProperties) {
        onInterfacesAdded(object, interfacesAndProperties);
    }
}

void DBusBluetoothManager::onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                       const std::map<sdbus::InterfaceName, 
                                      std::map<sdbus::PropertyName,
                                               sdbus::Variant>>&
                             interfacesAndProperties) {
    // Filter on Bluetooth devices.
    for (const auto& [interface, properties] : interfacesAndProperties) {
        if (interface == "org.bluez.Device1") {
            mDevices.push_back(std::make_shared<DBusBluetoothDevice>(
                this->mConnection,
                this->mDestination,
                objectPath
            ));
        } else if (interface == "org.bluez.GattCharacteristic1") {
            std::cout << "Added characteristic: " << objectPath << std::endl;
            std::string address = this->extractDeviceAddressFromObjectPath(objectPath);
            auto optDevice = findDBusDevice(address, "address");

            if (optDevice) {
                auto device = *optDevice;
                device->addCharacteristic(std::make_shared<DBusGattCharacteristic>(
                    this->mConnection,
                    this->mDestination,
                    objectPath
                ));
            } else {
                std::cout << "Couldn't find device: " << address << std::endl;
            }
        }
        auto aliasProperty = properties.find(sdbus::PropertyName{"Alias"});
        if (aliasProperty != properties.end()) {
            std::cout << "Device with alias: ";
            std::cout << properties.at(sdbus::PropertyName{"Alias"}).get<std::string>();
            std::cout << " was added." << std::endl;
        }
    }
}

void DBusBluetoothManager::onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                         const std::vector<sdbus::InterfaceName>& interfaces) {
    std::cout << "removed: " << objectPath << std::endl;
    for (const auto& interface : interfaces) {
        // Extract device address
        std::string address = this->extractDeviceAddressFromObjectPath(objectPath);
        auto devicePtr = findDBusDevice(address, "address");

        if (interface == "org.bluez.Device1") {
            if (devicePtr) {
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

std::string DBusBluetoothManager::extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath) {
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

std::vector<std::shared_ptr<IBluetoothDevice>> DBusBluetoothManager::getDevices() const {
    std::vector<std::shared_ptr<IBluetoothDevice>> outputDevices;
    std::ranges::transform(mDevices, std::back_inserter(outputDevices),
        [](const std::shared_ptr<DBusBluetoothDevice>& device) {
            return std::static_pointer_cast<IBluetoothDevice>(device);
        }
    );

    return outputDevices;
}

std::optional<std::shared_ptr<IBluetoothDevice>> DBusBluetoothManager::findDevice(std::string deviceName) const {
    auto device = this->findDBusDevice(deviceName);
    if (device) {
        return std::static_pointer_cast<IBluetoothDevice>(*device);
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<DBusBluetoothDevice>> DBusBluetoothManager::findDBusDevice(std::string value, std::string property) const {
    auto iterator = std::find_if(mDevices.begin(), mDevices.end(),
        [&](const auto& device) {
            std::string deviceValue;
            if (property == "name") {
                deviceValue = device->name();
            } else {
                deviceValue = device->address();
            }

            return deviceValue == value;
        }
    );

    if (iterator == mDevices.end()) {
        return std::nullopt;
    }

    return *iterator;
}

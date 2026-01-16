#include <sdbus-c++/sdbus-c++.h>
#include <iostream>

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
            devices.push_back(std::make_shared<DBusBluetoothDevice>(
                this->connection,
                this->destination,
                objectPath
            ));
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
    std::cout << objectPath << " removed:\t";
    for (const auto& interface : interfaces) {
        if (interface == "org.bluez.Device1") {
            // Extract device address
            std::string address = this->extractDeviceAddressFromObjectPath(objectPath);

            auto iterator = std::find_if(this->devices.begin(), this->devices.end(),
                [&](const auto& device) {
                    return device->address() == address;
                }
            );
            auto device = *iterator;

            std::cout << "Device " << device->name() << " removed." << std::endl;
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

    return address;
}

std::vector<std::shared_ptr<IBluetoothDevice>> DBusBluetoothManager::getDevices() const {
    std::vector<std::shared_ptr<IBluetoothDevice>> outputDevices;
    std::ranges::transform(devices, std::back_inserter(outputDevices),
        [](const std::shared_ptr<DBusBluetoothDevice>& device) {
            return std::static_pointer_cast<IBluetoothDevice>(device);
        }
    );

    return outputDevices;
}

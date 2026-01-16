#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <cctype>

#include "IBluetoothDevice.h"
#include "Device1.generated.h"
#include "DBusBluetoothDevice.h"

void DBusBluetoothDevice::addCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic) {
    mCharacteristics.push_back(characteristic);
}

void DBusBluetoothDevice::removeCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic) {
    mCharacteristics.erase(
        std::remove(mCharacteristics.begin(), mCharacteristics.end(), characteristic),
        mCharacteristics.end()
    );
}

void DBusBluetoothDevice::connect() {
    Connect();
}

void DBusBluetoothDevice::disconnect() {
    Disconnect();
}

bool DBusBluetoothDevice::isConnected() {
    return Connected();
}

std::string DBusBluetoothDevice::name() {
    return Alias();
}

std::string DBusBluetoothDevice::address() {
    return Address();
}

std::optional<std::shared_ptr<IGattCharacteristic>> DBusBluetoothDevice::findCharacteristic(std::string uuid) {
    auto characteristic = this->findDBusCharacteristic(uuid);
    if (characteristic) {
        return std::static_pointer_cast<IGattCharacteristic>(*characteristic);
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<DBusGattCharacteristic>> DBusBluetoothDevice::findDBusCharacteristic(std::string value, std::string property) {
    auto iterator = std::find_if(mCharacteristics.begin(), mCharacteristics.end(),
        [&](const auto& characteristic) {
            if (property == "uuid") {
                std::ranges::transform(value, value.begin(),
                    [](unsigned char c) { return std::tolower(c); });
                return characteristic->uuid() == value;
            } else {
                return characteristic->objectPath() == value;
            }
        }
    );

    if (iterator == mCharacteristics.end()) {
        return std::nullopt;
    }

    return *iterator;
}

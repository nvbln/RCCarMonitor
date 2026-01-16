#include <sdbus-c++/sdbus-c++.h>

#include "GattCharacteristic1.generated.h"
#include "DBusGattCharacteristic.h"

sdbus::ObjectPath DBusGattCharacteristic::objectPath() {
    return mObjectPath;
}

std::string DBusGattCharacteristic::uuid() {
    return UUID();
}

std::vector<uint8_t> DBusGattCharacteristic::read() {
    // TODO: Implement error when read doesn't succeed.
    return ReadValue({});
}

void DBusGattCharacteristic::write(const std::vector<uint8_t>& value) {
    // TODO: Implement error when write doesn't succeed.
    WriteValue(value, {});
}

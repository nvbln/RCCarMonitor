#include <sdbus-c++/sdbus-c++.h>
#include <iostream>

#include "IBluetoothDevice.h"
#include "Device1.generated.h"
#include "DBusBluetoothDevice.h"

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

#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include "DBusBluetoothManager.h"
#include "DBusBluetoothDevice.h"

int main() {
    // Create a proxy for the BlueZ ObjectManager.
    auto connection = sdbus::createSystemBusConnection();
    sdbus::ServiceName destination{"org.bluez"};
    sdbus::ObjectPath objectPath{"/"};

    // TODO: Error handling
    std::unique_ptr<IBluetoothManager> bluetoothManager;
    try {
    bluetoothManager = std::make_unique<DBusBluetoothManager>(*connection,
                                                              destination,
                                                              std::move(objectPath));

    } catch (const sdbus::Error& e) {
        std::cerr << "Call failed: " << e.getName() << " - " << e.getMessage() << std::endl;
        return 1;
    }

    std::vector<std::shared_ptr<IBluetoothDevice>> devices = bluetoothManager->getDevices();

    // Get the device
    std::string deviceName;
    std::cout << "Pick your device: ";
    std::cin >> deviceName;

    auto iterator = std::find_if(devices.begin(), devices.end(),
        [&](const auto& device) {
            return device->name() == deviceName;
        }
    );

    if (iterator == devices.end()) {
        std::cout << "Couldn't find the device" << std::endl;
        return 1;
    }
    auto device = *iterator;

    // Connect to the device
    device->connect();
    std::cout << "Connected to device? " << device->isConnected() << std::endl;

    std::cout << "Disconnecting..." << std::endl;
    device->disconnect();
    std::cout << "Connected to device? " << device->isConnected() << std::endl;


    return 0;
}

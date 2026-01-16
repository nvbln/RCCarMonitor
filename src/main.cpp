#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

#include "DBusBluetoothManager.h"
#include "DBusBluetoothDevice.h"

#include <chrono>
#include <thread>

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

    std::optional<std::shared_ptr<IBluetoothDevice>> optDevice = bluetoothManager->findDevice(deviceName);
    std::shared_ptr<IBluetoothDevice> device;
    if (optDevice) {
        device = *optDevice;
    } else {
        std::cout << "Couldn't find the device." << std::endl;
        return 1;
    }

    // Connect to the device
    device->connect();
    std::cout << "Connected to device? " << device->isConnected() << std::endl;

    // Read the characteristic.
    auto characteristicPtr = device->findCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
    if (characteristicPtr) {
        auto characteristic = *characteristicPtr;

        std::vector<uint8_t> bytesTrue = { 0x01 };
        std::vector<uint8_t> bytesFalse = { 0x00 };
        std::vector<uint8_t> currentValue = characteristic->read();

        if (currentValue != bytesTrue) {
            std::cout << "LED is off" << std::endl;
        } else {
            std::cout << "LED is on" << std::endl;
        }

        characteristic->write(bytesTrue);
        std::cout << "Turned LED on" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        characteristic->write(bytesFalse);
        std::cout << "Turned LED off" << std::endl;
    } else {
        std::cout << "Didn't find characteristic" << std::endl;
    }

    std::cout << "Disconnecting..." << std::endl;
    device->disconnect();
    std::cout << "Connected to device? " << device->isConnected() << std::endl;


    return 0;
}

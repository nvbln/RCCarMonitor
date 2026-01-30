#ifndef DBUSBLUETOOTHDEVICE_H
#define DBUSBLUETOOTHDEVICE_H

#include <sdbus-c++/sdbus-c++.h>

#include <optional>

#include "DBusGattCharacteristic.h"
#include "Device1.generated.h"
#include "IGattCharacteristic.h"
#include "IBluetoothDevice.h"

/**
 * @class DBusBluetoothDevice
 * @brief DBus implementation of the Bluetooth device.
 *
 * Brings together the general interface for Bluetooth devices
 * and the Bluetooth device as defined by the DBus XML object.
 * Interacts with the Bluetooth device through the DBus API.
 */
class DBusBluetoothDevice final : public IBluetoothDevice,
                                  public sdbus::ProxyInterfaces<org::bluez::Device1_proxy> {
public:
    /**
     * @brief creates an instance representing the DBus Bluetooth device.
     *
     * @param connection The Connection to the DBus.
     * @param destination The bus name of the service to communicate with.
     * @param path The path to the device that this instance should represent.
     */
    DBusBluetoothDevice(sdbus::IConnection& connection,
                        sdbus::ServiceName destination,
                        sdbus::ObjectPath path): 
        ProxyInterfaces(connection, destination, std::move(path)),
                mConnection(connection), mDestination(destination) {
        registerProxy();
    }

    ~DBusBluetoothDevice() {
        unregisterProxy();
    }

    /**
     * @brief adds a DBus GATT Characteristic to the device.
     * @param characteristic The characteristic to add to the device.
     */
    void addCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic);
    void removeCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic);
    std::optional<std::shared_ptr<DBusGattCharacteristic>> findDBusCharacteristic(std::string value, std::string property = "uuid");

    void connect() override;
    void disconnect() override;
    bool isConnected() override;
    std::string name() override;
    std::string address() override;
    std::optional<std::shared_ptr<IGattCharacteristic>> findCharacteristic(std::string uuid) override;

private:
    sdbus::IConnection& mConnection;
    sdbus::ServiceName mDestination;
    std::vector<std::shared_ptr<DBusGattCharacteristic>> mCharacteristics;
};

#endif

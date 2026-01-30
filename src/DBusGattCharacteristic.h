#ifndef DBUSGATTCHARACTERISTIC_H
#define DBUSGATTCHARACTERISTIC_H

#include "IGattCharacteristic.h"
#include "GattCharacteristic1.generated.h"

/**
 * @class DBusGattCharacteristic
 * @brief DBus implementation of the GATT Characteristic.
 *
 * Brings together the general interface for GATT Characteristics,
 * as well as Characteristic as defined by the DBus XML object.
 * Interacts with the GATT Characteristic through the DBus API.
 */
class DBusGattCharacteristic : public IGattCharacteristic,
                               public sdbus::ProxyInterfaces<org::bluez::GattCharacteristic1_proxy> {
public:
    /**
     * @brief creates an instance representing the DBus GATT Characteristic.
     *
     * @param connection The Connection to the DBus.
     * @param destination The bus name of the service to communicate with.
     * @param path The path to the Characteristic that this instance should represent.
     */
    DBusGattCharacteristic(sdbus::IConnection& connection,
                 sdbus::ServiceName destination,
                 sdbus::ObjectPath path): 
        ProxyInterfaces(connection, destination, path),
                mConnection(connection), mDestination(destination), mObjectPath(path) {
        registerProxy();
    }

    ~DBusGattCharacteristic() {
        unregisterProxy();
    }

    /**
     * @brief returns the complete path to the Characteristic.
     *
     * @return the DBus path to (and including) the Characteristic.
     */
    sdbus::ObjectPath objectPath();

    /**
     * @see IGattCharacteristic::uuid()
     */
    std::string uuid() override;

    /**
     * @see IGattCharacteristic::read()
     */
    std::vector<uint8_t> read() override;

    /**
     * @see IGattCharacteristic::write()
     */
    void write(const std::vector<uint8_t>& value) override;

private:
    sdbus::IConnection& mConnection;
    sdbus::ServiceName mDestination;
    sdbus::ObjectPath mObjectPath;
};

#endif

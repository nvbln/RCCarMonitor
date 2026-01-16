#ifndef DBUSGATTCHARACTERISTIC_H
#define DBUSGATTCHARACTERISTIC_H

#include "IGattCharacteristic.h"
#include "GattCharacteristic1.generated.h"

class DBusGattCharacteristic : public IGattCharacteristic,
                               public sdbus::ProxyInterfaces<org::bluez::GattCharacteristic1_proxy> {
public:
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

    sdbus::ObjectPath objectPath();

    std::string uuid() override;
    std::vector<uint8_t> read() override;
    void write(const std::vector<uint8_t>& value) override;

private:
    sdbus::IConnection& mConnection;
    sdbus::ServiceName mDestination;
    sdbus::ObjectPath mObjectPath;
};

#endif

#ifndef DBUSBLUETOOTHDEVICE_H
#define DBUSBLUETOOTHDEVICE_H

#include <sdbus-c++/sdbus-c++.h>

#include "Device1.generated.h"
#include "IBluetoothDevice.h"

class DBusBluetoothDevice final : public IBluetoothDevice,
                                  public sdbus::ProxyInterfaces<org::bluez::Device1_proxy> {
public:
    DBusBluetoothDevice(sdbus::IConnection& connection,
                        sdbus::ServiceName destination,
                        sdbus::ObjectPath path): 
        ProxyInterfaces(connection, destination, std::move(path)),
                connection(connection), destination(destination) {
        registerProxy();
    }

    ~DBusBluetoothDevice() {
        unregisterProxy();
    }

    void connect() override;
    void disconnect() override;
    bool isConnected() override;
    std::string name() override;
    std::string address() override;

private:
    sdbus::IConnection& connection;
    sdbus::ServiceName destination;
};

#endif

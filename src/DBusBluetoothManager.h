#ifndef DBUSBLUETOOTHMANAGER_H 
#define DBUSBLUETOOTHMANAGER_H

#include <vector>

#include <sdbus-c++/sdbus-c++.h>

#include "ObjectManager.generated.h"
#include "IBluetoothManager.h"
#include "DBusBluetoothDevice.h"
#include "IBluetoothDevice.h"

class DBusBluetoothManager final : public IBluetoothManager,
                                   public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
public:
    DBusBluetoothManager(sdbus::IConnection& connection,
                 sdbus::ServiceName destination,
                 sdbus::ObjectPath path): 
        ProxyInterfaces(connection, destination, std::move(path)),
                mConnection(connection), mDestination(destination) {
        registerProxy();
        handleExistingObjects();
    }

    ~DBusBluetoothManager() {
        unregisterProxy();
    }

    void handleExistingObjects();
    std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const override;
    std::optional<std::shared_ptr<IBluetoothDevice>> findDevice(std::string deviceName) const override;

private:
    std::vector<std::shared_ptr<DBusBluetoothDevice>> mDevices;
    sdbus::IConnection& mConnection;
    sdbus::ServiceName mDestination;

    void onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                           const std::map<sdbus::InterfaceName, 
                                          std::map<sdbus::PropertyName,
                                                   sdbus::Variant>>&
                                 interfacesAndProperties) override;

    void onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                             const std::vector<sdbus::InterfaceName>& interfaces) override;

    static std::string extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath);

    std::optional<std::shared_ptr<DBusBluetoothDevice>> findDBusDevice(std::string deviceValue, std::string property = "name") const;
};

#endif

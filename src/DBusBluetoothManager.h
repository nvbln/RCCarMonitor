#ifndef DBUSBLUETOOTHMANAGER_H 
#define DBUSBLUETOOTHMANAGER_H

#include <vector>

#include <sdbus-c++/sdbus-c++.h>

#include "ObjectManager.generated.h"
#include "IBluetoothManager.h"
#include "DBusBluetoothDevice.h"
#include "IBluetoothDevice.h"

/**
 * @class DBusBluetoothManager
 * @brief The DBus implementation of the generic Bluetooth interface
 *
 * In the DBus API the Bluetooth Manager is responsible for all the
 * bluetooth-related objects that are created in the DBus as part of
 * its device discovery. This class tries to approach the way that
 * the Object Manager is used in the DBus, while at the same time
 * adhering the to the generic Bluetooth interface of the application.
 */
class DBusBluetoothManager final : public IBluetoothManager,
                                   public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
public:
    /**
     * @brief creates an instance representing the DBus implementation of the Bluetooth
     *
     * @param connection The Connection to the DBus.
     * @param destination The bus name of the service to communicate with.
     * @param path The path to the root of the DBus Bluetooth component.
     */
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

    /**
     * @see IBluetoothManager::getDevices()
     */
    std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const override;

    /**
     * @see IBluetoothManager::findDevice()
     */
    std::optional<std::shared_ptr<IBluetoothDevice>> findDevice(std::string deviceName) const override;

private:
    std::vector<std::shared_ptr<DBusBluetoothDevice>> mDevices;
    sdbus::IConnection& mConnection;
    sdbus::ServiceName mDestination;

    void handleExistingObjects();

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

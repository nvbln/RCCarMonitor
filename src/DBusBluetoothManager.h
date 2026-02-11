#pragma once

#include <memory>
#include <vector>
#include <string>

#include "IBluetoothManager.h"
#include "DBusBluetoothDevice.h"
#include "IBluetoothDevice.h"
#include "IDBusObjectManagerProxy.h"

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
class DBusBluetoothManager final : public IBluetoothManager {
public:
    /**
     * @brief creates an instance representing the DBus implementation of the Bluetooth
     *
     * @param proxy The proxy through which the manager communicates with SDBus.
     */
    DBusBluetoothManager(std::shared_ptr<IDBusObjectManagerProxy> proxy);

    /**
     * @see IBluetoothManager::getDevices()
     */
    std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const override;

    /**
     * @see IBluetoothManager::findDevice()
     */
    std::optional<std::shared_ptr<IBluetoothDevice>> findDevice(std::string deviceName) const override;

private:
    std::shared_ptr<IDBusObjectManagerProxy> mProxy;
    std::vector<std::shared_ptr<DBusBluetoothDevice>> mDevices;

    void handleExistingObjects();

    void onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                           const std::map<sdbus::InterfaceName, 
                                          std::map<sdbus::PropertyName,
                                                   sdbus::Variant>>&
                                 interfacesAndProperties);

    void onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                             const std::vector<sdbus::InterfaceName>& interfaces);

    static std::string extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath);

    std::optional<std::shared_ptr<DBusBluetoothDevice>> findDBusDevice(std::string deviceValue, std::string property = "name") const;
};

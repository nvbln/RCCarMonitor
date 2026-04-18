#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DBusBluetoothAdapter.h"
#include "DBusBluetoothDevice.h"
#include "interfaces/IBluetoothAdapter.h"
#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IBluetoothManager.h"
#include "proxies/IDBusObjectManagerProxy.h"

using InterfacesAndPropertiesMap = IDBusObjectManagerProxy::InterfacesAndPropertiesMap;

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
  DBusBluetoothManager(std::unique_ptr<IDBusObjectManagerProxy> proxy);

  /**
   * @see IBluetoothManager::getAdapters()
   */
  std::vector<IBluetoothAdapter*> getAdapters() const override;

  /**
   * @see IBluetoothManager::getDevices()
   */
  std::vector<IBluetoothDevice*> getDevices() const override;

  /**
   * @see IBluetoothManager::findDevice()
   */
  std::optional<IBluetoothDevice*> findDevice(std::string deviceName) const override;

private:
  std::unique_ptr<IDBusObjectManagerProxy> mProxy;
  std::vector<std::unique_ptr<DBusBluetoothAdapter>> mAdapters;
  std::vector<std::unique_ptr<DBusBluetoothDevice>> mDevices;

  void handleExistingObjects();

  void onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                         const InterfacesAndPropertiesMap& interfacesAndProperties);

  void onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                           const std::vector<sdbus::InterfaceName>& interfaces);

  static std::string extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath);

  std::optional<DBusBluetoothDevice*> findDBusDevice(std::string deviceValue,
                                                     std::string property = "name") const;

  void removeDevice(DBusBluetoothDevice* device);
};

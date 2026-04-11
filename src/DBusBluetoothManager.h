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
  DBusBluetoothManager(std::shared_ptr<IDBusObjectManagerProxy> proxy);

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
  std::optional<std::shared_ptr<IBluetoothDevice>>
  findDevice(std::string deviceName) const override;

private:
  std::shared_ptr<IDBusObjectManagerProxy> mProxy;
  std::vector<std::shared_ptr<DBusBluetoothAdapter>> mAdapters;
  std::vector<std::shared_ptr<DBusBluetoothDevice>> mDevices;

  void handleExistingObjects();

  void onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                         const InterfacesAndPropertiesMap& interfacesAndProperties);

  void onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                           const std::vector<sdbus::InterfaceName>& interfaces);

  static std::string extractDeviceAddressFromObjectPath(const sdbus::ObjectPath& objectPath);

  std::optional<std::shared_ptr<DBusBluetoothDevice>>
  findDBusDevice(std::string deviceValue, std::string property = "name") const;

  void removeDevice(std::shared_ptr<DBusBluetoothDevice> device);

  /**
   * @brief Converts a subclass to its superclass.
   *
   * In some cases, C++ doesn't do the polymorphism (i.e.
   * converting SpecificImpl to IGenericInterface) automatically.
   * This function converts the objects explicitly to the given
   * superclass.
   */
  template <typename Base, typename Derived>
  std::vector<Base*> castVector(const std::vector<std::shared_ptr<Derived>> input) const {
    static_assert(std::is_base_of_v<Base, Derived>, "Derived must inherit from Base");
    std::vector<Base*> output;
    output.reserve(input.size()); // TODO: This should be changed right?

    std::ranges::transform(
        input, std::back_inserter(output),
        [](const std::shared_ptr<Derived>& ptr) { return static_cast<Base*>(ptr.get()); });

    return output;
  }
};

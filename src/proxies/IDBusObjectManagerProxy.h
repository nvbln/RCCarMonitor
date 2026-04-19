#pragma once

#include "IDBusAdapterProxy.h"
#include "IDBusCharacteristicProxy.h"
#include "IDBusDeviceProxy.h"
#include "IDBusPropertiesProxy.h"
#include <sdbus-c++/sdbus-c++.h>

/**
 * @class IDBusObjectManagerProxy
 * @brief interface for wrapper between DBusBluetoothManager and the SDBus library.
 *
 * The proxy implementation handles the communication with the SDBus library.
 * This interface facilitates the communication between the DBus implementation in
 * the application and the SDBus library specifics.
 */
class IDBusObjectManagerProxy {
public:
  using InterfacesAndPropertiesMap =
      std::map<sdbus::InterfaceName, std::map<sdbus::PropertyName, sdbus::Variant>>;
  /**
   * A callback such that the application DBus implementation is updated on
   * new interfaces, without having to inherit from the SDBus library.
   *
   * The callback is triggered every time an interface is added.
   */
  using OnInterfacesAddedCallback =
      std::function<void(const sdbus::ObjectPath& objectPath,
                         const InterfacesAndPropertiesMap& interfacesAndProperties)>;

  /**
   * A callback such that the application DBus implementation is updated on
   * removed interfaces, without having to inherit from the SDBus library.
   *
   * The callback is triggered every time an interface is removed.
   */
  using OnInterfacesRemovedCallback = std::function<void(
      const sdbus::ObjectPath& objectPath, const std::vector<sdbus::InterfaceName>& interfaces)>;

  virtual ~IDBusObjectManagerProxy() = default;

  /**
   * @brief subscribes the callback to updates on added interfaces.
   *
   * @param callback The function to notify of the new interface.
   */
  virtual void addOnInterfacesAddedCallback(const OnInterfacesAddedCallback callback) = 0;

  /**
   * @brief subscribes the callback to updates on removed interfaces.
   *
   * @param callback The function to notify of the removed interface.
   */
  virtual void addOnInterfacesRemovedCallback(const OnInterfacesRemovedCallback callback) = 0;

  /**
   * @brief creates a new instance of IDBusAdapterProxy with the given path.
   *
   * @param objectPath The path of the adapter.
   * @return a pointer to the newly created IDbusAdapterProxy instance.
   */
  virtual std::unique_ptr<IDBusAdapterProxy> createAdapter(std::string objectPath) = 0;

  /**
   * @brief creates a new instance of IDBusDeviceProxy with the given path.
   *
   * @param objectPath The path of the device.
   * @return a pointer to the newly created IDBusDeviceProxy instance.
   */
  virtual std::unique_ptr<IDBusDeviceProxy> createDevice(std::string objectPath) = 0;

  /**
   * @brief creates a new instance of IDBusCharacteristicProxy with the given path.
   *
   * @param objectPath The path of the characteristic.
   * @return a pointer to the newly created IDBusCharacteristicProxy instance.
   */
  virtual std::unique_ptr<IDBusCharacteristicProxy>
  createCharacteristic(std::string objectPath) = 0;

  /**
   * @brief creates an instance of the Properties interface on the given path.
   *
   * The instance allows for listening for property changes.
   *
   * @param objectPath The path of the object of interest.
   * @return a pointer to the newly created IDBusPropertiesProxy instance.
   */
  virtual std::unique_ptr<IDBusPropertiesProxy> createProperties(std::string objectPath) = 0;

  /**
   * @brief retrieves the objects that the ObjectManager currently holds.
   *
   * @return a map of objects that the ObjectManager holds.
   */
  virtual std::map<sdbus::ObjectPath, InterfacesAndPropertiesMap> managedObjects() = 0;
};

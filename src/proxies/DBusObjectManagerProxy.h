#pragma once

#include <sdbus-c++/sdbus-c++.h>
#include <vector>

#include "IDBusDeviceProxy.h"

#include "DBusAdapterProxy.h"
#include "DBusCharacteristicProxy.h"
#include "DBusDeviceProxy.h"
#include "DBusPropertiesProxy.h"
#include "IDBusCharacteristicProxy.h"
#include "IDBusDeviceProxy.h"
#include "IDBusObjectManagerProxy.h"
#include "IDBusPropertiesProxy.h"

using InterfacesAndPropertiesMap = IDBusObjectManagerProxy::InterfacesAndPropertiesMap;

/**
 * @class DBusObjectManagerProxy
 * @brief wrapper implementation of the Object Manager proxy.
 *
 * Forwards the Object Manager calls to the SDBus library.
 */
class DBusObjectManagerProxy : public IDBusObjectManagerProxy,
                               public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
public:
  /**
   * @brief creates a proxy to wrap the SDBus library.
   *
   * This proxy wraps the SDBus instance of the Object Manager in Bluetooth.
   *
   * @param connection The Connection to the DBus.
   * @param destination The bus name of the service to communicate with.
   * @param path The path to the root of the DBus Bluetooth component.
   */
  DBusObjectManagerProxy(sdbus::IConnection& connection, sdbus::ServiceName destination,
                         sdbus::ObjectPath path)
      : ProxyInterfaces(connection, destination, std::move(path)), mConnection(connection),
        mDestination(destination) {
    registerProxy();
  }

  ~DBusObjectManagerProxy() { unregisterProxy(); }

  /**
   * @see IDBusObjectManagerProxy::addOnInterfacesAddedCallback()
   */
  void addOnInterfacesAddedCallback(OnInterfacesAddedCallback callback) override {
    mOnAddedCallbacks.push_back(callback);
  };

  /**
   * @see IDBusObjectManagerProxy::addOnInterfacesRemovedCallback()
   */
  void addOnInterfacesRemovedCallback(OnInterfacesRemovedCallback callback) override {
    mOnRemovedCallbacks.push_back(callback);
  }

  /**
   * @see IDBusObjectManagerProxy::createAdapter()
   */
  std::unique_ptr<IDBusAdapterProxy> createAdapter(std::string objectPath) override {
    return std::make_unique<DBusAdapterProxy>(mConnection, mDestination,
                                              sdbus::ObjectPath{objectPath});
  };

  /**
   * @see IDBusObjectManagerProxy::createDevice()
   */
  std::unique_ptr<IDBusDeviceProxy> createDevice(std::string objectPath) override {
    return std::make_unique<DBusDeviceProxy>(mConnection, mDestination,
                                             sdbus::ObjectPath{objectPath});
  }

  /**
   * @see IDBusObjectManagerProxy::createCharacteristic()
   */
  std::unique_ptr<IDBusCharacteristicProxy> createCharacteristic(std::string objectPath) override {
    return std::make_unique<DBusCharacteristicProxy>(mConnection, mDestination,
                                                     sdbus::ObjectPath{objectPath});
  }

  /**
   * @see IDBusObjectManagerProxy::createProperties()
   */
  std::unique_ptr<IDBusPropertiesProxy> createProperties(std::string objectPath) override {
    return std::make_unique<DBusPropertiesProxy>(mConnection, mDestination,
                                                 sdbus::ObjectPath{objectPath});
  }

  /**
   * @see IDBusObjectManagerProxy::managedObjects()
   */
  std::map<sdbus::ObjectPath, InterfacesAndPropertiesMap> managedObjects() override {
    return GetManagedObjects();
  }

private:
  sdbus::IConnection& mConnection;
  sdbus::ServiceName mDestination;

  std::vector<OnInterfacesAddedCallback> mOnAddedCallbacks;
  std::vector<OnInterfacesRemovedCallback> mOnRemovedCallbacks;

  void onInterfacesAdded(const sdbus::ObjectPath& objectPath,
                         const InterfacesAndPropertiesMap& interfacesAndProperties) override {
    for (auto callback : mOnAddedCallbacks) {
      callback(objectPath, interfacesAndProperties);
    }
  }

  void onInterfacesRemoved(const sdbus::ObjectPath& objectPath,
                           const std::vector<sdbus::InterfaceName>& interfaces) override {
    for (auto callback : mOnRemovedCallbacks) {
      callback(objectPath, interfaces);
    }
  }
};

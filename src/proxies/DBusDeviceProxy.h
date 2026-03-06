#pragma once

#include <sdbus-c++/sdbus-c++.h>

#include "Device1.generated.h"
#include "IDBusDeviceProxy.h"

/**
 * @class DBusDeviceProxy
 * @brief wrapper implementation of the device proxy.
 *
 * Forwards the device calls to the SDBus library.
 */
class DBusDeviceProxy : public IDBusDeviceProxy,
                        public sdbus::ProxyInterfaces<org::bluez::Device1_proxy> {
public:
  /**
   * @brief creates a proxy to wrap the SDBus library.
   *
   * This proxy wraps the SDBus instance of the Device in Bluetooth.
   *
   * @param connection The Connection to the DBus.
   * @param destination The bus name of the service to communicate with.
   * @param path The path to the device that this instance should represent.
   */
  DBusDeviceProxy(sdbus::IConnection &connection, sdbus::ServiceName destination,
                  sdbus::ObjectPath path)
      : ProxyInterfaces(connection, destination, std::move(path)) {
    registerProxy();
  }

  ~DBusDeviceProxy() { unregisterProxy(); }

  /**
   * @see IDBusDeviceProxy::connect()
   */
  void connect() override { Connect(); }

  /**
   * @see IDBusDeviceProxy::disconnect()
   */
  void disconnect() override { Disconnect(); }

  /**
   * @see IDBusDeviceProxy::isConnected()
   */
  bool isConnected() override { return Connected(); }

  /**
   * @see IDBusDeviceProxy::name()
   */
  std::string name() override { return Alias(); }

  /**
   * @see IDBusDeviceProxy::address()
   */
  std::string address() override { return Address(); }
};

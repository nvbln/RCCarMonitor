#pragma once

#include "Adapter1.generated.h"
#include "IDBusAdapterProxy.h"

#include <sdbus-c++/sdbus-c++.h>

/**
 * @class DBusAdapterProxy
 * @brief wrapper implementation of the Adapter proxy.
 *
 * Forwards the calls to the SDBus library.
 */
class DBusAdapterProxy : public IDBusAdapterProxy,
                         public sdbus::ProxyInterfaces<org::bluez::Adapter1_proxy> {
public:
  /**
   * @brief Creates a proxy to wrap the SDBus library.
   *
   * @param connection the Connection to the DBus.
   * @param destination The bus name of the service to communicate with.
   * @param path The path to the adapter that should be represented by this object.
   */
  DBusAdapterProxy(sdbus::IConnection& connection, sdbus::ServiceName destination,
                   sdbus::ObjectPath path)
      : ProxyInterfaces(connection, destination, std::move(path)), mConnection(connection),
        mDestination(destination) {
    registerProxy();
  }

  ~DBusAdapterProxy() { unregisterProxy(); }

  /**
   * @see IDBusAdapterProxy::startDiscovery();
   */
  void startDiscovery() override { StartDiscovery(); }

  /**
   * @see IDBusAdapterProxy::stopDiscovery();
   */
  void stopDiscovery() override { StopDiscovery(); }

private:
  sdbus::IConnection& mConnection;
  sdbus::ServiceName mDestination;
};

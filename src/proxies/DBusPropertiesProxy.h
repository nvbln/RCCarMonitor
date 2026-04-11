#pragma once

#include "IDBusPropertiesProxy.h"

#include <sdbus-c++/sdbus-c++.h>
#include <vector>

/**
 * @class DBusPropertiesProxy
 * @brief wrapper implementation of the Properties proxy.
 *
 * Forwards the Properties calls to the SDBus library.
 */
class DBusPropertiesProxy : public IDBusPropertiesProxy,
                            public sdbus::ProxyInterfaces<sdbus::Properties_proxy> {
public:
  /**
   * @brief creates a proxy to wrap the SDBus library.
   *
   * This proxy wraps the SDBus instance of the Properties interface in Bluetooth.
   *
   * @param connection The Connection to the DBus.
   * @param destination The bus name of the service to communicate with.
   * @param path The path to the root of the DBus Bluetooth object.
   */
  DBusPropertiesProxy(sdbus::IConnection& connection, sdbus::ServiceName destination,
                      sdbus::ObjectPath path)
      : ProxyInterfaces(connection, destination, std::move(path)), mConnection(connection),
        mDestination(destination) {
    registerProxy();
  }

  ~DBusPropertiesProxy() { unregisterProxy(); }

  /**
   * @see IDBusPropertiesProxy::subscribeToOnPropertiesChanged()
   */
  void subscribeToOnPropertiesChanged(OnPropertiesChangedCallback callback) override {
    mOnPropertiesChangedCallbacks.push_back(callback);
  }

private:
  sdbus::IConnection& mConnection;
  sdbus::ServiceName mDestination;

  std::vector<OnPropertiesChangedCallback> mOnPropertiesChangedCallbacks;

  void onPropertiesChanged(const sdbus::InterfaceName& interface,
                           const std::map<sdbus::MemberName, sdbus::Variant>& changedProperties,
                           const std::vector<sdbus::MemberName>& invalidatedProperties) override {
    for (auto callback : mOnPropertiesChangedCallbacks) {
      callback(interface, changedProperties, invalidatedProperties);
    }
  }
};

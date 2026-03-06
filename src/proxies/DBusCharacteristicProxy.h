#pragma once

#include "GattCharacteristic1.generated.h"
#include "IDBusCharacteristicProxy.h"

class DBusCharacteristicProxy
    : public IDBusCharacteristicProxy,
      public sdbus::ProxyInterfaces<org::bluez::GattCharacteristic1_proxy> {
public:
  /**
   * @brief creates a proxy to wrap the SDBus library.
   *
   * This proxy wraps the SDBus instance of the GattCharacteristic in Bluetooth.
   *
   * @param connection The Connection to the DBus.
   * @param destination The bus name of the service to communicate with.
   * @param path The path to the Characteristic that this instance should represent.
   */
  DBusCharacteristicProxy(sdbus::IConnection &connection, sdbus::ServiceName destination,
                          sdbus::ObjectPath path)
      : ProxyInterfaces(connection, destination, path), mObjectPath(path) {
    registerProxy();
  }

  ~DBusCharacteristicProxy() { unregisterProxy(); }

  /**
   * @see IDBusCharacteristicProxy::objectPath()
   */
  std::string objectPath() override { return mObjectPath.c_str(); }

  /**
   * @see IDBusCharacteristicProxy::uuid()
   */
  std::string uuid() override { return UUID(); }

  /**
   * @see IDBusCharacteristicProxy::read()
   */
  std::vector<uint8_t> read() override { return ReadValue({}); }

  /**
   * @see IDBusCharacteristicProxy::write()
   */
  void write(const std::vector<uint8_t> &value) override { return WriteValue(value, {}); }

private:
  sdbus::ObjectPath mObjectPath;
};

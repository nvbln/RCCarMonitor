#pragma once

#include "IDBusAdapterProxy.h"
#include "interfaces/IBluetoothAdapter.h"

#include <memory>

/**
 * @class DBusBluetoothAdapter
 * @brief The DBus implementation of the generic adapter interface.
 *
 * In the DBus API the Bluetooth Adapter is responsible for the
 * Bluetooth device discovery.
 */
class DBusBluetoothAdapter final : public IBluetoothAdapter {
public:
  DBusBluetoothAdapter(std::shared_ptr<IDBusAdapterProxy> proxy) : mProxy(proxy) {}

  /**
   * @brief Start scanning for new Bluetooth devices.
   *
   * @see IDBusAdapterProxy::startDiscovery();
   */
  void startDiscovery() { mProxy->startDiscovery(); }

  /**
   * @brief Stop scanning for new Bluetooth devices.
   *
   * @see IDBusAdapterProxy::stopDiscovery();
   */
  void stopDiscovery() { mProxy->stopDiscovery(); }

private:
  const std::shared_ptr<IDBusAdapterProxy> mProxy;
};

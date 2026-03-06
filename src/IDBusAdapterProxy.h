#pragma once

/**
 * @class IDBusAdapterProxy
 * @brief interface for wrapper between DBusAdapter and the SDBus library.
 *
 * The proxy implementation handles the communication with the SDBus library.
 * This interface facilitates the communication between the DBus implementation in
 * the application and the SDBus library specifics.
 */
class IDBusAdapterProxy {
public:
  ~IDBusAdapterProxy() = default;

  /**
   * @brief Start scanning for nearby Bluetooth devices.
   *
   * Discovered devices are not returned here. Instead, the ObjectManager
   * gets a signal on OnInterfacesAdded.
   */
  virtual void startDiscovery() = 0;

  /**
   * @brief Stop scanning for nearby Bluetooth devices.
   */
  virtual void stopDiscovery() = 0;
};

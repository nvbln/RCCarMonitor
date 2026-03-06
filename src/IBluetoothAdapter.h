#pragma once

/**
 * @class IBluetoothAdapter
 * @brief Interface for interacting with the Bluetooth adapter.
 *
 * This interfaces provides a library-agnostic way of interacting with the
 * Bluetooth adapter. It manages the discovery of devices through the adapter.
 */
class IBluetoothAdapter {
public:
  virtual ~IBluetoothAdapter() = default;

  /**
   * @brief Start discovery of devices.
   */
  virtual void startDiscovery() = 0;

  /**
   * @brief Stop discovery of devices.
   */
  virtual void stopDiscovery() = 0;
};

#pragma once

#include "IGattCharacteristic.h"
#include <functional>
#include <optional>
#include <string>

/**
 * @class IBluetoothDevice
 * @brief Representation of a Bluetooth device.
 *
 * This interface is a representation of a generic Bluetooth
 * device and enables generic interactions with said device.
 */
class IBluetoothDevice {
public:
  /**
   * @brief Callback to get notified of changes to the GattCharacteristic.
   * @return the GattCharacteristic that is updated.
   */
  using Callback = std::function<void(IGattCharacteristic*)>;

  virtual ~IBluetoothDevice() = default;

  /**
   * @brief Connect with the Bluetooth device.
   */
  virtual void connect() = 0;

  /**
   * @brief Disconnect from the Bluetooth device.
   */
  virtual void disconnect() = 0;

  /**
   * @brief Check if a connection is established with the device.
   *
   * @return Whether a connection is established with the device.
   */
  virtual bool isConnected() = 0;

  /**
   * @brief Returns the name of the device.
   *
   * @return The name of the device
   */
  virtual std::string name() const = 0;

  /**
   * @brief Returns the hexadecimal address of the device.
   *
   * This address only includes the hexadecimal address of the
   * device itself not the path leading to the device.
   * @return The hexadecimal address of the device.
   */
  virtual std::string address() const = 0;

  /**
   * @brief returns the GATT Characteristic matched by the name, if any
   *
   * @param uuid The UUID of the Characteristic that should be returned.
   * @return an optional that contains a pointer to the Characteristic if found.
   */
  virtual std::optional<IGattCharacteristic*> findCharacteristic(std::string uuid) = 0;

  /**
   * @brief subscribes to notifications of added characteristics.
   *
   * If a characteristic gets added to the device, the callback
   * is triggered and the added characteristic is send to the
   * subscriber.
   */
  virtual void subscribeToAddCharacteristic(Callback callback) = 0;

  /**
   * @brief subscribes to notifications of removed characteristics.
   *
   * If a characteristic gets removed from the device, the callback
   * is triggered and the removed characteristic is send to the
   * subscriber.
   */
  virtual void subscribeToRemoveCharacteristic(Callback callback) = 0;

  // TODO: Add UUID
};

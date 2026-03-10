#pragma once

#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IGattCharacteristic.h"
#include "views/IToggleableView.h"

#include <memory>
#include <string>

/**
 * @class DriveLockController
 * @brief Handles the drive lock on the vehicle.
 *
 * The vehicle has a 'drive lock' installed. This prevents the
 * vehicle from moving without explicit permission from the monitor.
 */
class DriveLockController final {
public:
  inline static const std::vector<uint8_t> BytesTrue = {0x01};
  inline static const std::vector<uint8_t> BytesFalse = {0x00};

  /**
   * @brief Creates a controller to control the drive lock on the vehicle.
   *
   * @param device The RC Car, should contain a drive lock characteristic.
   * @param toggleable a UI element that allows for toggling the setting.
   */
  DriveLockController(std::shared_ptr<IBluetoothDevice> device,
                      std::shared_ptr<IToggleableView> toggleable, std::string characteristicId);

  /**
   * @brief Updates the view with the most recent setting.
   */
  void update();

  /**
   * @brief Writes the user set toggle value to the device.
   *
   * @param on true if on, false if off.
   */
  void writeToggle(bool on);

  /**
   * @brief Reads the toggle value from the device.
   *
   * @return true if on, false if off.
   */
  bool readToggle();

private:
  std::shared_ptr<IBluetoothDevice> mBluetoothDevice;
  std::shared_ptr<IToggleableView> mToggleable;
  std::shared_ptr<IGattCharacteristic> mChar;
};

#pragma once

#include "interfaces/IGattCharacteristicHandler.h"
#include "views/IToggleableView.h"

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
  DriveLockController(IToggleableView* toggleable, IGattCharacteristicHandler* handler);

  /**
   * @brief Updates the view with the most recent setting.
   */
  void update();

private:
  IToggleableView* mToggleable;
  IGattCharacteristicHandler* mHandler;

  /**
   * @brief Writes the user set toggle value to the device.
   *
   * @param on true if on, false if off.
   */
  void writeToggle(bool on);
};

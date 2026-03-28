#pragma once

#include "Event.h"
#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IGattCharacteristic.h"
#include "interfaces/IGattCharacteristicHandler.h"

#include <memory>

/**
 * @class GattCharacteristicHandler
 * @brief Safely handles the GattCharacteristic in Bluetooth BLE.
 *
 * The GattCharacteristics on Bluetooth BLE are not guaranteed
 * to always be available. This results in a need to safely handle
 * the characteristic. As multiple characteristics are used throughout
 * the program, this handler encapsulates all the safety mechanisms.
 */
class GattCharacteristicHandler : public IGattCharacteristicHandler {
public:
  /**
   * @brief Creates a handler for the GattCharactierstic.
   *
   * @param device pointer to the Bluetooth device,
   *        assumed to be available during entire lifetime.
   * @param characteristicId the ID that the characteristic is identified by on BLE.
   */
  GattCharacteristicHandler(IBluetoothDevice *device, std::string characteristicId);

  /**
   * @see IGattCharacteristicHandler::isAvailable()
   */
  bool isAvailable() const override { return mChar != nullptr; }

  /**
   * @see IGattCharacteristicHandler::onAvailable()
   */
  void onAvailable(OnAvailableCallback callback) override { onAvailableEvent.subscribe(callback); }

  /**
   * @see IGattCharacteristicHandler::onUnavailable()
   */
  void onUnavailable(OnUnavailableCallback callback) override {
    onUnavailableEvent.subscribe(callback);
  }

  /**
   * @see IGattCharacteristicHandler::read()
   */
  std::optional<std::vector<uint8_t>> read() override;

  /**
   * @see IGattCharacteristicHandler::write()
   */
  bool write(const std::vector<uint8_t> &data) override;

private:
  IBluetoothDevice *mDevice;
  std::shared_ptr<IGattCharacteristic> mChar = nullptr;
  std::string mCharId;

  Event<OnAvailableCallback> onAvailableEvent;
  Event<OnUnavailableCallback> onUnavailableEvent;
};

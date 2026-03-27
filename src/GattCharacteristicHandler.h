#pragma once

#include "Event.h"
#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IGattCharacteristic.h"
#include "interfaces/IGattCharacteristicHandler.h"

#include <memory>

// TODO: Add documentation.
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

  bool isAvailable() const override { return mChar != nullptr; }
  void onAvailable(OnAvailableCallback callback) override { onAvailableEvent.subscribe(callback); }
  void onUnavailable(OnUnavailableCallback callback) override {
    onUnavailableEvent.subscribe(callback);
  }

  std::optional<std::vector<uint8_t>> read() override;
  bool write(const std::vector<uint8_t> &data) override;

private:
  std::shared_ptr<IBluetoothDevice> mDevice;
  std::shared_ptr<IGattCharacteristic> mChar = nullptr;
  std::string mCharId;

  Event<OnAvailableCallback> onAvailableEvent;
  Event<OnUnavailableCallback> onUnavailableEvent;
};

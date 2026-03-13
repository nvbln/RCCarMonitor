#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IGattCharacteristic.h"

#include "GattCharacteristicHandler.h"

#include <memory>

GattCharacteristicHandler::GattCharacteristicHandler(std::shared_ptr<IBluetoothDevice> device,
                                                     std::string characteristicId)
    : mDevice(device), mCharId(characteristicId) {
  auto result = device->findCharacteristic(characteristicId);

  if (result.has_value()) {
    mChar = *result;
    onAvailableEvent.notify();
  } else {
    device->subscribeToAddCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
      if (gattChar->uuid() == characteristicId) {
        this->mChar = gattChar;
        onAvailableEvent.notify();
      }
    });
  }

  device->subscribeToRemoveCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
    if (mChar != nullptr && mChar->uuid() == characteristicId) {
      this->mChar = nullptr;
      onUnavailableEvent.notify();
    }
  });
}

std::optional<std::vector<uint8_t>> GattCharacteristicHandler::read() {
  if (mChar != nullptr) {
    return mChar->read();
  }

  return std::nullopt;
}

bool GattCharacteristicHandler::write(const std::vector<uint8_t> &data) {
  if (mChar != nullptr) {
    mChar->write(data);
    return true;
  }

  return false;
}

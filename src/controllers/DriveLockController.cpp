#include "controllers/DriveLockController.h"
#include "interfaces/IBluetoothDevice.h"
#include "views/IToggleableView.h"

#include <memory>
#include <string>

// TODO: Add safety features for when the characteristic is not immediately
// TODO: initialised.

DriveLockController::DriveLockController(std::shared_ptr<IBluetoothDevice> device,
                                         std::shared_ptr<IToggleableView> toggleable,
                                         std::string characteristicId)
    : mBluetoothDevice(device), mToggleable(toggleable) {
  toggleable->disable();
  auto result = device->findCharacteristic(characteristicId);

  if (result.has_value()) {
    mChar = *result;
    toggleable->enable();
  } else {
    device->subscribeToAddCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
      if (gattChar->uuid() == characteristicId) {
        this->mChar = gattChar;
        toggleable->enable();
      }
    });
  }

  device->subscribeToRemoveCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
    if (mChar != nullptr && mChar->uuid() == characteristicId) {
      this->mChar = nullptr;
      toggleable->disable();
    }
  });

  mToggleable->subscribe([this](bool on) { writeToggle(on); });
  update();
}

void DriveLockController::update() {
  bool on = readToggle();
  mToggleable->update(on);
}

void DriveLockController::writeToggle(bool on) {
  if (on) {
    mChar->write(BytesTrue);
  } else {
    mChar->write(BytesFalse);
  }
}

bool DriveLockController::readToggle() {
  std::vector<uint8_t> value = mChar->read();
  if (value == BytesTrue) {
    return true;
  } else {
    return false;
  }
}

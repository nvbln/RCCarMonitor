#include "MovementStatusController.h"
#include "interfaces/IBluetoothDevice.h"
#include "views/IStatusView.h"

#include <string>

#include <iostream>

MovementStatusController::MovementStatusController(std::shared_ptr<IBluetoothDevice> device,
                                                   std::shared_ptr<IStatusView> statusView,
                                                   std::string characteristicId)
    : mDevice(device), mView(statusView) {
  auto result = device->findCharacteristic(characteristicId);

  if (result.has_value()) {
    mChar = *result;
  } else {
    device->subscribeToAddCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
      if (gattChar->uuid() == characteristicId) {
        this->mChar = gattChar;
      }
    });
  }

  device->subscribeToRemoveCharacteristic([&](std::shared_ptr<IGattCharacteristic> gattChar) {
    if (mChar != nullptr && mChar->uuid() == characteristicId) {
      this->mChar = nullptr;
    }
  });

  if (mChar == nullptr) {
    std::cout << "Char is null" << std::endl;
  }

  update();
}

void MovementStatusController::update() {
  std::string statusString = "Unknown";

  if (mChar != nullptr) {
    std::vector<uint8_t> value = mChar->read();
    MovementStatus status = static_cast<MovementStatus>(value[0]);
    switch (status) {
    case MovementStatus::Forwards:
      statusString = "Forwards";
      break;
    case MovementStatus::Backwards:
      statusString = "Backwards";
      break;
    case MovementStatus::Turning:
      statusString = "Turning";
      break;
    case MovementStatus::Still:
      statusString = "Still";
      break;
    }
  }

  mView->update(statusString);
}

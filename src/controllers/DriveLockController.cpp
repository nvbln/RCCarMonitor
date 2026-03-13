#include "controllers/DriveLockController.h"
#include "interfaces/IGattCharacteristicHandler.h"
#include "views/IToggleableView.h"

#include <memory>

DriveLockController::DriveLockController(std::shared_ptr<IToggleableView> toggleable,
                                         std::shared_ptr<IGattCharacteristicHandler> handler)
    : mToggleable(toggleable), mHandler(handler) {
  if (mHandler->isAvailable()) {
    toggleable->enable();
  } else {
    toggleable->disable();
  }

  mHandler->onAvailable([&toggleable]() { toggleable->enable(); });
  mHandler->onUnavailable([&toggleable]() { toggleable->disable(); });

  mToggleable->subscribe([this](bool on) { writeToggle(on); });
  update();
}

void DriveLockController::update() {
  auto valueOpt = mHandler->read();
  if (valueOpt.has_value()) {
    std::vector<uint8_t> value = *valueOpt;
    if (value == BytesTrue) {
      mToggleable->update(true);
    } else {
      mToggleable->update(false);
    }
  }
}

// TODO: Consider what to do if write fails.
void DriveLockController::writeToggle(bool on) {
  if (on) {
    mHandler->write(BytesTrue);
  } else {
    mHandler->write(BytesFalse);
  }
}

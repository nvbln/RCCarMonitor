#include "MovementStatusController.h"

#include <string>

void MovementStatusController::update() {
  std::string statusString = "Unknown";

  auto statusOpt = mHandler->read();
  if (statusOpt.has_value()) {
    std::vector<uint8_t> value = *statusOpt;
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

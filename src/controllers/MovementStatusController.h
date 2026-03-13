#pragma once

#include <cstdint>

#include "interfaces/IGattCharacteristicHandler.h"
#include "views/IStatusView.h"

#include <memory>

/**
 * @brief Describes the type of movement that the vehicle is executing.
 *
 * This is a copy of the MovementStatus in IVehicleMovement
 * in the AutonomousRCCar project.
 */
enum class MovementStatus : uint8_t { Forwards = 0, Backwards = 1, Turning = 2, Still = 3 };

/**
 * @class MovementStatusController
 * @brief Shows the current movement status of the vehicle.
 */
class MovementStatusController final {
public:
  /**
   * @brief Creates a controller to update on the status of the movement of the vehicle.
   *
   * @param device The RC Car, should contain a movement status characteristic.
   * @param statusView The view that the movement status should be displayed in.
   */
  MovementStatusController(std::shared_ptr<IStatusView> statusView,
                           std::shared_ptr<IGattCharacteristicHandler> handler)
      : mView(statusView), mHandler(handler) {
    update();
  }

  void update();

private:
  std::shared_ptr<IStatusView> mView;
  std::shared_ptr<IGattCharacteristicHandler> mHandler;
};

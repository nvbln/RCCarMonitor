#pragma once

#include "IView.h"

#include <functional>

/**
 * @class IToggleableView
 * @brief Interface for Views with the purpose of showing a boolean setting.
 *
 * A toggleable view generally looks like a label (describing/labelling the toggle)
 * on one side, and a button or switch with the boolean setting on the other side.
 */
class IToggleableView : public IView {
public:
  /**
   * @brief Callback to receive the new boolean value of the toggle after user interaction.
   * @return The boolean value of the toggle.
   */
  using Callback = std::function<void(bool)>;

  ~IToggleableView() = default;

  /**
   * @brief Updates the value of the toggle.
   *
   * @param toggle Whether it is ON or OFF.
   */
  virtual void update(const bool on) = 0;

  /**
   * @brief Subscribes for a value change of the toggle.
   *
   * @param callback The callback to call when the user changed the toggle value.
   */
  virtual void subscribe(Callback callback) = 0;
};

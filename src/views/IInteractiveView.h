#pragma once

#include "IView.h"

/**
 * @class IInteractiveView
 * @brief Represents any interactive view that can be drawn using the Dear ImGui library.
 *
 * In addition to IView, IInteractiveView contains the ability to enable/disable
 * views when necessary.
 */
class IInteractiveView : public IView {
public:
  virtual ~IInteractiveView() = default;

  /**
   * @brief Disables the View for interaction.
   */
  virtual void enable() = 0;

  /**
   * @brief Disables the View from interaction.
   */
  virtual void disable() = 0;
};

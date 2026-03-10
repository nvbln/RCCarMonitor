#pragma once

#include "IView.h"

#include <string>

/**
 * @class IStatusView
 * @brief Interface for Views with the purpose of displaying a status.
 *
 * The general idea behind the status view is that it is supposed to
 * display changing properties that cannot be interacted with.
 */
class IStatusView : public IView {
public:
  ~IStatusView() = default;

  /**
   * @brief Updates the status value.
   *
   * @param status The status that should be displayed.
   */
  virtual void update(const std::string status) = 0;
};

#pragma once

#include "IStatusView.h"

/**
 * @class StatusView
 * @brief Creates a view of a property (status) that can be updated over time.
 */
class StatusView final : public IStatusView {
public:
  /**
   * @brief Creates an instance of the status view.
   */
  StatusView() {}

  /**
   * @see IStatusView::update()
   */
  void update(const std::string status) override { mStatus = status; }

  /**
   * @brief Draws the view using Dear ImGui
   */
  void draw() override;

private:
  std::string mStatus = "Unknown";
};

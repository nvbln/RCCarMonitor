#pragma once

#include "IInteractiveView.h"

#include <functional>
#include <string>
#include <vector>

/**
 * @class IItemPickerView
 * @brief Interface for Views with the purpose of picking an element from a list.
 *
 * The general flow of the IItemPickerView is that it displays a list of items (strings).
 * These items can be updated at every iteration using the update() function (i.e.
 * if item availability can change during display). Observers interested in getting
 * the selected item can subscribe to the View and receive the index of the item in
 * the list once selected by the user.
 */
class IItemPickerView : public IInteractiveView {
public:
  /**
   * @brief Callback to receive the index of the selected item in the list.
   * @return The index of the selected item.
   */
  using Callback = std::function<void(int)>;

  ~IItemPickerView() = default;

  /**
   * @brief Updates the list of labels to display
   *
   * @param labels The labels to display in the View.
   */
  virtual void update(const std::vector<std::string> labels) = 0;

  /**
   * @brief Subscribes for the item selected by the user.
   *
   * @param callback The callback to call when the user selected an item.
   */
  virtual void subscribe(Callback callback) = 0;
};

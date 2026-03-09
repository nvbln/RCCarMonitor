#pragma once

#include "Event.h"
#include "IItemPickerView.h"

#include <string>
#include <vector>

/**
 * @Class ItemPickerView
 * @brief Creates a simple vertical list of items that can be selected (one at a time).
 */
class ItemPickerView final : public IItemPickerView {
public:
  /**
   * @brief Initialises the picker with the given labels.
   *
   * @param labels The labels to display on the UI.
   */
  ItemPickerView(const std::vector<std::string> labels) : mLabels(labels) {}

  /**
   * @brief Updates the labels (by replacing the old ones) that are displayed in the View.
   *
   * @see IItemPickerView::update()
   * @param labels list of labels to display.
   */
  void update(const std::vector<std::string> labels) override { mLabels = labels; }

  /**
   * @brief Draws the View
   * @see IView::draw()
   */
  void draw() override;

  /**
   * @see IView::enable()
   */
  void enable() override { mDisabled = false; }

  /**
   * @see IView::disable()
   */
  void disable() override { mDisabled = true; }

  /**
   * @brief Subscribes for the item selected by the user.
   *
   * @see IItemPickerView::subscribe()
   * Returns the index of the item that was selected by the user.
   * @param callback The callbck to call when the user selected an item.
   */
  void subscribe(Callback callback) override { event.subscribe(callback); }

private:
  std::vector<std::string> mLabels;
  int mSelectedIdx = -1;
  bool mDisabled = false;

  Event<Callback, int> event;
};

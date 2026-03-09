#pragma once

#include "Event.h"
#include "views/IToggleableView.h"

class ToggleableView : public IToggleableView {
public:
  /**
   * @brief Initialises the ToggleableView with the default value.
   *
   * @param on, true if on, false if off.
   */
  ToggleableView(const bool on) : mOn(on) { mDisabled = false; }

  /**
   * @see IToggleableView::update()
   */
  void update(const bool on) override { mOn = on; }

  /**
   * @see IToggleableView::subscribe()
   */
  void subscribe(Callback callback) override { event.subscribe(callback); }

  /**
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

private:
  bool mOn;
  bool mDisabled;

  Event<Callback, bool> event;
};

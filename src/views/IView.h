#pragma once

/**
 * @class IView
 * @brief Represents any view that can be drawn using the Dear ImGui library.
 *
 * For a consistent interface, any View should inherit IView or a subclass thereof.
 */
class IView {
public:
  virtual ~IView() = default;

  /**
   * @brief Draws the view on the screen using Dear ImGui.
   *
   * Allows all views to be drawn with a simple call to draw().
   */
  virtual void draw() = 0;
};

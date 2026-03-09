#include "imgui.h"

#include "views/ToggleableView.h"

#include <string>

void ToggleableView::draw() {
  ImGui::Begin("Drive Lock");
  if (mDisabled) {
    ImGui::BeginDisabled(mDisabled);
  }
  ImGui::Text("Drive lock:");
  ImGui::SameLine();

  // TODO: Consider renaming.
  std::string buttonText;
  if (mOn) {
    buttonText = "OFF";
  } else {
    buttonText = "ON";
  }
  if (ImGui::Button(buttonText.c_str())) {
    mOn = !mOn;
    event.notify(mOn);
  }

  if (mDisabled) {
    ImGui::EndDisabled();
  }
  ImGui::End();
}

#include "imgui.h"

#include "views/StatusView.h"

#include <string>

void StatusView::draw() {
  ImGui::Begin("Movement status");
  ImGui::Text("Movement Status: ");
  ImGui::SameLine();
  ImGui::TextUnformatted(mStatus.c_str());
  ImGui::End();
}

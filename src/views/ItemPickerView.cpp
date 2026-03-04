#include "imgui.h"

#include "ItemPickerView.h"

void ItemPickerView::draw() {
  ImGui::Begin("Bluetooth devices: ");
  for (int i = 0; i < mLabels.size(); i++) {
    if (ImGui::Selectable(mLabels[i].c_str(), mSelectedIdx == i)) {
      mSelectedIdx = i;
      event.notify(i);
    }
  }
  ImGui::End();
}

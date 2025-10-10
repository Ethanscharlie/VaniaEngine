#include "EntityPanel.hpp"
#include "imgui.h"
#include <format>

namespace Vania {
EntityPanel::EntityPanel() { entities = {"Ethan", "Connor"}; }

void EntityPanel::update() {
  ImGui::Begin("Entity");

  static int selected = 0;
  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0),
                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (int i = 0; i < entities.size(); i++) {
      const std::string &entity = entities[i];
      if (ImGui::Selectable(entity.c_str(), selected == i,
                            ImGuiSelectableFlags_SelectOnNav))
        selected = i;
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();

  ImGui::Text(std::format("Name: {}", entities[selected]).c_str());

  ImGui::End();
}
} // namespace Vania

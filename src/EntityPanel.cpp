#include "EntityPanel.hpp"
#include "EntityDef.hpp"
#include "imgui.h"
#include <format>

namespace Vania {
EntityPanel::EntityPanel() {
  EntityDef ethan;
  ethan.name = "Ethan";
  EntityDef connor;
  connor.name = "Connor";

  entities = {ethan, connor};
}

void EntityPanel::update() {
  ImGui::Begin("Entity");

  static int selected = 0;
  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0),
                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (int i = 0; i < entities.size(); i++) {
      const EntityDef &entity = entities[i];
      if (ImGui::Selectable(entity.name.c_str(), selected == i,
                            ImGuiSelectableFlags_SelectOnNav))
        selected = i;
    }
    ImGui::EndChild();
  }
  ImGui::SameLine();

  ImGui::Text(std::format("Name: {}", entities[selected].name).c_str());

  ImGui::End();
}
} // namespace Vania

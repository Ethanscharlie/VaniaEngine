#include "EntityPanel.hpp"
#include "EntityDef.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cstring>
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

  if (ImGui::Button("   +   ")) {
    entities.push_back({});
  }

  static int selected = 0;
  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0),
                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (int i = 0; i < entities.size(); i++) {
      const EntityDef &entity = entities[i];
      const std::string label =
          std::format("{}###{}n{}", entity.name, entity.name, i);

      if (ImGui::Selectable(label.c_str(), selected == i,
                            ImGuiSelectableFlags_SelectOnNav))
        selected = i;
    }
    ImGui::EndChild();
  }

  ImGui::SameLine();

  ImGui::BeginGroup();
  {
    EntityDef &selectedEntity = entities[selected];
    ImGui::InputText("name", &selectedEntity.name);
    ImGui::InputInt("Width", &selectedEntity.width);
    ImGui::InputInt("Height", &selectedEntity.height);
    ImGui::InputText("Color", &selectedEntity.color);
  }
  ImGui::EndGroup();

  ImGui::End();
}
} // namespace Vania

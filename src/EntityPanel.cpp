#include "EntityPanel.hpp"
#include "GameDataStructs.hpp"
#include "imgui.h"
#include <format>

namespace Vania {
EntityPanel::EntityPanel(GameData &gameData) : gameData(gameData) {
  EntityDef ethan;
  ethan.name = "Ethan";
  EntityDef connor;
  connor.name = "Connor";

  gameData.entityDefs = {ethan, connor};
}

void EntityPanel::update() {
  ImGui::Begin("Entity");

  if (ImGui::Button("   +   ")) {
    gameData.entityDefs.push_back({});
  }

  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0),
                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (int i = 0; i < gameData.entityDefs.size(); i++) {
      EntityDef &entity = gameData.entityDefs[i];
      const std::string label =
          std::format("{}###{}n{}", entity.name, entity.name, i);

      if (ImGui::Selectable(label.c_str(),
                            gameData.editorData.selectedEntityDef == &entity,
                            ImGuiSelectableFlags_SelectOnNav))
        gameData.editorData.selectedEntityDef = &entity;
    }
    ImGui::EndChild();
  }

  // ImGui::SameLine();
  //
  // ImGui::BeginGroup();
  // {
  //   EntityDef &selectedEntity = gameData.entityDefs[selected];
  //   ImGui::InputText("name", &selectedEntity.name);
  //   ImGui::InputInt("Width", &selectedEntity.width);
  //   ImGui::InputInt("Height", &selectedEntity.height);
  //   ImGui::InputText("Color", &selectedEntity.color);
  // }
  // ImGui::EndGroup();

  ImGui::End();
}
} // namespace Vania

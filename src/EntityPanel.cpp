#include "EntityPanel.hpp"
#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
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

  ImGui::SameLine();

  const bool somethingIsSelected = gameData.editorData.selectedEntityDef;
  if (ImGui::Button("Edit") && somethingIsSelected) {
    ImGui::OpenPopup("Entity Def Editor");
  }

  bool defEditorOpen = true;
  if (ImGui::BeginPopupModal("Entity Def Editor", &defEditorOpen)) {
    showPropertyEditor();
    ImGui::EndPopup();
  }

  ImGui::End();
}

void EntityPanel::showPropertyEditor() {
  ImGui::BeginGroup();
  {
    EntityDef &selectedEntity = *gameData.editorData.selectedEntityDef;
    ImGui::InputText("name", &selectedEntity.name);
    ImGui::InputFloat("Width", &selectedEntity.width);
    ImGui::InputFloat("Height", &selectedEntity.height);
    ImGui::InputText("Color", &selectedEntity.color);
    ImGui::InputText("Script", &selectedEntity.script);
  }
  ImGui::EndGroup();
}

} // namespace Vania

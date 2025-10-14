#include "EntityPanel.hpp"
#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

namespace Vania {
EntityPanel::EntityPanel(GameData &gameData) : gameData(gameData) {
  EntityDef player;
  player.name = "Player";
  player.script = "player.lua";
  EntityDef zombie;
  zombie.name = "Zombie";
  zombie.script = "zombie.lua";

  gameData.entityDefs = {player, zombie};
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
  static int SMALL_NUMBER_WIDTH = 100;

  ImGui::BeginGroup();
  EntityDef &selectedEntity = *gameData.editorData.selectedEntityDef;
  ImGui::InputText("name", &selectedEntity.name);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Width", &selectedEntity.width);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Height", &selectedEntity.height);

  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  if (ImGui::BeginCombo("###renderCombo",
                        selectedEntity.imageMode ? "image" : "box")) {
    if (ImGui::Selectable("box", !selectedEntity.imageMode)) {
      selectedEntity.imageMode = false;
    }
    if (ImGui::Selectable("image", selectedEntity.imageMode)) {
      selectedEntity.imageMode = true;
    }
    ImGui::EndCombo();
  }

  ImGui::SameLine();
  if (selectedEntity.imageMode) {
    ImGui::InputText("Image", &selectedEntity.image);
  } else {
    int &r = selectedEntity.r;
    int &g = selectedEntity.g;
    int &b = selectedEntity.b;
    int &a = selectedEntity.a;
    float color_f[4] = {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    if (ImGui::ColorEdit4("###Color", color_f, ImGuiColorEditFlags_NoInputs)) {
      r = color_f[0] * 255.0f;
      g = color_f[1] * 255.0f;
      b = color_f[2] * 255.0f;
      a = color_f[3] * 255.0f;
    }
  }

  ImGui::InputText("Script", &selectedEntity.script);
  ImGui::EndGroup();
}

} // namespace Vania

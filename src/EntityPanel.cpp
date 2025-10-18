#include "EntityPanel.hpp"

#include <format>
#include <fstream>
#include <print>
#include <random>

#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
EntityPanel::EntityPanel(GameData& gameData) : gameData(gameData) {}

void EntityPanel::update() {
  ImGui::Begin("Entity");

  if (ImGui::Button("   +   ")) {
    createDef();
  }

  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (auto& [id, entity] : gameData.entityDefs) {
      const std::string label = std::format("{}###{}n{}", entity.name, entity.name, id);

      if (ImGui::Selectable(label.c_str(), gameData.editorData.selectedEntityDef == &entity,
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

  ImGui::SameLine();

  if (ImGui::Button("Save")) {
    const std::filesystem::path mainPath = gameData.editorData.rootPath / "main.json";
    const nlohmann::json j = gameData;
    std::ofstream file(mainPath);
    file << j.dump(4);
    file.close();
  }

  bool defEditorOpen = true;
  if (ImGui::BeginPopupModal("Entity Def Editor", &defEditorOpen)) {
    showPropertyEditor();
    ImGui::EndPopup();
  }

  ImGui::End();
}

EntityDef& EntityPanel::createDef() {
  int id = rand() % 100000;
  EntityDef newDef;
  newDef.id = id;
  gameData.entityDefs[id] = newDef;
  return gameData.entityDefs.at(id);
}

void EntityPanel::showPropertyEditor() {
  static int SMALL_NUMBER_WIDTH = 100;

  ImGui::BeginGroup();
  EntityDef& selectedEntity = *gameData.editorData.selectedEntityDef;
  ImGui::InputText("name", &selectedEntity.name);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Width", &selectedEntity.width);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Height", &selectedEntity.height);

  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  if (ImGui::BeginCombo("###renderCombo", selectedEntity.imageMode ? "image" : "box")) {
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
    int& r = selectedEntity.r;
    int& g = selectedEntity.g;
    int& b = selectedEntity.b;
    int& a = selectedEntity.a;
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

}  // namespace Vania

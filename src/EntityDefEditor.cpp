#include "EntityDefEditor.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
EntityDefEditor::EntityDefEditor(GameData &gameData) : gameData(gameData) {}

void EntityDefEditor::update() {
  ImGui::Begin("Entity Def Editor");

  if (gameData.editorData.selectedEntityDef != nullptr) {
    showPropertyEditor();
  } else {
    ImGui::Text("No entity is selected");
  }

  ImGui::End();
}

void EntityDefEditor::showPropertyEditor() {
  ImGui::BeginGroup();
  {
    EntityDef &selectedEntity = *gameData.editorData.selectedEntityDef;
    ImGui::InputText("name", &selectedEntity.name);
    ImGui::InputInt("Width", &selectedEntity.width);
    ImGui::InputInt("Height", &selectedEntity.height);
    ImGui::InputText("Color", &selectedEntity.color);
  }
  ImGui::EndGroup();
}

} // namespace Vania

#include "EntityPanel.hpp"

#include <format>
#include <fstream>
#include <print>
#include <random>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "run/AssetManager.hpp"

namespace Vania {
EntityPanel::EntityPanel(GameData &gameData, SDL_Renderer *renderer,
                         FileSystemWatcher &filesystemWatcher)
    : gameData(gameData), renderer(renderer),
      filesystemWatcher(filesystemWatcher) {}

void EntityPanel::update() {
  ImGui::Begin("Entity");

  if (ImGui::Button("   +   ")) {
    *gameData.editorData.selectedEntityDef = createDef();
  }

  {
    ImGui::BeginChild("Entity Selection", ImVec2(150, 0),
                      ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (auto &[id, entity] : gameData.entityDefs) {
      const std::string label =
          std::format("{}###{}n{}", entity.name, entity.name, id);
      const bool isSelected = gameData.editorData.selectedEntityDef == &entity;
      if (ImGui::Selectable(label.c_str(), isSelected,
                            ImGuiSelectableFlags_SelectOnNav)) {
        gameData.editorData.selectedEntityDef = &entity;
      }
    }
    ImGui::EndChild();
  }

  ImGui::SameLine();

  const bool somethingIsSelected = gameData.editorData.selectedEntityDef;

  if (ImGui::Button("Save")) {
    const std::filesystem::path mainPath =
        gameData.editorData.rootPath / "main.json";
    const nlohmann::json j = gameData;
    std::ofstream file(mainPath);
    file << j.dump(4);
    file.close();
  }

  bool defEditorOpen = true;
  if (ImGui::BeginPopupModal("Entity Def Editor", &defEditorOpen)) {
    // showPropertyEditor();
    ImGui::EndPopup();
  }

  ImGui::End();
}

EntityDef &EntityPanel::createDef() {
  int id = rand() % 100000;
  EntityDef newDef;
  newDef.id = id;
  gameData.entityDefs[id] = newDef;
  return gameData.entityDefs.at(id);
}

} // namespace Vania

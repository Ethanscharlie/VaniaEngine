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
EntityPanel::EntityPanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData), renderer(renderer) {}

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
    if (ImGui::Button("Set Image")) {
      cellWidth = 1;
      cellHeight = 1;
      ImGui::OpenPopup("Image Picker");
    }
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

  bool b = true;
  if (ImGui::BeginPopupModal("Image Picker", &b)) {
    showImagePicker();
    ImGui::EndPopup();
  }

  ImGui::InputText("Script", &selectedEntity.script);
  ImGui::EndGroup();
}

void EntityPanel::showImagePicker() {
  EntityDef& selectedEntity = *gameData.editorData.selectedEntityDef;
  ImGui::InputText("Image", &selectedEntity.image);

  static float zoom = 1;
  static int gridSize = 32;

  ImGui::SliderFloat("Zoom", &zoom, 1, 10);
  ImGui::InputInt("Grid Size", &gridSize);

  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputInt("Cell Width", &cellWidth);
  ImGui::SameLine();
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputInt("Cell Height", &cellHeight);

  auto& root = gameData.editorData.rootPath;
  AssetManager& assetManager = AssetManager::getInstance();
  SDL_Texture* texture = assetManager.get(renderer, root / selectedEntity.image);

  float width, height;
  SDL_GetTextureSize(texture, &width, &height);
  width *= zoom;
  height *= zoom;

  float tile_size_x = gridSize * zoom * cellWidth;
  float tile_size_y = gridSize * zoom * cellHeight;

  int cols = static_cast<int>(width) / tile_size_x;
  int rows = static_cast<int>(height) / tile_size_y;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      ImVec2 position(col * tile_size_x, row * tile_size_y);

      ImVec2 uv0(col * tile_size_x / width, row * tile_size_y / height);
      ImVec2 uv1((col + 1) * tile_size_x / width, (row + 1) * tile_size_y / height);

      std::string buttonID = std::format("{},{}", row, col);
      const bool buttonPressed =
          ImGui::ImageButton(buttonID.c_str(), texture, ImVec2(tile_size_x, tile_size_y), uv0, uv1);

      if (buttonPressed) {
        selectedEntity.imageWidth = gridSize * cellWidth;
        selectedEntity.imageHeight = gridSize * cellHeight;
        selectedEntity.imageRow = row * tile_size_x;
        selectedEntity.imageCol = col * tile_size_y;
        ImGui::CloseCurrentPopup();
      }

      ImGui::SameLine();
    }

    ImGui::NewLine();
  }
}

}  // namespace Vania

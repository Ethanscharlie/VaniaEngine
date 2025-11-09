#include "InspectorPanel.hpp"

#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <print>

#include "FilesystemWatcher.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "run/AssetManager.hpp"

namespace Vania {
InspectorPanel::InspectorPanel(EditorContext& context) : context(context), vaniaRenderer(context) {}

void InspectorPanel::update() {
  ImGui::Begin("Inspector");
  if (context.gameData.editorData.selectedEntityDef != nullptr) showPropertyEditor();
  ImGui::End();
}

void InspectorPanel::showPropertyEditor() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;

  showSample();

  ImGui::InputText("name", &selectedEntity.name);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Width", &selectedEntity.width);
  ImGui::SetNextItemWidth(SMALL_NUMBER_WIDTH);
  ImGui::InputFloat("Height", &selectedEntity.height);

  ImGui::SliderFloat("Center Offset X", &selectedEntity.centerOffsetX, -selectedEntity.width, selectedEntity.width);
  ImGui::SliderFloat("Center Offset Y", &selectedEntity.centerOffsetY, -selectedEntity.height, selectedEntity.height);

  if (ImGui::CollapsingHeader("Graphic", ImGuiTreeNodeFlags_DefaultOpen)) showGraphic();
  if (ImGui::CollapsingHeader("Behaviour", ImGuiTreeNodeFlags_DefaultOpen)) showBehaviour();
  if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen)) showCollision();
}

void InspectorPanel::showSample() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;
  auto* draw_list = ImGui::GetWindowDrawList();

  // Background
  const ImVec2 backgroundMin = ImGui::GetCursorScreenPos();
  ImVec2 backgroundSize = ImGui::GetContentRegionAvail();
  if (backgroundSize.x < 50.0f) backgroundSize.x = 50.0f;
  if (backgroundSize.x > 300.0f) backgroundSize.x = 300.0f;
  backgroundSize.y = backgroundSize.x;
  const ImVec2 backgroundMax = {backgroundMin.x + backgroundSize.x, backgroundMin.y + backgroundSize.y};

  draw_list->AddRectFilled(backgroundMin, backgroundMax, ImGui::ColorConvertFloat4ToU32({0.1, 0.1, 0.1, 1}));

  // Entity
  const float innerSize = backgroundSize.x / 2;
  float largestSide = std::max(selectedEntity.width, selectedEntity.height);
  const float scale = innerSize / largestSide;
  const SDL_FPoint center = {backgroundMin.x + backgroundSize.x / 2, backgroundMin.y + backgroundSize.y / 2};

  vaniaRenderer.drawEntity(selectedEntity, center, scale);

  const SDL_FPoint crossMarkerCenter = {
      center.x + selectedEntity.centerOffsetX * scale,  //
      center.y + selectedEntity.centerOffsetY * scale   //
  };
  vaniaRenderer.drawCross(crossMarkerCenter, 15, 5, {0, 0, 0, 255});
  vaniaRenderer.drawCross(crossMarkerCenter, 10, 2, {0, 255, 255, 255});

  // Adjust cursor to after
  ImVec2 newPos = ImGui::GetCursorPos();
  newPos.y += backgroundSize.y + 20;
  ImGui::SetCursorPos(newPos);
}

void InspectorPanel::showGraphic() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;

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
}

void InspectorPanel::showBehaviour() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;

  bool willCreateScript = false;
  const std::string previewValue = (selectedEntity.script != "") ? selectedEntity.script : "Select Script";
  if (ImGui::BeginCombo("ScriptSelectionCombo", previewValue.c_str())) {
    if (ImGui::Selectable("None")) {
      selectedEntity.script = "";
    }

    if (ImGui::Selectable("[+] Create Script")) {
      willCreateScript = true;
    }

    const auto& scripts = context.filesystemWatcher.getAllFilesWithExtension(".lua");
    for (const auto& script : scripts) {
      if (ImGui::Selectable(script.c_str(), script == selectedEntity.script)) {
        selectedEntity.script = script;
      }
    }
    ImGui::EndCombo();
  }

  if (willCreateScript) ImGui::OpenPopup("Script Creator");

  if (ImGui::BeginPopup("Script Creator")) {
    showScriptCreator();
    ImGui::EndPopup();
  }
}

void InspectorPanel::showCollision() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;

  if (ImGui::BeginCombo("###colliderCombo", selectedEntity.colliderType.c_str())) {
    if (ImGui::Selectable("rect")) selectedEntity.colliderType = "rect";
    if (ImGui::Selectable("circle")) selectedEntity.colliderType = "circle";
    ImGui::EndCombo();
  }

  float largestSide = (selectedEntity.width > selectedEntity.height) ? selectedEntity.width : selectedEntity.height;
  ImGui::DragFloat("Width Fraction", &selectedEntity.colliderWidthFraction, 0.01, 0.1);
  if (selectedEntity.colliderType != "circle") {
    ImGui::DragFloat("Height Fraction", &selectedEntity.colliderHeightFraction, 0.01, 0.1);
  }
}

void InspectorPanel::showImagePicker() {
  EntityDef& selectedEntity = *context.gameData.editorData.selectedEntityDef;

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

  auto& root = context.gameData.editorData.rootPath;
  AssetManager& assetManager = AssetManager::getInstance();
  SDL_Texture* texture = assetManager.get(context.renderer, root / selectedEntity.image);

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

void InspectorPanel::showScriptCreator() {
  ImGui::Text("New script");
  ImGui::InputText("###New Script Name", &scriptCreatorTempText);
  ImGui::SameLine();
  if (ImGui::Button("Enter###EnterScriptCreator")) {
    createScriptAndAttach(std::format("{}.lua", scriptCreatorTempText));
    scriptCreatorTempText = "";
    ImGui::CloseCurrentPopup();
  }
}

void InspectorPanel::createScriptAndAttach(const std::filesystem::path& newScript) {
  context.gameData.editorData.selectedEntityDef->script = newScript;

  std::ofstream file(context.gameData.editorData.rootPath / newScript);
  file << "local function setup(entity)" << "\n";
  file << "end" << "\n";
  file << "\n";
  file << "local function update(entity, deltaTime)" << "\n";
  file << "end" << "\n";
  file << "\n";
  file << "return {" << "\n";
  file << "	setup = setup," << "\n";
  file << "	update = update," << "\n";
  file << "}" << "\n";
  file << "\n";
  file.close();
}

}  // namespace Vania

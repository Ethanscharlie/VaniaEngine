
#include "AssetPanel.hpp"

#include <format>

#include "GameDataStructs.hpp"
#include "SDL3/SDL.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "run/AssetManager.hpp"

namespace Vania {
AssetPanel::AssetPanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData), renderer(renderer) {}

void AssetPanel::update() {
  ImGui::Begin("Assets");

  ImGui::SliderFloat("Zoom", &zoom, 1, 10);
  ImGui::InputInt("Grid Size", &gridSize);

  auto& root = gameData.editorData.rootPath;
  AssetManager& assetManager = AssetManager::getInstance();
  SDL_Texture* texture = assetManager.get(renderer, root / "sheet.png");

  float width, height;
  SDL_GetTextureSize(texture, &width, &height);
  width *= zoom;
  height *= zoom;

  float tile_size = gridSize * zoom;

  int cols = static_cast<int>(width) / tile_size;
  int rows = static_cast<int>(height) / tile_size;

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      ImVec2 position(col * tile_size, row * tile_size);

      ImVec2 uv0(col * tile_size / width, row * tile_size / height);
      ImVec2 uv1((col + 1) * tile_size / width, (row + 1) * tile_size / height);

      ImGui::Image(texture, ImVec2(tile_size, tile_size), uv0, uv1);
      ImGui::SameLine();
    }

    ImGui::NewLine();
  }

  ImGui::End();
}

}  // namespace Vania

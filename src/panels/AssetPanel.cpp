
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

  auto& root = gameData.editorData.rootPath;
  AssetManager& assetManager = AssetManager::getInstance();
  SDL_Texture* texture = assetManager.get(renderer, root / "sheet.png");

  float width, height;
  SDL_GetTextureSize(texture, &width, &height);
  width *= zoom;
  height *= zoom;

  ImGui::Image(texture, ImVec2(width, height));

  ImGui::End();
}

}  // namespace Vania

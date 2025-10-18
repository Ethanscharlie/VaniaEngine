
#include "AssetPanel.hpp"

#include <format>

#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
AssetPanel::AssetPanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData) {}

void AssetPanel::update() {
  ImGui::Begin("Assets");
  ImGui::End();
}

}  // namespace Vania

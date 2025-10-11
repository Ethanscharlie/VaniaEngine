#include "WorldPanel.hpp"

#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

namespace Vania {
WorldPanel::WorldPanel(GameData &gameData) : gameData(gameData) {}

void WorldPanel::update() {
  ImGui::Begin("World");

  ImGui::End();
}
} // namespace Vania

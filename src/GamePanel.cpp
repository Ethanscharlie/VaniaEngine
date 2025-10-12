#include "GamePanel.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_oldnames.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

namespace Vania {
GamePanel::GamePanel(GameData &gameData, SDL_Renderer *renderer)
    : gameData(gameData), runner(gameData, renderer) {}

void GamePanel::update() {
  if (running) {
    runner.update();
  }

  ImGui::Begin("Game Runner");
  ImGui::Image((ImTextureID)(intptr_t)runner.displayTexture,
               ImVec2(runner.DISPLAY_WIDTH, runner.DISPLAY_HEIGHT));
  ImGui::End();
}

} // namespace Vania

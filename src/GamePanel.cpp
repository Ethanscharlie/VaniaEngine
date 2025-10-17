#include "GamePanel.hpp"

#include <format>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_oldnames.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
GamePanel::GamePanel(nlohmann::json& gameData, SDL_Renderer* renderer)
    : gameData(gameData), runner(gameData, renderer) {}

void GamePanel::update() {
  if (running) {
    runner.update();
  }

  ImGui::Begin("Game Runner");

  if (ImGui::Button("Run")) {
    running = true;
    runner.reset(gameData);
    runner.runAllScriptsSetups();
  }

  ImGui::SameLine();

  if (ImGui::Button("Stop")) {
    running = false;
    runner.reset(gameData);
  }

  ImGui::SameLine();

  ImGui::Text((running) ? "Running..." : "Stopped.");

  ImGui::Image((ImTextureID)(intptr_t)runner.displayTexture, ImVec2(runner.DISPLAY_WIDTH, runner.DISPLAY_HEIGHT));
  ImGui::End();
}

}  // namespace Vania

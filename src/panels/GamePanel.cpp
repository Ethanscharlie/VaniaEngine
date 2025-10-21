#include "GamePanel.hpp"

#include <format>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_oldnames.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
GamePanel::GamePanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData), runner(gameData, renderer) {}

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

  ImVec2 avail_size = ImGui::GetContentRegionAvail();
  float aspect_ratio = (float)runner.DISPLAY_WIDTH / runner.DISPLAY_HEIGHT;
  float scale_x = avail_size.x / runner.DISPLAY_WIDTH;
  float scale_y = avail_size.y / runner.DISPLAY_HEIGHT;
  float scale = std::min(scale_x, scale_y);

  const ImVec2 newSize = {runner.DISPLAY_WIDTH * scale, runner.DISPLAY_HEIGHT * scale};
  ImGui::Image(runner.displayTexture, newSize);
  ImGui::End();
}

}  // namespace Vania

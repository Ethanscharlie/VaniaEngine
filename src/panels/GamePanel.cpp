#include "GamePanel.hpp"

#include <format>
#include <print>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_rect.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
GamePanel::GamePanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData), runner(gameData, renderer) {}

void GamePanel::update() {
  ImGui::Begin("Game Runner");

  if (running) {
    runner.updateMousePos(getMousePosOnGame());
    runner.update();
  }

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
  gameWindowSize = {runner.DISPLAY_WIDTH * scale, runner.DISPLAY_HEIGHT * scale};

  ImGui::Image(runner.displayTexture, gameWindowSize);

  ImGui::End();
}

SDL_FPoint GamePanel::getMousePosOnGame() {
  SDL_FPoint mousePos;
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  const ImVec2 gamePanelPos = ImGui::GetWindowPos();
  mousePos.x -= gamePanelPos.x;
  mousePos.y -= gamePanelPos.y;

  const float scale = gameWindowSize.x / runner.DISPLAY_WIDTH;
  mousePos.x /= scale;
  mousePos.y /= scale;

  return mousePos;
}

}  // namespace Vania

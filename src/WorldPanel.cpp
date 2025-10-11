#include "WorldPanel.hpp"

#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <cmath>
#include <format>

namespace Vania {
WorldPanel::WorldPanel(GameData &gameData) : gameData(gameData) {}

void WorldPanel::update() {
  ImGui::Begin("World");

  calculateCanvasPositionValues();

  ImGui::InvisibleButton("canvas", canvas_sz,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonRight);

  const bool is_active = ImGui::IsItemActive();
  if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f)) {
    ImGuiIO &io = ImGui::GetIO();
    scrolling.x += io.MouseDelta.x;
    scrolling.y += io.MouseDelta.y;
  }

  draw();

  ImGui::End();
}

ImVec2 WorldPanel::getMousePositionOnCanvas() {
  ImGuiIO &io = ImGui::GetIO();
  const ImVec2 origin = {canvas_p0.x + scrolling.x,
                         canvas_p0.y + scrolling.y}; // Lock scrolled origin
  return {io.MousePos.x - origin.x, io.MousePos.y - origin.y};
}

void WorldPanel::calculateCanvasPositionValues() {
  // Using InvisibleButton() as a convenience 1) it will advance the layout
  // cursor and 2) allows us to use IsItemHovered()/IsItemActive()
  canvas_p0 =
      ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
  canvas_sz =
      ImGui::GetContentRegionAvail(); // Resize canvas to what's available
  if (canvas_sz.x < 50.0f)
    canvas_sz.x = 50.0f;
  if (canvas_sz.y < 50.0f)
    canvas_sz.y = 50.0f;
  canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
}

void WorldPanel::drawGrid() {

  for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                       ImVec2(canvas_p0.x + x, canvas_p1.y), LIGHT_GRAY);
  for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                       ImVec2(canvas_p1.x, canvas_p0.y + y), LIGHT_GRAY);
}

void WorldPanel::draw() {
  draw_list = ImGui::GetWindowDrawList(); // ImGui is annoying

  draw_list->AddRectFilled(canvas_p0, canvas_p1, DARK_GRAY);
  draw_list->AddRect(canvas_p0, canvas_p1, WHITE);

  // Draw grid + all lines in the canvas
  draw_list->PushClipRect(canvas_p0, canvas_p1, true);
  drawGrid();
  draw_list->PopClipRect();
}

} // namespace Vania

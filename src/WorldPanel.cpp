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

  // This will catch our interactions
  ImGui::InvisibleButton("canvas", canvas_sz,
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonRight);

  findWindowInputState();

  if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f)) {
    ImGuiIO &io = ImGui::GetIO();
    scrolling.x += io.MouseDelta.x;
    scrolling.y += io.MouseDelta.y;
  }

  draw();

  ImGui::End();
}

void WorldPanel::findWindowInputState() {
  ImGuiIO &io = ImGui::GetIO();
  is_hovered = ImGui::IsItemHovered(); // Hovered
  is_active = ImGui::IsItemActive();   // Held
  origin = {canvas_p0.x + scrolling.x,
            canvas_p0.y + scrolling.y}; // Lock scrolled origin
  mouse_pos_in_canvas = {io.MousePos.x - origin.x, io.MousePos.y - origin.y};
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

void WorldPanel::draw() {
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
  draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

  // Draw grid + all lines in the canvas
  draw_list->PushClipRect(canvas_p0, canvas_p1, true);
  const float GRID_STEP = 64.0f;
  for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                       ImVec2(canvas_p0.x + x, canvas_p1.y),
                       IM_COL32(200, 200, 200, 40));
  for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                       ImVec2(canvas_p1.x, canvas_p0.y + y),
                       IM_COL32(200, 200, 200, 40));
  draw_list->PopClipRect();
}

} // namespace Vania

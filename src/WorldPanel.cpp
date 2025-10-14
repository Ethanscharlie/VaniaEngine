#include "WorldPanel.hpp"

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "run/AssetManager.hpp"
#include <cmath>
#include <format>

namespace Vania {
WorldPanel::WorldPanel(GameData &gameData, SDL_Renderer *renderer)
    : gameData(gameData), renderer(renderer) {
  gameData.worldData.entities.push_back({&gameData.entityDefs[0], 0, 0});
}

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
  const ImVec2 origin = getOrigin();
  return {io.MousePos.x - origin.x, io.MousePos.y - origin.y};
}

ImVec2 WorldPanel::getOrigin() {
  return {canvas_p0.x + scrolling.x,
          canvas_p0.y + scrolling.y}; // Lock scrolled origin
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

  for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
       x += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                       ImVec2(canvas_p0.x + x, canvas_p1.y), LIGHT_GRAY);
  }

  for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
       y += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                       ImVec2(canvas_p1.x, canvas_p0.y + y), LIGHT_GRAY);
  }
}

void WorldPanel::draw() {
  draw_list = ImGui::GetWindowDrawList(); // ImGui is annoying

  draw_list->AddRectFilled(canvas_p0, canvas_p1, DARK_GRAY);
  draw_list->AddRect(canvas_p0, canvas_p1, WHITE);

  draw_list->PushClipRect(canvas_p0, canvas_p1, true);

  drawGrid();

  const ImVec2 origin = getOrigin();
  for (const Entity &entity : gameData.worldData.entities) {
    const float x = entity.x + origin.x;
    const float y = entity.y + origin.y;
    const float w = entity.entityDef->width;
    const float h = entity.entityDef->height;

    const ImVec2 min = {x, y};
    const ImVec2 max = {x + w, y + h};

    if (entity.entityDef->imageMode) {
      auto &root = gameData.editorData.rootPath;
      const std::string &image = entity.entityDef->image;
      drawImage(min, max, root / image);
    } else {
      drawBox(min, max, entity.entityDef->r, entity.entityDef->g,
              entity.entityDef->b, entity.entityDef->a);
    }
  }

  draw_list->PopClipRect();
}

void WorldPanel::drawImage(const ImVec2 &min, const ImVec2 &max,
                           const std::string &fullpath) {
  AssetManager &assetManager = AssetManager::getInstance();
  SDL_Texture *texture = assetManager.get(renderer, fullpath);
  if (texture == nullptr)
    return;
  draw_list->AddImage((ImTextureID)(intptr_t)texture, min, max);
}

void WorldPanel::drawBox(const ImVec2 &min, const ImVec2 &max, int r, int g,
                         int b, int a) {
  draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a));
}

} // namespace Vania

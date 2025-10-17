#include "WorldPanel.hpp"

#include <cmath>
#include <format>
#include <fstream>
#include <nlohmann/json_fwd.hpp>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "nlohmann/detail/json_pointer.hpp"
#include "run/AssetManager.hpp"

namespace Vania {
WorldPanel::WorldPanel(nlohmann::json& gameData, SDL_Renderer* renderer) : gameData(gameData), renderer(renderer) {
  gameData["entities"].push_back(createEntity("13", 0, 0));
}

void WorldPanel::update() {
  ImGui::Begin("World");

  calculateCanvasPositionValues();

  ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

  const bool is_active = ImGui::IsItemActive();
  if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f)) {
    ImGuiIO& io = ImGui::GetIO();
    scrolling.x += io.MouseDelta.x;
    scrolling.y += io.MouseDelta.y;
  }

  draw();

  ImGui::End();
}

ImVec2 WorldPanel::getMousePositionOnCanvas() {
  ImGuiIO& io = ImGui::GetIO();
  const ImVec2 origin = getOrigin();
  return {io.MousePos.x - origin.x, io.MousePos.y - origin.y};
}

ImVec2 WorldPanel::getOrigin() {
  return {canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y};  // Lock scrolled origin
}

void WorldPanel::calculateCanvasPositionValues() {
  // Using InvisibleButton() as a convenience 1) it will advance the layout
  // cursor and 2) allows us to use IsItemHovered()/IsItemActive()
  canvas_p0 = ImGui::GetCursorScreenPos();     // ImDrawList API uses screen coordinates!
  canvas_sz = ImGui::GetContentRegionAvail();  // Resize canvas to what's available
  if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
  if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
  canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
}

void WorldPanel::drawGrid() {
  for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), LIGHT_GRAY);
  }

  for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP) {
    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), LIGHT_GRAY);
  }
}

void WorldPanel::draw() {
  draw_list = ImGui::GetWindowDrawList();  // ImGui is annoying

  draw_list->AddRectFilled(canvas_p0, canvas_p1, DARK_GRAY);
  draw_list->AddRect(canvas_p0, canvas_p1, WHITE);

  draw_list->PushClipRect(canvas_p0, canvas_p1, true);

  drawGrid();

  const ImVec2 origin = getOrigin();
  for (const nlohmann::json& entity : gameData["entities"]) {
    const nlohmann::json& def = gameData["defs"][entity["defId"]];
    const float x = entity["x"].get<float>() + origin.x;
    const float y = entity["y"].get<float>() + origin.y;
    const float w = def["width"];
    const float h = def["height"];

    const ImVec2 min = {x, y};
    const ImVec2 max = {x + w, y + h};

    if (def["imageMode"]) {
      std::filesystem::path root = gameData["editor"]["rootPath"];
      const std::string& image = def["image"];
      drawImage(min, max, root / image);
    } else {
      drawBox(min, max, def["r"], def["g"], def["b"], def["a"]);
    }
  }

  draw_list->PopClipRect();
}

void WorldPanel::drawImage(const ImVec2& min, const ImVec2& max, const std::string& fullpath) {
  AssetManager& assetManager = AssetManager::getInstance();
  SDL_Texture* texture = assetManager.get(renderer, fullpath);
  if (texture == nullptr) {
    drawNoImage(min, max);
    return;
  }
  draw_list->AddImage((ImTextureID)(intptr_t)texture, min, max);
}

void WorldPanel::drawNoImage(const ImVec2& min, const ImVec2& max) {
  draw_list->AddRectFilled(min, max, IM_COL32(0, 0, 0, 255));
  draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
  draw_list->AddLine(min, max, IM_COL32(255, 0, 0, 255));
}

void WorldPanel::drawBox(const ImVec2& min, const ImVec2& max, int r, int g, int b, int a) {
  draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a));
}

}  // namespace Vania

#include "WorldPanel.hpp"

#include <cmath>
#include <format>
#include <print>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "run/AssetManager.hpp"

namespace Vania {
WorldPanel::WorldPanel(GameData& gameData, SDL_Renderer* renderer) : gameData(gameData), renderer(renderer) {}

void WorldPanel::update() {
  ImGui::Begin("World");
  ImGui::SliderFloat("Zoom", &zoom, 1, 10);

  const float gridSize = gameData.worldData.gridSize;
  const float gridSizeWithZoom = gameData.worldData.gridSize * zoom;

  calculateCanvasPositionValues();

  ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

  const bool is_active = ImGui::IsItemActive();
  if (is_active) {
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f)) {
      ImGuiIO& io = ImGui::GetIO();
      scrolling.x += io.MouseDelta.x;
      scrolling.y += io.MouseDelta.y;
    }

    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      const ImVec2 mousePos = getMousePositionOnCanvas();
      const float snapedX = (snapPositionToGrid(mousePos.x / zoom) + gridSize / 2);
      const float snapedY = (snapPositionToGrid(mousePos.y / zoom) + gridSize / 2);
      createEntity(snapedX, snapedY);
    }
  }

  draw();

  ImGui::End();
}

ImVec2 WorldPanel::getMousePositionOnCanvas() {
  ImGuiIO& io = ImGui::GetIO();
  const ImVec2 origin = getOrigin();
  return {
      io.MousePos.x - origin.x,  //
      io.MousePos.y - origin.y   //
  };
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
  const float gridSize = gameData.worldData.gridSize;
  const float gridSizeWithZoom = gameData.worldData.gridSize * zoom;

  for (float x = fmodf(scrolling.x, gridSizeWithZoom); x < canvas_sz.x; x += gridSizeWithZoom) {
    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), LIGHT_GRAY);
  }

  for (float y = fmodf(scrolling.y, gridSizeWithZoom); y < canvas_sz.y; y += gridSizeWithZoom) {
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
  for (const Entity& entity : gameData.worldData.entities) {
    const EntityDef& def = gameData.entityDefs.at(entity.defID);
    const float zoomedWidth = def.width * zoom;
    const float zoomedHeight = def.width * zoom;
    const float x = entity.x * zoom + origin.x - zoomedWidth / 2;
    const float y = entity.y * zoom + origin.y - zoomedHeight / 2;
    const float w = zoomedWidth;
    const float h = zoomedHeight;

    const ImVec2 min = {x, y};
    const ImVec2 max = {x + w, y + h};

    if (def.imageMode) {
      auto& root = gameData.editorData.rootPath;
      const std::string& image = def.image;
      AssetManager& assetManager = AssetManager::getInstance();
      SDL_Texture* texture = assetManager.get(renderer, root / image);

      float textureW, textureH;
      SDL_GetTextureSize(texture, &textureW, &textureH);

      ImVec2 uv0 = {
          def.imageCol / textureW,  //
          def.imageRow / textureH   //
      };

      ImVec2 uv1 = {
          (def.imageCol + def.imageWidth) / textureW,  //
          (def.imageRow + def.imageHeight) / textureH  //
      };

      if (texture == nullptr) {
        drawNoImage(min, max);
        return;
      }

      draw_list->AddImage((ImTextureID)(intptr_t)texture, min, max, uv0, uv1);
    } else {
      drawBox(min, max, def.r, def.g, def.b, def.a);
    }
  }

  draw_list->PopClipRect();
}

void WorldPanel::drawNoImage(const ImVec2& min, const ImVec2& max) {
  draw_list->AddRectFilled(min, max, IM_COL32(0, 0, 0, 255));
  draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
  draw_list->AddLine(min, max, IM_COL32(255, 0, 0, 255));
}

void WorldPanel::drawBox(const ImVec2& min, const ImVec2& max, int r, int g, int b, int a) {
  draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a));
}

float WorldPanel::snapPositionToGrid(float x) {
  const int gridSize = gameData.worldData.gridSize;
  return floor(x / gridSize) * gridSize;
}

void WorldPanel::createEntity(float x, float y) {
  EntityDef* def = gameData.editorData.selectedEntityDef;
  if (def == nullptr) return;
  gameData.worldData.entities.push_back({def->id, x, y});
}

}  // namespace Vania

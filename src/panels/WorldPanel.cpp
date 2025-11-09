#include "WorldPanel.hpp"

#include <pstl/glue_algorithm_defs.h>

#include <cmath>
#include <cstdio>

#include "EditorContext.hpp"
#include "GameDataStructs.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Vania {
WorldPanel::WorldPanel(EditorContext& context) : context(context), vaniaRenderer(context) {}

void WorldPanel::update() {
  ImGui::Begin("World");
  ImGui::SliderFloat("Zoom", &zoom, 1, 10);

  calculateCanvasPositionValues();
  ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

  if (ImGui::IsItemActive()) whileActive();

  draw();

  Entity* hoveredEntity = isHoveringOverEntity();
  if (hoveredEntity != nullptr) {
    drawHoverBox(*hoveredEntity);
  } else {
    drawGhostAtCursor();
  }

  ImGui::End();
}

void WorldPanel::whileActive() {
  const float gridSize = context.gameData.worldData.gridSize;
  const float gridSizeWithZoom = context.gameData.worldData.gridSize * zoom;

  Entity* hoveredEntity = isHoveringOverEntity();

  if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, -1.0f)) {
    ImGuiIO& io = ImGui::GetIO();
    scrolling.x += io.MouseDelta.x;
    scrolling.y += io.MouseDelta.y;
  }

  else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    if (hoveredEntity != nullptr) {  // Select entity
      removeEntity(*hoveredEntity);
    }

    else {  // Create new Entity
      const ImVec2 mousePos = getMousePositionOnCanvas();
      const float snapedX = (snapPositionToGrid(mousePos.x) + gridSize / 2);
      const float snapedY = (snapPositionToGrid(mousePos.y) + gridSize / 2);
      createEntity(snapedX, snapedY);
    }
  }
}

void WorldPanel::drawHoverBox(const Entity& entity) {
  const ImVec4 minAndMax = getEntityMinAndMax(entity);
  const ImVec2 min = {
      minAndMax.x - HOVER_BOX_PADDING,  //
      minAndMax.y - HOVER_BOX_PADDING   //
  };
  const ImVec2 max = {
      minAndMax.z + HOVER_BOX_PADDING,  //
      minAndMax.w + HOVER_BOX_PADDING   //
  };

  draw_list->AddRect(min, max, RED);
}

void WorldPanel::drawGhostAtCursor() {
  EntityDef* defPtr = context.gameData.editorData.selectedEntityDef;
  if (defPtr == nullptr) return;

  const float gridSize = context.gameData.worldData.gridSize;
  const ImVec2 mousePos = getMousePositionOnCanvas();
  const float snapedX = (snapPositionToGrid(mousePos.x) + gridSize / 2);
  const float snapedY = (snapPositionToGrid(mousePos.y) + gridSize / 2);

  Entity ghost = {0, defPtr->id, snapedX, snapedY};

  const ImVec4 minAndMax = getEntityMinAndMax(ghost);
  const ImVec2 min = {minAndMax.x, minAndMax.y};
  const ImVec2 max = {minAndMax.z, minAndMax.w};
  const SDL_FPoint center = {
      min.x + (max.x - min.x) / 2,  //
      min.y + (max.y - min.y) / 2,  //
  };

  EntityDef def = *context.gameData.editorData.selectedEntityDef;
  def.a = GHOST_ALPHA;
  vaniaRenderer.drawEntity(def, center, zoom);
}

ImVec2 WorldPanel::getMousePositionOnCanvas() {
  ImGuiIO& io = ImGui::GetIO();
  const ImVec2 origin = getOrigin();
  return {
      (io.MousePos.x - origin.x) / zoom,  //
      (io.MousePos.y - origin.y) / zoom   //
  };
}

ImVec2 WorldPanel::getOrigin() {
  return {canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y};  // Lock scrolled origin
}

Entity* WorldPanel::isHoveringOverEntity() {
  ImVec2 mousePos = getMousePositionOnCanvas();

  for (Entity& entity : context.gameData.worldData.entities) {
    const EntityDef& def = context.gameData.entityDefs.at(entity.defID);
    const float w = def.width;
    const float h = def.height;
    const float x = entity.x - w / 2;
    const float y = entity.y - h / 2;
    if (mousePos.x < x + w && mousePos.x > x && mousePos.y < y + h && mousePos.y > y) {
      return &entity;
    }
  }

  return nullptr;
}

ImVec4 WorldPanel::getEntityMinAndMax(const Entity& entity) {
  const EntityDef& def = context.gameData.entityDefs.at(entity.defID);
  const ImVec2 origin = getOrigin();
  const float zoomedWidth = def.width * zoom;
  const float zoomedHeight = def.height * zoom;
  const float x = entity.x * zoom + origin.x - zoomedWidth / 2;
  const float y = entity.y * zoom + origin.y - zoomedHeight / 2;
  const float w = zoomedWidth;
  const float h = zoomedHeight;
  return {x, y, x + w, y + h};
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
  const float gridSize = context.gameData.worldData.gridSize;
  const float gridSizeWithZoom = context.gameData.worldData.gridSize * zoom;

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
  for (const Entity& entity : context.gameData.worldData.entities) {
    const ImVec4 minAndMax = getEntityMinAndMax(entity);
    const ImVec2 min = {minAndMax.x, minAndMax.y};
    const ImVec2 max = {minAndMax.z, minAndMax.w};
    const EntityDef& def = context.gameData.entityDefs.at(entity.defID);

    const SDL_FPoint center = {
        min.x + (max.x - min.x) / 2,  //
        min.y + (max.y - min.y) / 2,  //
    };

    vaniaRenderer.drawEntity(def, center, zoom);
    vaniaRenderer.drawCollider(def, center, zoom);
  }

  draw_list->PopClipRect();
}

float WorldPanel::snapPositionToGrid(float x) {
  const int gridSize = context.gameData.worldData.gridSize;
  return floor(x / gridSize) * gridSize;
}

void WorldPanel::createEntity(float x, float y) {
  EntityDef* def = context.gameData.editorData.selectedEntityDef;
  if (def == nullptr) return;
  int id = rand() % 100000;
  context.gameData.worldData.entities.push_back({id, def->id, x, y});
}

void WorldPanel::removeEntity(Entity& entity) {
  auto& entities = context.gameData.worldData.entities;
  auto it = std::remove(entities.begin(), entities.end(), entity);
  entities.erase(it, entities.end());
}

}  // namespace Vania

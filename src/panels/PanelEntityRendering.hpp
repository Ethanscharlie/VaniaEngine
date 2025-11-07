#pragma once

#include "EditorContext.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "run/AssetManager.hpp"

static void drawNoImage(const ImVec2& min, const ImVec2& max) {
  auto* draw_list = ImGui::GetWindowDrawList();  // ImGui is annoying
  draw_list->AddRectFilled(min, max, IM_COL32(0, 0, 0, 255));
  draw_list->AddRect(min, max, IM_COL32(255, 0, 0, 255));
  draw_list->AddLine(min, max, IM_COL32(255, 0, 0, 255));
}

static void drawBox(const ImVec2& min, const ImVec2& max, int r, int g, int b, int a) {
  auto* draw_list = ImGui::GetWindowDrawList();  // ImGui is annoying
  draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a));
}

inline void renderEntityOnPanel(EditorContext& context, const EntityDef& def, ImVec2 min, ImVec2 max, int alpha = 255) {
  if (def.imageMode) {
    auto& root = context.gameData.editorData.rootPath;
    const std::string& image = def.image;
    AssetManager& assetManager = AssetManager::getInstance();
    SDL_Texture* texture = assetManager.get(context.renderer, root / image);

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

    const ImU32 color = IM_COL32(255, 255, 255, alpha);
    auto* draw_list = ImGui::GetWindowDrawList();  // ImGui is annoying
    draw_list->AddImage((ImTextureID)(intptr_t)texture, min, max, uv0, uv1, color);
  }

  else {
    drawBox(min, max, def.r, def.g, def.b, alpha);
  }
}

inline void renderEntityColliderOnPanel(EditorContext& context, const EntityDef& def, ImVec2 eMin, ImVec2 eMax) {
  auto* draw_list = ImGui::GetWindowDrawList();

  const float scaledWidth = eMax.x - eMin.x;
  const float scaledHeight = eMax.y - eMin.y;
  const ImVec2 center = {eMin.x + scaledWidth / 2, eMin.y + scaledHeight / 2};

  const std::string& type = def.colliderType;
  const auto& colliderColor = ImGui::ColorConvertFloat4ToU32({0, 1, 0, 1});
  const float& offsetX = def.colliderOffsetX;
  const float& offsetY = def.colliderOffsetY;
  const float& widthFraction = def.colliderWidthFraction;
  const float& heightFraction = def.colliderHeightFraction;

  // Render
  if (type == "rect") {
    const float colliderWidth = scaledWidth * widthFraction;
    const float colliderHeight = scaledHeight * heightFraction;
    SDL_FRect rect = {center.x - colliderWidth / 2, center.y - colliderHeight / 2, colliderWidth, colliderHeight};

    context.postImGuiRender.push_back(([rect, &context]() {
      SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
      SDL_RenderRect(context.renderer, &rect);
    }));
  }

  else if (type == "circle") {
    const ImVec2 colliderCenter = {center.x + offsetX, center.y + offsetY};
    draw_list->AddCircle(colliderCenter, scaledWidth * widthFraction / 2, colliderColor);
  }
}

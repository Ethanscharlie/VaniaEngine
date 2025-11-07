#pragma once

#include "EditorContext.hpp"
#include "GameDataStructs.hpp"
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

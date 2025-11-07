#pragma once

#include "EditorContext.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "run/AssetManager.hpp"

// https://stackoverflow.com/questions/38334081/how-to-draw-circles-arcs-and-vector-graphics-in-sdl
static void drawCircle(SDL_Renderer* renderer, SDL_FPoint center, int32_t radius) {
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderPoint(renderer, center.x + x, center.y - y);
    SDL_RenderPoint(renderer, center.x + x, center.y + y);
    SDL_RenderPoint(renderer, center.x - x, center.y - y);
    SDL_RenderPoint(renderer, center.x - x, center.y + y);
    SDL_RenderPoint(renderer, center.x + y, center.y - x);
    SDL_RenderPoint(renderer, center.x + y, center.y + x);
    SDL_RenderPoint(renderer, center.x - y, center.y - x);
    SDL_RenderPoint(renderer, center.x - y, center.y + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

inline void renderEntityOnPanel(EditorContext& context, const EntityDef& def, SDL_FPoint center, float scale = 1,
                                int alpha = 255) {
  SDL_FRect rect;
  rect.x = center.x - def.width * scale / 2;
  rect.y = center.y - def.height * scale / 2;
  rect.w = def.width * scale;
  rect.h = def.height * scale;

  if (def.imageMode) {
    auto& root = context.gameData.editorData.rootPath;
    const std::string& image = def.image;
    AssetManager& assetManager = AssetManager::getInstance();
    std::string texturePath = root / image;
    SDL_Texture* texture = assetManager.get(context.renderer, texturePath);

    SDL_FRect srcRect = {
        def.imageCol,     //
        def.imageRow,     //
        def.imageWidth,   //
        def.imageHeight,  //
    };

    if (texture == nullptr) {
      context.postImGuiRender.push_back(([rect, &context]() {
        SDL_SetRenderDrawColor(context.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(context.renderer, &rect);
      }));
      return;
    }

    context.postImGuiRender.push_back(([rect, srcRect, texturePath, &context]() {
      AssetManager& assetManager = AssetManager::getInstance();
      SDL_Texture* texture = assetManager.get(context.renderer, texturePath);
      SDL_RenderTexture(context.renderer, texture, &srcRect, &rect);
    }));
  }

  else {
    int r = def.r;
    int g = def.g;
    int b = def.b;
    int a = def.a;
    context.postImGuiRender.push_back(([rect, r, g, b, a, &context]() {
      SDL_SetRenderDrawColor(context.renderer, r, g, b, a);
      SDL_RenderFillRect(context.renderer, &rect);
    }));
  }
}

inline void renderEntityColliderOnPanel(EditorContext& context, const EntityDef& def, SDL_FPoint center,
                                        float scale = 1) {
  auto* draw_list = ImGui::GetWindowDrawList();

  const float scaledWidth = def.width * scale;
  const float scaledHeight = def.height * scale;

  const std::string& type = def.colliderType;
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
    const SDL_FPoint colliderCenter = {center.x + offsetX, center.y + offsetY};
    const float radius = scaledWidth * widthFraction / 2;

    context.postImGuiRender.push_back(([colliderCenter, radius, &context]() {
      SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
      drawCircle(context.renderer, colliderCenter, radius);
    }));
  }
}

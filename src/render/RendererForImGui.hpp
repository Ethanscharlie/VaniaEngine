#pragma once

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "imgui.h"
#include "render/IRenderer.hpp"
#include "run/AssetManager.hpp"

class RendererForImGui : public IRenderer {
  ImVec2 minFromRect(SDL_FRect rect) { return {rect.x, rect.y}; }
  ImVec2 maxFromRect(SDL_FRect rect) { return {rect.x + rect.w, rect.y + rect.h}; }
  ImColor colorFromColor(SDL_Color color) {
    float r = (float)color.r / 255.0f;
    float g = (float)color.g / 255.0f;
    float b = (float)color.b / 255.0f;
    float a = (float)color.a / 255.0f;
    return ImGui::GetColorU32({r, g, b, a});
  }

 public:
  RendererForImGui(EditorContext& context) : IRenderer(context) {}

  void drawRect(SDL_FRect rect, SDL_Color color) override {
    ImGui::GetWindowDrawList()->AddRect(minFromRect(rect), maxFromRect(rect), colorFromColor(color));
  }

  void drawFillRect(SDL_FRect rect, SDL_Color color) override {
    ImGui::GetWindowDrawList()->AddRectFilled(minFromRect(rect), maxFromRect(rect), colorFromColor(color));
  }

  void drawCircle(SDL_FPoint center, float radius, SDL_Color color) override {
    ImGui::GetWindowDrawList()->AddCircle({center.x, center.y}, radius, colorFromColor(color));
  }

  void drawAsset(SDL_FRect rect, SDL_FRect srcRect, const std::string& pathWithRoot, int alpha = 255) override {
    AssetManager& assetManager = AssetManager::getInstance();
    SDL_Texture* texture = assetManager.get(context.renderer, pathWithRoot);

    float textureW, textureH;
    SDL_GetTextureSize(texture, &textureW, &textureH);

    ImVec2 uv0 = {srcRect.x / textureW, srcRect.y / textureH};
    ImVec2 uv1 = {(srcRect.x + srcRect.w) / textureW, (srcRect.y + srcRect.h) / textureH};

    const auto color = colorFromColor({255, 255, 255, (Uint8)alpha});

    ImGui::GetWindowDrawList()->AddImage(texture, minFromRect(rect), maxFromRect(rect), uv0, uv1, color);
  }
};

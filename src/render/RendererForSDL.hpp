#pragma once

#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "panels/EditorContext.hpp"
#include "render/IRenderer.hpp"
#include "run/AssetManager.hpp"

class RendererForSDL : public IRenderer {
 public:
  RendererForSDL(EditorContext& context) : IRenderer(context) {}

  void drawRect(SDL_FRect rect, SDL_Color color) override {
    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderRect(context.renderer, &rect);
  }

  void drawFillRect(SDL_FRect rect, SDL_Color color) override {
    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(context.renderer, &rect);
  }

  void drawCircle(SDL_FPoint center, float radius, SDL_Color color) override {
    SDL_SetRenderDrawColor(context.renderer, color.r, color.g, color.b, color.a);

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
      //  Each of the following renders an octant of the circle
      SDL_RenderPoint(context.renderer, center.x + x, center.y - y);
      SDL_RenderPoint(context.renderer, center.x + x, center.y + y);
      SDL_RenderPoint(context.renderer, center.x - x, center.y - y);
      SDL_RenderPoint(context.renderer, center.x - x, center.y + y);
      SDL_RenderPoint(context.renderer, center.x + y, center.y - x);
      SDL_RenderPoint(context.renderer, center.x + y, center.y + x);
      SDL_RenderPoint(context.renderer, center.x - y, center.y - x);
      SDL_RenderPoint(context.renderer, center.x - y, center.y + x);

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

  void drawAsset(SDL_FRect rect, SDL_FRect srcRect, const std::string& pathWithRoot, int alpha = 255) override {
    AssetManager& assetManager = AssetManager::getInstance();
    SDL_Texture* texture = assetManager.get(context.renderer, pathWithRoot);

    // TODO Alpha

    SDL_RenderTexture(context.renderer, texture, &srcRect, &rect);
  }
};

#pragma once

#include <algorithm>
#include <string>

#include "GameDataStructs.hpp"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_rect.h"
#include "panels/EditorContext.hpp"

class IRenderer {
 protected:
  EditorContext& context;

 public:
  const SDL_Color COLLIDER_COLOR = {0, 255, 0, 255};

  IRenderer(EditorContext& context) : context(context) {}

  virtual void drawRect(SDL_FRect rect, SDL_Color color) = 0;
  virtual void drawFillRect(SDL_FRect rect, SDL_Color color) = 0;
  virtual void drawCircle(SDL_FPoint center, float radius, SDL_Color color) = 0;
  virtual void drawAsset(SDL_FRect rect, SDL_FRect srcRect, const std::string& pathWithRoot, int alpha = 255) = 0;

  void drawEntity(const EntityDef& def, SDL_FPoint center, int scale = 1) {
    SDL_FRect rect;
    rect.x = center.x - def.width * scale / 2;
    rect.y = center.y - def.height * scale / 2;
    rect.w = def.width * scale;
    rect.h = def.height * scale;

    if (def.imageMode) {
      SDL_FRect srcRect = {def.imageCol, def.imageRow, def.imageWidth, def.imageHeight};
      const std::string& image = context.gameData.editorData.rootPath / def.image;
      drawAsset(rect, srcRect, image, def.a);
    }

    else {
      SDL_Color color;
      color.r = def.r;
      color.g = def.g;
      color.b = def.b;
      color.a = def.a;
      drawFillRect(rect, color);
    }
  }

  void drawCollider(const EntityDef& def, SDL_FPoint center, int scale = 1) {
    const float scaledWidth = def.width * scale;
    const float scaledHeight = def.height * scale;

    const std::string& type = def.colliderType;
    const float& offsetX = def.colliderOffsetX;
    const float& offsetY = def.colliderOffsetY;
    const float& widthFraction = def.colliderWidthFraction;
    const float& heightFraction = def.colliderHeightFraction;

    if (type == "rect") {
      const float colliderWidth = scaledWidth * widthFraction;
      const float colliderHeight = scaledHeight * heightFraction;
      SDL_FRect rect = {center.x - colliderWidth / 2, center.y - colliderHeight / 2, colliderWidth, colliderHeight};
      drawRect(rect, COLLIDER_COLOR);
    }

    else if (type == "circle") {
      const SDL_FPoint colliderCenter = {center.x + offsetX, center.y + offsetY};
      const float radius = scaledWidth * widthFraction / 2;
      drawCircle(colliderCenter, radius, COLLIDER_COLOR);
    }
  }
};

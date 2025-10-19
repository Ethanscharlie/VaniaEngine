#pragma once

#include "AssetManager.hpp"
#include "GameDataStructs.hpp"
#include "SDL3/SDL.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

namespace Vania::Render {
inline void render(SDL_Renderer* renderer, const Entity& entity, const EntityDef& def,
                   const std::filesystem::path root) {
  SDL_FRect rect = {
      entity.x,                        //
      entity.y,                        //
      entity.entityDefOverride.width,  //
      entity.entityDefOverride.height  //
  };

  if (def.imageMode) {
    SDL_Texture* texture = AssetManager::getInstance().get(renderer, root / def.image);
    SDL_FRect srcRect = {
        def.imageCol,     //
        def.imageRow,     //
        def.imageWidth,   //
        def.imageHeight,  //
    };

    SDL_RenderTexture(renderer, texture, &srcRect, &rect);
  }
  //
  else {
    SDL_SetRenderDrawColor(renderer, entity.entityDefOverride.r, entity.entityDefOverride.g, entity.entityDefOverride.b,
                           entity.entityDefOverride.a);
    SDL_RenderFillRect(renderer, &rect);
  }
}

}  // namespace Vania::Render

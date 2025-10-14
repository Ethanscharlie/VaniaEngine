#include "AssetManager.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"

std::map<std::string, SDL_Texture *> AssetManager::assets;

SDL_Texture *AssetManager::get(SDL_Renderer *renderer,
                               const std::string &name) {
  if (assets.contains(name)) {
    return assets.at(name);
  }

  SDL_Texture *texture = IMG_LoadTexture(renderer, name.c_str());
  assets[name] = texture;
  return texture;
}

void AssetManager::clear() {
  for (auto &[name, texture] : assets) {
    SDL_DestroyTexture(texture);
  }

  assets.clear();
}

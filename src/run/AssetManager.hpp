#include "SDL3/SDL_render.h"
#include <map>
#include <string>
class AssetManager {
  static std::map<std::string, SDL_Texture *> assets;

public:
  static AssetManager &getInstance() {
    static AssetManager instance;
    return instance;
  }

  static SDL_Texture *get(SDL_Renderer *renderer, const std::string &name);
  static void clear();

private:
  AssetManager() {}

public:
  AssetManager(AssetManager const &) = delete;
  void operator=(AssetManager const &) = delete;
};

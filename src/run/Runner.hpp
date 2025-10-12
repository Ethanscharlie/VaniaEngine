#include "GameDataStructs.hpp"
#include "SDL3/SDL_render.h"
namespace Vania {

class Runner {
public:
  const int DISPLAY_WIDTH = 700;
  const int DISPLAY_HEIGHT = 700;

  SDL_Texture *displayTexture;

private:
  const GameData &gameData;
  SDL_Renderer *renderer;

  GameData instanceOfGameData;

public:
  Runner(const GameData &gameData, SDL_Renderer *renderer);
  ~Runner();
  void reset();
  void update();
};

}; // namespace Vania

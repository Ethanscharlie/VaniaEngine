#include "Runner.hpp"
#include "SDL3/SDL_render.h"
#include <print>

namespace Vania {

Runner::Runner(const GameData &gameData, SDL_Renderer *renderer)
    : gameData(gameData), renderer(renderer) {
  reset();

  displayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH,
                                     DISPLAY_HEIGHT);
}

Runner::~Runner() { SDL_DestroyTexture(displayTexture); }

void Runner::reset() { instanceOfGameData = gameData; }

void Runner::update() {
  SDL_SetRenderTarget(renderer, displayTexture);

  SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderTarget(renderer, nullptr);
}

} // namespace Vania

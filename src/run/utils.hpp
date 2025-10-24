#include <algorithm>
#include <cmath>
#include <string>

#include "GameDataStructs.hpp"
#include "Runner.hpp"
#include "SDL3/SDL_mouse.h"

namespace Vania::utils {

inline bool getButtonHeld(const std::string& button) {
  std::string b = button;
  std::transform(b.begin(), b.end(), b.begin(), ::tolower);

  const bool* k = SDL_GetKeyboardState(NULL);

  if (k[SDL_SCANCODE_SPACE] && (b == "" || b == " ")) return true;
  if (k[SDL_SCANCODE_A] && b == "a") return true;
  if (k[SDL_SCANCODE_B] && b == "b") return true;
  if (k[SDL_SCANCODE_C] && b == "c") return true;
  if (k[SDL_SCANCODE_D] && b == "d") return true;
  if (k[SDL_SCANCODE_E] && b == "e") return true;
  if (k[SDL_SCANCODE_F] && b == "f") return true;
  if (k[SDL_SCANCODE_G] && b == "g") return true;
  if (k[SDL_SCANCODE_H] && b == "h") return true;
  if (k[SDL_SCANCODE_I] && b == "i") return true;
  if (k[SDL_SCANCODE_J] && b == "j") return true;
  if (k[SDL_SCANCODE_K] && b == "k") return true;
  if (k[SDL_SCANCODE_L] && b == "l") return true;
  if (k[SDL_SCANCODE_M] && b == "m") return true;
  if (k[SDL_SCANCODE_N] && b == "n") return true;
  if (k[SDL_SCANCODE_O] && b == "o") return true;
  if (k[SDL_SCANCODE_P] && b == "p") return true;
  if (k[SDL_SCANCODE_Q] && b == "q") return true;
  if (k[SDL_SCANCODE_R] && b == "r") return true;
  if (k[SDL_SCANCODE_S] && b == "s") return true;
  if (k[SDL_SCANCODE_T] && b == "t") return true;
  if (k[SDL_SCANCODE_U] && b == "u") return true;
  if (k[SDL_SCANCODE_V] && b == "v") return true;
  if (k[SDL_SCANCODE_W] && b == "w") return true;
  if (k[SDL_SCANCODE_X] && b == "x") return true;
  if (k[SDL_SCANCODE_Y] && b == "y") return true;
  if (k[SDL_SCANCODE_Z] && b == "z") return true;
  if (k[SDL_SCANCODE_UP] && b == "up") return true;
  if (k[SDL_SCANCODE_DOWN] && b == "down") return true;
  if (k[SDL_SCANCODE_LEFT] && b == "left") return true;
  if (k[SDL_SCANCODE_RIGHT] && b == "right") return true;

  return false;
}

inline bool getMouseButtonHeld(const std::string& button) {
  std::string b = button;
  std::transform(b.begin(), b.end(), b.begin(), ::tolower);

  SDL_MouseButtonFlags mouseState = SDL_GetMouseState(nullptr, nullptr);

  if (SDL_BUTTON_MASK(mouseState) == SDL_BUTTON_LMASK && b == "left") return true;
  if (SDL_BUTTON_MASK(mouseState) == SDL_BUTTON_RMASK && b == "right") return true;
  if (SDL_BUTTON_MASK(mouseState) == SDL_BUTTON_MMASK && b == "middle") return true;

  return false;
}

inline void moveForward(Entity& entity, float speed) {
  const float radians = entity.angle * (M_PI / 180.0);
  entity.x += cos(radians) * speed;
  entity.y += sin(radians) * speed;
}

inline void lookAt(Entity& entity, float targetX, float targetY) {
  const float deltaX = targetX - entity.x;
  const float deltaY = targetY - entity.y;
  entity.angle = std::atan2(deltaY, deltaX) * (180.0 / M_PI);
}

inline void exposeAll(sol::state& lua) {
  lua.set_function("getButtonHeld", getButtonHeld);
  lua.set_function("getMouseButtonHeld", getMouseButtonHeld);
  lua.set_function("moveForward", moveForward);
  lua.set_function("lookAt", lookAt);
}
}  // namespace Vania::utils

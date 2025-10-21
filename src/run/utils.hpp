#include <algorithm>
#include <cmath>
#include <string>

#include "GameDataStructs.hpp"
#include "Runner.hpp"

namespace Vania::utils {
inline void setPropStr(Entity& entity, const std::string& key, const std::string& value) {
  entity.entityDefOverride.properties[key] = Property(0.0f, value);
}

inline void setPropNum(Entity& entity, const std::string& key, float value) {
  entity.entityDefOverride.properties[key] = Property(value, "");
}

inline std::string& getPropStr(Entity& entity, const std::string& key) {
  return entity.entityDefOverride.properties[key].text;
}

inline int getPropNum(Entity& entity, const std::string& key) {
  return entity.entityDefOverride.properties[key].number;
}

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

inline void moveForward(Entity& entity, float speed) {
  const float radians = entity.angle * (M_PI / 180.0);
  entity.x += cos(radians) * speed;
  entity.y += sin(radians) * speed;
}

inline void exposeAll(sol::state& lua) {
  lua.set_function("setPropStr", setPropStr);
  lua.set_function("setPropNum", setPropNum);
  lua.set_function("getPropStr", getPropStr);
  lua.set_function("getPropNum", getPropNum);
  lua.set_function("getButtonHeld", getButtonHeld);
  lua.set_function("moveForward", moveForward);
}
}  // namespace Vania::utils

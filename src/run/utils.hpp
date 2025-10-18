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

inline void exposeAll(sol::state& lua) {
  lua.set_function("setPropStr", setPropStr);
  lua.set_function("setPropNum", setPropNum);
  lua.set_function("getPropStr", getPropStr);
  lua.set_function("getPropNum", getPropNum);
}
}  // namespace Vania::utils

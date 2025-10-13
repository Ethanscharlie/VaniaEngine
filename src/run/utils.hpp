#include "Runner.hpp"
#include <string>

namespace Vania::utils {
inline void setPropStr(RuntimeEntity &entity, const std::string &key,
                       const std::string &value) {
  entity.entityDef.properties[key] = Property(0.0f, value);
}

inline void setPropNum(RuntimeEntity &entity, const std::string &key,
                       float value) {
  entity.entityDef.properties[key] = Property(value, "");
}

inline std::string &getPropStr(RuntimeEntity &entity, const std::string &key) {
  return entity.entityDef.properties[key].text;
}

inline int getPropNum(RuntimeEntity &entity, const std::string &key) {
  return entity.entityDef.properties[key].number;
}

inline void exposeAll(sol::state &lua) {
  lua.set_function("setPropStr", setPropStr);
  lua.set_function("setPropNum", setPropNum);
  lua.set_function("getPropStr", getPropStr);
  lua.set_function("getPropNum", getPropNum);
}
} // namespace Vania::utils

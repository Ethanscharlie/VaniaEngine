#include "Runner.hpp"
#include <string>

namespace Vania::utils {
inline void setProp(RuntimeEntity &entity, const std::string &key,
                    const std::string &value) {
  entity.entityDef.properties[key] = Property(0.0f, value);
}

inline std::string &getProp(RuntimeEntity &entity, const std::string &key) {
  return entity.entityDef.properties[key].text;
}

inline void exposeAll(sol::state &lua) {
  lua.set_function("setProp", setProp);
  lua.set_function("getProp", getProp);
}
} // namespace Vania::utils

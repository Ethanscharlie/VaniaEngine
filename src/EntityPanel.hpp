#include "EntityDef.hpp"
#include "IPanel.hpp"
#include <string>
#include <vector>

namespace Vania {
class EntityPanel : public IPanel {
  std::vector<EntityDef> entities;

public:
  EntityPanel();
  void update() override;
};
} // namespace Vania

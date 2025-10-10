#include "GameDataStructs.hpp"
#include "IPanel.hpp"
#include <vector>

namespace Vania {
class EntityPanel : public IPanel {
  std::vector<EntityDef> entities;

public:
  EntityPanel();
  void update() override;
};
} // namespace Vania

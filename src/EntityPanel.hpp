#include "IPanel.hpp"

namespace Vania {
class EntityPanel : public IPanel {
public:
  EntityPanel();
  void update() override;
};
} // namespace Vania

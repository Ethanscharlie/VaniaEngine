#include "IPanel.hpp"
#include <string>
#include <vector>

namespace Vania {
class EntityPanel : public IPanel {
  std::vector<std::string> entities;

public:
  EntityPanel();
  void update() override;
};
} // namespace Vania

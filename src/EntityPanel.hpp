#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"

namespace Vania {
class EntityPanel : public IPanel {
  nlohmann::json& gameData;

 public:
  EntityPanel(nlohmann::json& gameData);
  void update() override;

 private:
  void showPropertyEditor();
};
}  // namespace Vania

#include <vector>

#include "GameDataStructs.hpp"
#include "IPanel.hpp"

namespace Vania {
class EntityPanel : public IPanel {
  GameData& gameData;

 public:
  EntityPanel(GameData& gameData);
  void update() override;

 private:
  void showPropertyEditor();
  EntityDef& createDef();
};
}  // namespace Vania

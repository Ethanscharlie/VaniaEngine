#pragma once

#include "panels/EditorContext.hpp"
#include "panels/IPanel.hpp"

namespace Vania {
class ProjectPanel : public IPanel {
  EditorContext& context;

 public:
  ProjectPanel(EditorContext& context);
  void update() override;
};
}  // namespace Vania

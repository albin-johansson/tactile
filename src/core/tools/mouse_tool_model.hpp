#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/unique.hpp"
#include "mouse_tool.hpp"
#include "mouse_tool_type.hpp"

#include "core/events/mouse_drag_event.hpp"

namespace Tactile {

class Model;

class MouseToolModel final
{
 public:
  explicit MouseToolModel(Model* model);

  void Select(MouseToolType type);

  void OnMouseDragged(const MouseDragEvent& event);

  [[nodiscard]] auto GetActive() const -> MouseToolType;

 private:
  AMouseTool* mCurrent{};
  rune::vector_map<MouseToolType, Unique<AMouseTool>> mTools;

  void SwitchTo(AMouseTool* tool);
};

}  // namespace Tactile

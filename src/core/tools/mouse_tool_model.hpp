#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/not_null.hpp"
#include "aliases/unique.hpp"
#include "core/events/tools/mouse_drag_event.hpp"
#include "core/events/tools/mouse_pressed_event.hpp"
#include "core/events/tools/mouse_released_event.hpp"
#include "mouse_tool.hpp"
#include "mouse_tool_type.hpp"

namespace Tactile {

class Model;

class MouseToolModel final
{
 public:
  explicit MouseToolModel(NotNull<Model*> model);

  void Select(MouseToolType type);

  void OnMousePressed(const MousePressedEvent& event);

  void OnMouseReleased(const MouseReleasedEvent& event);

  void OnMouseDragged(const MouseDragEvent& event);

  [[nodiscard]] auto GetActive() const -> MouseToolType;

 private:
  AMouseTool* mCurrent{};
  rune::vector_map<MouseToolType, Unique<AMouseTool>> mTools;

  void SwitchTo(AMouseTool* tool);
};

}  // namespace Tactile

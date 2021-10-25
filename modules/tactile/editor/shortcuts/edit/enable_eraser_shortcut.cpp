#include "enable_eraser_shortcut.hpp"

#include "editor/events/tool_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/model.hpp"

namespace Tactile {

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e}
{}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
}

auto EnableEraserShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile

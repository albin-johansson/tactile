#include "enable_eraser_shortcut.hpp"

#include "core/model.hpp"
#include "events/tool_events.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"

namespace Tactile {

EnableEraserShortcut::EnableEraserShortcut() : AShortcut{cen::scancodes::e}
{}

void EnableEraserShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Eraser);
}

auto EnableEraserShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsViewportFocused();
}

}  // namespace Tactile

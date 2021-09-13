#include "enable_stamp_shortcut.hpp"

#include "core/model.hpp"
#include "events/tool_events.hpp"
#include "gui/widgets/viewport/viewport_widget.hpp"

namespace Tactile {

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s}
{}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
}

auto EnableStampShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsViewportFocused();
}

}  // namespace Tactile

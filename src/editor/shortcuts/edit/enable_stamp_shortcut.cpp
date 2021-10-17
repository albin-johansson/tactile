#include "enable_stamp_shortcut.hpp"

#include "editor/events/tool_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/model.hpp"

namespace Tactile {

EnableStampShortcut::EnableStampShortcut() : AShortcut{cen::scancodes::s}
{}

void EnableStampShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<SelectToolEvent>(ToolType::Stamp);
}

auto EnableStampShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile

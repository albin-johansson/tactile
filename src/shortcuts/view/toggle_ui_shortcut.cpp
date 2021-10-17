#include "toggle_ui_shortcut.hpp"

#include "core/model.hpp"
#include "gui/events/view_events.hpp"
#include "gui/widgets/focus.hpp"

namespace Tactile {

ToggleUiShortcut::ToggleUiShortcut() : AShortcut{cen::scancodes::tab}
{}

void ToggleUiShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ToggleUiEvent>();
}

auto ToggleUiShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument() && IsEditorFocused();
}

}  // namespace Tactile

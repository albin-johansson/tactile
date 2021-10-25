#include "toggle_ui_shortcut.hpp"

#include "editor/events/view_events.hpp"
#include "editor/gui/widgets/focus.hpp"
#include "editor/model.hpp"

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

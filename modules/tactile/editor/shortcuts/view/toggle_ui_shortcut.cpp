#include "toggle_ui_shortcut.hpp"

#include "tactile/editor/events/view_events.hpp"
#include "tactile/editor/gui/widgets/focus.hpp"
#include "tactile/editor/model.hpp"

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

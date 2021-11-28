#include "pan_left_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

PanLeftShortcut::PanLeftShortcut() : AShortcut{cen::scancodes::left, KMOD_LCTRL}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

auto PanLeftShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

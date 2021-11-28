#include "pan_right_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

PanRightShortcut::PanRightShortcut() : AShortcut{cen::scancodes::right, KMOD_LCTRL}
{}

void PanRightShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

auto PanRightShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

#include "pan_up_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

PanUpShortcut::PanUpShortcut() : AShortcut{cen::scancodes::up, KMOD_LCTRL}
{}

void PanUpShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

auto PanUpShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

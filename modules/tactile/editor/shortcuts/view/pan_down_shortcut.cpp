#include "pan_down_shortcut.hpp"

#include "editor/events/viewport_events.hpp"
#include "editor/model.hpp"

namespace Tactile {

PanDownShortcut::PanDownShortcut() : AShortcut{cen::scancodes::down, KMOD_LCTRL}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

auto PanDownShortcut::IsEnabled(const Model& model) const -> bool
{
  return model.HasActiveDocument();
}

}  // namespace Tactile

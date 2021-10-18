#include "pan_down_shortcut.hpp"

#include "tactile/editor/events/viewport_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, cen::key_mod::left_ctrl}
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

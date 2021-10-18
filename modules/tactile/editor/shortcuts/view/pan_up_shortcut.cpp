#include "pan_up_shortcut.hpp"

#include "tactile/editor/events/viewport_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

PanUpShortcut::PanUpShortcut() : AShortcut{cen::scancodes::up, cen::key_mod::left_ctrl}
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

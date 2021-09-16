#include "pan_up_shortcut.hpp"

#include "core/model.hpp"
#include "events/viewport_events.hpp"

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

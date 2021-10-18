#include "pan_right_shortcut.hpp"

#include "tactile/editor/events/viewport_events.hpp"
#include "tactile/editor/model.hpp"

namespace Tactile {

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, cen::key_mod::left_ctrl}
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

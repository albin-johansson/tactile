#include "pan_right_shortcut.hpp"

#include "events/viewport/pan_right_event.hpp"

namespace Tactile {

PanRightShortcut::PanRightShortcut()
    : AShortcut{cen::scancodes::right, cen::key_mod::left_ctrl}
{}

void PanRightShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanRightEvent>();
}

}  // namespace Tactile

#include "pan_down_shortcut.hpp"

#include "events/viewport_events.hpp"

namespace Tactile {

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, cen::key_mod::left_ctrl}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

}  // namespace Tactile

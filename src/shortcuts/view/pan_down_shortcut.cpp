#include "pan_down_shortcut.hpp"

#include "events/viewport/pan_down_event.hpp"

namespace Tactile {

PanDownShortcut::PanDownShortcut()
    : AShortcut{cen::scancodes::down, cen::key_mod::left_ctrl}
{}

void PanDownShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanDownEvent>();
}

}  // namespace Tactile

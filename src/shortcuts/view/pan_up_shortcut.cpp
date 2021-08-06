#include "pan_up_shortcut.hpp"

#include "events/viewport/pan_up_event.hpp"

namespace Tactile {

PanUpShortcut::PanUpShortcut()
    : AShortcut{cen::scancodes::up, cen::key_mod::left_ctrl}
{}

void PanUpShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanUpEvent>();
}

}  // namespace Tactile

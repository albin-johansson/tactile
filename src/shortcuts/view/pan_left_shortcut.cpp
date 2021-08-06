#include "pan_left_shortcut.hpp"

#include "events/viewport/pan_left_event.hpp"

namespace Tactile {

PanLeftShortcut::PanLeftShortcut()
    : AShortcut{cen::scancodes::left, cen::key_mod::left_ctrl}
{}

void PanLeftShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<PanLeftEvent>();
}

}  // namespace Tactile

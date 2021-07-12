#include "new_map_shortcut.hpp"

#include "core/events/maps/add_map_event.hpp"

namespace Tactile {

NewMapShortcut::NewMapShortcut()
    : AShortcut{cen::scancodes::n, cen::key_mod::left_ctrl}
{}

void NewMapShortcut::Activate(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddMapEvent>();
}

}  // namespace Tactile
